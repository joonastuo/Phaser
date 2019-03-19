/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PhaserAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    PhaserAudioProcessorEditor (PhaserAudioProcessor&);
    ~PhaserAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PhaserAudioProcessor& processor;
	AudioProcessorValueTreeState& mState;

	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

	// Speed of phaser LFO
	Label mSpeedLabel;
	Slider mSpeedSlider;
	std::unique_ptr<SliderAttachment> mSpeedSliderAttachment;

	// Wetness of effect
	Label mWetLabel;
	Slider mWetSlider;
	std::unique_ptr<SliderAttachment> mWetSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaserAudioProcessorEditor)
};
