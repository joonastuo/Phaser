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
	int windowWidth = 2 * mWindowMarginWidth + 3 * jmax(mSliderWidth, mLabelWidht) + mSpaceBetweenW;
	int windowHeight = 2 * mWindowMarginHeight + (mLabelHeight + mSliderHeight) + mSpaceBetweenH;
	setSize(windowWidth, windowHeight);
	initialiseGUI();
}

PhaserAudioProcessorEditor::~PhaserAudioProcessorEditor()
{
	// Empty destructor
}

//==============================================================================
void PhaserAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
	Colour backgroundColour = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    g.fillAll (backgroundColour);
	auto area = getLocalBounds().reduced(mWindowMarginWidth, mWindowMarginHeight);
	float width = area.getWidth();
	float height = area.getHeight();

	g.setColour(backgroundColour.darker(.8));
	g.drawRoundedRectangle(mWindowMarginWidth, mWindowMarginHeight, width, 40, 10.f, 3.f);
	g.setColour(Colours::white);
	g.setFont(Font("Pacifico", 40.f, Font::plain));
	g.drawFittedText("Phaser", getWidth() / 2 - 50.f, mWindowMarginHeight, 100.f, 40.f, Justification::centred, 1);
	//g.drawLine(24.f, 30.f, 116.f, 30.f, 2.f);
	//g.drawLine(148.f, 30.f, 270.f, 30.f, 2.f);

	//g.drawLine(24.f, 40.f, 112.f, 40.f, 2.f);
	//g.drawLine(190.f, 40.f, 270.f, 40.f, 2.f);

	//g.drawLine(24.f, 50.f, 114.f, 50.f, 2.f);
	//g.drawLine(190.f, 50.f, 270.f, 50.f, 2.f);

	//g.drawRoundedRectangle(20, 50, width, 120, 10.f, 2.f);
}

void PhaserAudioProcessorEditor::resized()
{
	// Mix ===============================
	FlexBox mixBox;
	mixBox.alignContent = FlexBox::AlignContent::center;
	mixBox.justifyContent = FlexBox::JustifyContent::center;
	mixBox.flexDirection = FlexBox::Direction::column;
	mixBox.items.addArray(
		{
			FlexItem(mMixLabel) .withWidth(mLabelWidht) .withHeight(mLabelHeight),
			FlexItem(mMixSlider).withWidth(mSliderWidth).withHeight(mSliderHeight)
		});

	// Speed
	FlexBox speedBox;
	speedBox.alignContent = FlexBox::AlignContent::center;
	speedBox.justifyContent = FlexBox::JustifyContent::center;
	speedBox.flexDirection = FlexBox::Direction::column;
	speedBox.items.addArray(
		{
			FlexItem(mSpeedLabel) .withWidth(mLabelWidht) .withHeight(mLabelHeight),
			FlexItem(mSpeedSlider).withWidth(mSliderWidth).withHeight(mSliderHeight)
		});

	// Feedback
	FlexBox fbBox;
	fbBox.alignContent = FlexBox::AlignContent::center;
	fbBox.justifyContent = FlexBox::JustifyContent::center;
	fbBox.flexDirection = FlexBox::Direction::column;
	fbBox.items.addArray(
		{
			FlexItem(mFeedbackLabel) .withWidth(mLabelWidht) .withHeight(mLabelHeight),
			FlexItem(mFeedbackSlider).withWidth(mSliderWidth).withHeight(mSliderHeight)
		});

	// MASTER ======================
	int masterItemWidth = jmax(mSliderWidth, mLabelWidht);
	int masterItemHeight = mSliderHeight + mLabelHeight;
	int itemSpaceW = mSpaceBetweenW / 3.f;

	FlexBox masterBox;
	masterBox.alignContent = FlexBox::AlignContent::spaceBetween;
	masterBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	masterBox.flexDirection = FlexBox::Direction::row;
	masterBox.items.addArray(
		{
			FlexItem(mixBox).withWidth(masterItemWidth + itemSpaceW).withHeight(masterItemHeight + mSpaceBetweenH),
			FlexItem(speedBox).withWidth(masterItemWidth + itemSpaceW).withHeight(masterItemHeight + mSpaceBetweenH),
			FlexItem(fbBox).withWidth(masterItemWidth + itemSpaceW).withHeight(masterItemHeight + mSpaceBetweenH),
		});

	auto area = getLocalBounds();
	area.removeFromTop(40.f);
	area.removeFromBottom(20.f);
	area = area.reduced(mWindowMarginWidth, mWindowMarginHeight);
	masterBox.performLayout(area.toFloat());
}

void PhaserAudioProcessorEditor::initialiseGUI()
{
	// SPEED =============================================
	mSpeedLabel.setText("Speed", dontSendNotification);
	mSpeedLabel.setSize(mLabelWidht, mLabelHeight);
	mSpeedLabel.setJustificationType(Justification::centred);
	mSpeedLabel.setFont(mLabelFont);
	addAndMakeVisible(mSpeedLabel);

	mSpeedSlider.setSliderStyle(mSliderStyle);
	mSpeedSlider.setSize(mSliderWidth, mSliderHeight);
	mSpeedSlider.setTextBoxStyle(Slider::TextBoxBelow, true, mTextBoxWidth, mTextBoxHeight);
	addAndMakeVisible(mSpeedSlider);
	mSpeedSliderAttachment.reset(new SliderAttachment(mState, IDs::speed, mSpeedSlider));

	// WETNESS ===========================================
	mMixLabel.setText("Wet", dontSendNotification);
	mMixLabel.setSize(mLabelWidht, mLabelHeight);
	mMixLabel.setJustificationType(Justification::centred);
	mMixLabel.setFont(mLabelFont);
	addAndMakeVisible(mMixLabel);

	mMixSlider.setSliderStyle(mSliderStyle);
	mMixSlider.setSize(mSliderWidth, mSliderHeight);
	mMixSlider.setTextBoxStyle(Slider::TextBoxBelow, true, mTextBoxWidth, mTextBoxHeight);
	addAndMakeVisible(mMixSlider);
	mWetSliderAttachment.reset(new SliderAttachment(mState, IDs::wetness, mMixSlider));

	// FEEDBACK ==========================================
	mFeedbackLabel.setText("Feedback", dontSendNotification);
	mFeedbackLabel.setSize(mLabelWidht, mLabelHeight);
	mFeedbackLabel.setJustificationType(Justification::centred);
	mFeedbackLabel.setFont(mLabelFont);
	addAndMakeVisible(mFeedbackLabel);

	mFeedbackSlider.setSliderStyle(mSliderStyle);
	mFeedbackSlider.setSize(mSliderWidth, mSliderHeight);
	mFeedbackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, mTextBoxWidth, mTextBoxHeight);
	addAndMakeVisible(mFeedbackSlider);
	mFeedbackSliderAttachment.reset(new SliderAttachment(mState, IDs::feedback, mFeedbackSlider));
}
