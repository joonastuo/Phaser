/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PhaserAudioProcessorEditor::PhaserAudioProcessorEditor (PhaserAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), mState(p.getState())
{
    setSize (400, 200);

	// SPEED =============================================
	mSpeedLabel.setText("Speed", dontSendNotification);
	mSpeedLabel.setSize(150, 20);
	mSpeedLabel.setJustificationType(Justification::centred);
	mSpeedLabel.setFont(20);
	addAndMakeVisible(mSpeedLabel);

	mSpeedSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	mSpeedSlider.setSize(100, 100);
	mSpeedSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	addAndMakeVisible(mSpeedSlider);
	mSpeedSliderAttachment.reset(new SliderAttachment(mState, "speed", mSpeedSlider));

	// WETNESS ===========================================
	mWetLabel.setText("Wet", dontSendNotification);
	mWetLabel.setSize(150, 20);
	mWetLabel.setJustificationType(Justification::centred);
	mWetLabel.setFont(20);
	addAndMakeVisible(mWetLabel);

	mWetSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	mWetSlider.setSize(100, 100);
	mWetSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	addAndMakeVisible(mWetSlider);
	mWetSliderAttachment.reset(new SliderAttachment(mState, "wetness", mWetSlider));
}

PhaserAudioProcessorEditor::~PhaserAudioProcessorEditor()
{
}

//==============================================================================
void PhaserAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

	auto area = getLocalBounds().reduced(10, 10);
	auto leftSide = area.removeFromLeft(area.getWidth() / 2);

	mSpeedLabel.setBounds(area.removeFromTop(20));
	area.removeFromTop(20);
	mSpeedSlider.setBounds(area.removeFromTop(100));

	mWetLabel.setBounds(leftSide.removeFromTop(20));
	leftSide.removeFromTop(20);
	mWetSlider.setBounds(leftSide.removeFromTop(100));
}

void PhaserAudioProcessorEditor::resized()
{
	// Empty resize method
}
