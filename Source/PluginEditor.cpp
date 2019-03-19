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
    setSize (200, 200);

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
	mSpeedLabel.setBounds(area.removeFromTop(20));
	area.removeFromTop(20);
	mSpeedSlider.setBounds(area.removeFromTop(100));
}

void PhaserAudioProcessorEditor::resized()
{
	// Empty resize method
}
