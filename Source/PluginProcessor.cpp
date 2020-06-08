/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KadenzeChorusProcessor::KadenzeChorusProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
 , apvts(*this,nullptr,"PARAMETERS",{
         std::make_unique<AudioParameterFloat>("drywet","DryWet",0.0f,1.0f,0.5f),
         std::make_unique<AudioParameterFloat>("feedback","Feedback",0.0f,0.98f,0.0f),
         std::make_unique<AudioParameterFloat>("depth","Depth",0.01f,1.0f,0.5f),
         std::make_unique<AudioParameterFloat>("rate","Rate",0.1f,20.0f,10.0f),
         std::make_unique<AudioParameterFloat>("offset","Phase Offset",0.0f,1.0,0.1f),
         std::make_unique<AudioParameterInt>("type","Type",0,1,0)
      })
{

    // parameters are defined in the member initialization above.
    

                 
    mCircularBufferL = nullptr;
    mCircularBufferR = nullptr;
    mCircularBufferWriteHead=0;
    mCircularBufferLength=0;
    //mDelayTimeSamples =0.0;
    //mCircularBufferReadHead = 0;
    mFeedbackL=0;
    mFeedbackR=0;
    
    mLFOPhase =0;
  //  mTimeSmoothed=0;
 
}

KadenzeChorusProcessor::~KadenzeChorusProcessor()
{
    if(mCircularBufferL != nullptr) {
      delete [] mCircularBufferL;
        mCircularBufferL= nullptr;
    }
    if(mCircularBufferR != nullptr){
        delete [] mCircularBufferR;
        mCircularBufferR = nullptr;
    }
    
  //   The parameter objects are managed and deleted automatically by the /// AudioProcessor when no longer needed.


  
}

//==============================================================================
const String KadenzeChorusProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KadenzeChorusProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KadenzeChorusProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KadenzeChorusProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double KadenzeChorusProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KadenzeChorusProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KadenzeChorusProcessor::getCurrentProgram()
{
    return 0;
}

void KadenzeChorusProcessor::setCurrentProgram (int index)
{
}

const String KadenzeChorusProcessor::getProgramName (int index)
{
    return {};
}

void KadenzeChorusProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void KadenzeChorusProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mCircularBufferLength=  sampleRate*MAX_DELAY_TIME;
    
    //sample rate may have changed, make sure to delete the old
    // should use smart pointers, but i am not smart enough.
     if(mCircularBufferL != nullptr) {
       delete [] mCircularBufferL;
         mCircularBufferL= nullptr;
     }
     if(mCircularBufferR != nullptr){
         delete [] mCircularBufferR;
         mCircularBufferR = nullptr;
     }
    
    // THere is no way at this point that these buffers are not null, but whatever
    
    if(mCircularBufferL== nullptr) {
        mCircularBufferL = new float[mCircularBufferLength];
    }
    if(mCircularBufferR== nullptr){
           mCircularBufferR = new float[mCircularBufferLength];
       }
    //  zero out the buffers.
    for(int i = 0;i< mCircularBufferLength;++i)
    {
        mCircularBufferR[i]=0.0;
        mCircularBufferL[i]=0.0;
    }
    mCircularBufferWriteHead=0;
   
   // mTimeSmoothed = 1.0;
    mLFOPhase=0;
}

void KadenzeChorusProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KadenzeChorusProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

// helper function
float KadenzeChorusProcessor::remix(float dry, float wet)
{
    float drywet = *apvts.getRawParameterValue("drywet");
    float mix= drywet;
    return dry *(1.0- mix) + mix*wet;
}

void KadenzeChorusProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    
 
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
     

    
    // grab all the parameters to the stack from teh apvts. Except dry wet which is done in a helper function where it is used.
    float phaseOffset = *apvts.getRawParameterValue("offset");
    float feedbackAmt = *apvts.getRawParameterValue("feedback");
    float lfoDepth= *apvts.getRawParameterValue("depth");
    float lfoRate = *apvts.getRawParameterValue("rate");
    int type = *apvts.getRawParameterValue("type");

    
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    // should be 2, but just in case something got borked.
    if(totalNumOutputChannels== 2)
    {
    
    auto* leftChannel = buffer.getWritePointer (0);
    auto* rightChannel = buffer.getWritePointer(1);
    
    
    for (auto i = 0;i< buffer.getNumSamples();++i)
    {
 
       // write into the buffer with the feedback.
          
          mCircularBufferL[mCircularBufferWriteHead] = leftChannel[i]+mFeedbackL;
          mCircularBufferR[mCircularBufferWriteHead] = rightChannel[i]+mFeedbackR;
       
        
        //LFO Computations
        
        
        // compute right channel phase.
        float LFOPhaseRight = mLFOPhase + phaseOffset;
        //wrap phase
        if(LFOPhaseRight >1)
                  LFOPhaseRight -= 1;
        
        
        // remap depth to the a nonlinear function to make more responsive.
        float curvedDepth = (exp(lfoDepth * CURVE_SHAPE)-1)/exp(CURVE_SHAPE);
        
        // Calculate current LFO state
        float lfoOutLeft = curvedDepth*sin(2*M_PI*mLFOPhase );
        float lfoOutRight = curvedDepth*sin(2*M_PI*LFOPhaseRight );

        
        
        // REMAP to delay ranges appropriate to teh particular type of effect.
        
        float lfoOutMappedLeft;
        float lfoOutMappedRight;
        
        if(type  == 0)
        {
        //chorus
          lfoOutMappedLeft= jmap(lfoOutLeft, -1.0f, 1.0f, 0.005f, 0.030f);
          lfoOutMappedRight= jmap(lfoOutRight, -1.0f, 1.0f, 0.005f, 0.030f);
        } else {
        // flanger
          lfoOutMappedLeft= jmap(lfoOutLeft, -1.0f, 1.0f, 0.001f, 0.005f);
          lfoOutMappedRight= jmap(lfoOutRight, -1.0f, 1.0f, 0.001f, 0.005f);
            
        }
        
        // Advance LFO Phase
        mLFOPhase += lfoRate/getSampleRate();

        if(mLFOPhase > 1)
            mLFOPhase -= 1;
        
 
 
        
    // Compute delay in samples, modulated by LFO.
       float delayTimeSamplesLeft = lfoOutMappedLeft * getSampleRate();
       float delayTimeSamplesRight = lfoOutMappedRight * getSampleRate();
   
    
    // compute read heads
       float delayReadHeadLeft = mCircularBufferWriteHead-delayTimeSamplesLeft;
           if(delayReadHeadLeft <0)
               delayReadHeadLeft  += mCircularBufferLength;
        
        float delayReadHeadRight = mCircularBufferWriteHead-delayTimeSamplesRight;
             if(delayReadHeadRight <0)
                 delayReadHeadRight  += mCircularBufferLength;
       
        
    // get the interpolated sample
        float delayedSampleL =sampleInterp(delayReadHeadLeft, mCircularBufferL);
        float delayedSampleR =sampleInterp(delayReadHeadRight, mCircularBufferR);
        
    // Save feedback.
        mFeedbackL = delayedSampleL * (feedbackAmt);
        mFeedbackR = delayedSampleR *  (feedbackAmt);
        
        
 // move  buffer write head.
        mCircularBufferWriteHead++;
        if(mCircularBufferWriteHead >= mCircularBufferLength)
            mCircularBufferWriteHead=0;
        
   // write to audio channel
        
        leftChannel[i] = remix(leftChannel[i],delayedSampleL);
        rightChannel[i] = remix(rightChannel[i],delayedSampleR);
 
    }
        
    }
}

float KadenzeChorusProcessor::sampleInterp(float readHead, float* buffer)
{
    int readHead_x = readHead;
    float inPhase  = readHead-readHead_x;
    int readHead_x1 = readHead_x+1;
         
    if(readHead_x1>=mCircularBufferLength)
             readHead_x1=0;
         
         
         
   return lin_interp(buffer[readHead_x],buffer[readHead_x1],inPhase);
    
    
    
}

//==============================================================================
bool KadenzeChorusProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* KadenzeChorusProcessor::createEditor()
{
    return new KadenzeChorusProcessorEditor (*this);
}

//==============================================================================





// The code below was copied right from the tutorial on saving state as Value trees.



void KadenzeChorusProcessor::getStateInformation (MemoryBlock& destData)
{
    
    // Grab a copy of the state to make sure we are all synced and thread safe.
   auto state = apvts.copyState();
   std::unique_ptr<XmlElement> xml (state.createXml());
   copyXmlToBinary (*xml, destData);
//
//    copyXmlToBinary(*xml, destData);
}

void KadenzeChorusProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
 
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KadenzeChorusProcessor();
}


float KadenzeChorusProcessor::lin_interp(float inSampleX,float inSampleY,float inFloatPhase)
{
    return (1 - inFloatPhase) * inSampleX  + inFloatPhase * inSampleY;
    
}
