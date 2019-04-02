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
	int windowWidth  = 2 * mWindowMarginWidth  + 2 * jmax(mSliderWidth, mLabelWidht) + mSpaceBetweenW;
	int windowHeight = 2 * mWindowMarginHeight + 2 * (mLabelHeight + mSliderHeight)  + mSpaceBetweenH + mTitleHeight;
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

	auto area = getLocalBounds().reduced(mWindowMarginWidth / 2.f, mWindowMarginHeight / 2.f);
	// Draw title
	auto titleArea = area.removeFromBottom(mTitleHeight);
	drawTitle(g, titleArea.toFloat());
	// Draw parameter area
	//area.removeFromBottom(10.f);
	//g.setColour(backgroundColour.darker(.8));
	//g.drawRoundedRectangle(area.toFloat(), 10.f, 3.f);
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

	FlexBox firstRowBox;
	firstRowBox.alignContent = FlexBox::AlignContent::center;
	firstRowBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	firstRowBox.flexDirection = FlexBox::Direction::row;
	firstRowBox.items.addArray(
		{
			FlexItem(speedBox) .withWidth(jmax(mSliderWidth, mLabelWidht)).withHeight(mLabelHeight + mSliderHeight),
			FlexItem(fbBox)	   .withWidth(jmax(mSliderWidth, mLabelWidht)).withHeight(mLabelHeight + mSliderHeight)
		});

	// MASTER ======================
	float masterItemWidth = (2 * jmax(mSliderWidth, mLabelWidht)) + mSpaceBetweenW;
	float masterItemHeight = mSliderHeight + mLabelHeight + mSpaceBetweenH / 2.f;

	FlexBox masterBox;
	masterBox.alignContent = FlexBox::AlignContent::spaceBetween;
	masterBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	masterBox.flexDirection = FlexBox::Direction::column;
	masterBox.items.addArray(
		{
			FlexItem(firstRowBox).withWidth(masterItemWidth).withHeight(masterItemHeight),
			FlexItem(mixBox)	 .withWidth(masterItemWidth).withHeight(masterItemHeight)
		});

	auto area = getLocalBounds();
	area = area.reduced(mWindowMarginWidth, mWindowMarginHeight);
	area.removeFromBottom(mTitleHeight + 10.f);
	masterBox.performLayout(area.toFloat());
}

void PhaserAudioProcessorEditor::initialiseGUI()
{
	// SPEED =============================================
	mSpeedLabel.setText("LFO Freq", dontSendNotification);
	mSpeedLabel.setSize(mLabelWidht, mLabelHeight);
	mSpeedLabel.setJustificationType(Justification::centred);
	mSpeedLabel.setFont(mLabelFont);
	addAndMakeVisible(mSpeedLabel);

	mSpeedSlider.setSliderStyle(mSliderStyle);
	mSpeedSlider.setSize(mSliderWidth, mSliderHeight);
	mSpeedSlider.setTextBoxStyle(Slider::TextBoxBelow, true, mTextBoxWidth, mTextBoxHeight);
	mSpeedSlider.setLookAndFeel(&mKnobLookAndFeel);
	mSpeedSlider.setTextValueSuffix(" Hz");
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
	mMixSlider.setLookAndFeel(&mKnobLookAndFeel);
	mMixSlider.setTextValueSuffix(" %");
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
	mFeedbackSlider.setLookAndFeel(&mKnobLookAndFeel);
	mFeedbackSlider.setTextValueSuffix(" %");
	addAndMakeVisible(mFeedbackSlider);
	mFeedbackSliderAttachment.reset(new SliderAttachment(mState, IDs::feedback, mFeedbackSlider));
}

void PhaserAudioProcessorEditor::drawTitle(Graphics & g, Rectangle<float> area)
{
	Colour bgColour = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
	g.setColour(bgColour.darker(.8f));
	//g.drawRoundedRectangle(area.toFloat(), 10.f, 3.f);

	Path textPath;
	GlyphArrangement glyphs;
	float x = area.getX(); ;
	float y = area.getY();
	float w = area.getWidth();
	float h = area.getHeight();

	g.setColour(Colours::white);
	float startX = area.getTopLeft().getX() + 4.f;
	float endX = area.getTopRight().getX() - 4.f;
	//for (auto i = 1; i < 4; ++i)
	//{
	//	g.drawLine(startX, y + 10.f * i, endX, y + 10.f * i, 2.f);
	//}

	glyphs.addFittedText(mTitleFont, mTitleText, x, y, w, h, Justification::centred, 1);
	glyphs.createPath(textPath);

	g.setColour(bgColour.darker(.4));
	auto textBounds = textPath.getBounds().expanded(10.f, 10.f);
	g.fillRoundedRectangle(textBounds.toFloat(), 18.f);

	g.setColour(juce::Colours::white);
	juce::PathStrokeType strokeType(2.5f);
	//g.strokePath(textPath, strokeType);
	//g.setColour(juce::Colours::black);
	g.fillPath(textPath);
}

