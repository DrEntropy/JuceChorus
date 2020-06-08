/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define MAX_DELAY_TIME 2

#define CURVE_SHAPE 3

//==============================================================================
/**
*/
class KadenzeChorusProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    KadenzeChorusProcessor();
    ~KadenzeChorusProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    
    
    // ALL parameteres are in this Value Tree.
    AudioProcessorValueTreeState apvts;

private:
    
    
    
 
   
    // LFO Data
    
    float mLFOPhase;
    
    //Circular buffer
    float *mCircularBufferL;
    float *mCircularBufferR;
    int mCircularBufferWriteHead;
    int mCircularBufferLength;
 
    
    // Feedback samples.
    float mFeedbackL;
    float mFeedbackR;
  
    
    // Private helper functions
    float remix(float dry,float wet);
    float sampleInterp(float readHead, float* buffer);

    float lin_interp(float sample_x,float sample_x1,float inPhase);
     
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KadenzeChorusProcessor)
};
