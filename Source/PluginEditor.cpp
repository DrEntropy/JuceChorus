/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KadenzeChorusProcessorEditor::KadenzeChorusProcessorEditor (KadenzeChorusProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 300);
 
  //  DBG("InEditorContructor:"<< *dryWetParameter);
   aDryWetSlider= SliderHelper(mDryWetSlider, 0 ,0,"drywet");
   aFeedbackSlider= SliderHelper(mFeedbackSlider,100,0,"feedback");
   aDepthSlider= SliderHelper(mDepthSlider,200,0,"depth");
   aRateSlider= SliderHelper(mRateSlider,300,0,"rate");
   aPhaseOffsetSlider= SliderHelper(mPhaseOffsetSlider,0,100,"offset");
    
    mType.addItem("Chorus", 1);
    mType.addItem("Flanger",2);
    // TODO fix this so it uses the actual parameter
    mType.setSelectedItemIndex(1);
    mType.setBounds(100,100,100,30);
    aTypeBox = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(p.apvts,"type",mType);
    
    addAndMakeVisible(mType);
    

    
    
}



//DRY - er

std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> KadenzeChorusProcessorEditor::SliderHelper(Slider& aSlider,int xpos,int ypos,const char* paramLabel)
{
    aSlider.setBounds(xpos, ypos, 100, 100);
    aSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    aSlider.setTextBoxStyle(Slider::NoTextBox, true, 0,0);
    
    // grab the parameter for what we are doing below
    auto param = (AudioParameterFloat *) processor.apvts.getParameter(paramLabel);
    //DBG("Param:" << paramLabel << " value start:" << param->range.start << " vale end:" <<param->range.end);
    
    aSlider.setRange(param-> range.start,param->range.end);
    aSlider.setValue(*param);
    addAndMakeVisible(aSlider);
    
    //Generate a unique pointer to a new slider attachment and return it. 
    
    return std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts,paramLabel,aSlider);
    
    
    
     
}

KadenzeChorusProcessorEditor::~KadenzeChorusProcessorEditor()
{
}

//==============================================================================
void KadenzeChorusProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Final Project!", getLocalBounds(), Justification::centred, 1);
}

void KadenzeChorusProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
