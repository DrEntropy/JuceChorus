/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class KadenzeChorusProcessorEditor  : public AudioProcessorEditor
{
public:
    KadenzeChorusProcessorEditor (KadenzeChorusProcessor&);
    ~KadenzeChorusProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    KadenzeChorusProcessor& processor;
    
    // My controls
    
    Slider mDryWetSlider;
    Slider mFeedbackSlider;
    Slider mDepthSlider;
    Slider mRateSlider;
    Slider mPhaseOffsetSlider;
    
    ComboBox mType;
    
    // Used for repetitious slider code
    
    // all the slider attachemnts and teh combo box attachment.  These are declared last so that they are destroyed first as required
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>  aDryWetSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>  aFeedbackSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>  aDepthSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>  aRateSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>  aPhaseOffsetSlider;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>  aTypeBox;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> SliderHelper(Slider& aSlider,int xpos,int ypos,const char* param);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KadenzeChorusProcessorEditor)
    
 

    
    
};
