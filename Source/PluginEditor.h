/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Helper/Identifiers.h"
#include "LookAndFeel/KnobLookAndFeel.h"

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
	void initialiseGUI();

private:
	void drawTitle(Graphics& g, Rectangle<float> area);
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PhaserAudioProcessor& processor;
	AudioProcessorValueTreeState& mState;

	// Look and feel =====================
	// Component sizes
	const int mSliderWidth = 80;
	const int mSliderHeight = 80;
	const int mTextBoxWidth = mSliderWidth;
	const int mTextBoxHeight = 14;
	const int mLabelHeight = 16;
	const int mLabelWidht = mSliderWidth;
	const int mWindowMarginWidth = 20;
	const int mWindowMarginHeight = 20;
	const int mSpaceBetweenW = 20;
	const int mSpaceBetweenH = 20;
	const int mTitleHeight = 40;
	// Label font
	Font mLabelFont = Font(mLabelHeight, Font::plain);
	String mTitleText = "Phaser";
	Font mTitleFont = Font(30.f, Font::plain);
	Slider::SliderStyle mSliderStyle = Slider::SliderStyle::Rotary;
	KnobLookAndFeel mKnobLookAndFeel;

	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

	// Speed of phaser LFO
	Label mSpeedLabel;
	Slider mSpeedSlider;
	std::unique_ptr<SliderAttachment> mSpeedSliderAttachment;

	// Wetness of effect
	Label mMixLabel;
	Slider mMixSlider;
	std::unique_ptr<SliderAttachment> mWetSliderAttachment;

	// Feedback
	Label mFeedbackLabel;
	Slider mFeedbackSlider;
	std::unique_ptr<SliderAttachment> mFeedbackSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaserAudioProcessorEditor)
};
