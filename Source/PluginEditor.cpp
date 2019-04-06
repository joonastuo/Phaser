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

//==============================================================================
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

	auto area = getLocalBounds().reduced(static_cast<int>(mWindowMarginWidth / 2.f), static_cast<int>(mWindowMarginHeight / 2.f));
	// Draw title
	auto titleArea = area.removeFromBottom(mTitleHeight);
	drawTitle(g, titleArea.toFloat());
}

//==============================================================================
void PhaserAudioProcessorEditor::resized()
{
	// Mix ===============================
	FlexBox mixBox;
	mixBox.alignContent = FlexBox::AlignContent::center;
	mixBox.justifyContent = FlexBox::JustifyContent::center;
	mixBox.flexDirection = FlexBox::Direction::column;
	mixBox.items.addArray(
		{
			createItem(mMixLabel, mLabelWidht, mLabelHeight),
			createItem(mMixSlider, mSliderWidth, mSliderHeight)
		});

	// Speed
	FlexBox speedBox;
	speedBox.alignContent = FlexBox::AlignContent::center;
	speedBox.justifyContent = FlexBox::JustifyContent::center;
	speedBox.flexDirection = FlexBox::Direction::column;
	speedBox.items.addArray(
		{
			createItem(mSpeedLabel, mLabelWidht, mLabelHeight),
			createItem(mSpeedSlider, mSliderWidth, mSliderHeight)
		});

	// Feedback
	FlexBox fbBox;
	fbBox.alignContent = FlexBox::AlignContent::center;
	fbBox.justifyContent = FlexBox::JustifyContent::center;
	fbBox.flexDirection = FlexBox::Direction::column;
	fbBox.items.addArray(
		{
			createItem(mFeedbackLabel, mLabelWidht, mLabelHeight),
			createItem(mFeedbackSlider, mSliderWidth, mSliderHeight)
		});

	FlexBox toggleBox;
	toggleBox.alignContent = FlexBox::AlignContent::center;
	toggleBox.justifyContent = FlexBox::JustifyContent::center;
	toggleBox.flexDirection = FlexBox::Direction::column;
	toggleBox.items.addArray(
		{
			createItem(mLFOWaveformButton, 20, 40)
		});

	FlexBox waveformBox;
	waveformBox.alignContent = FlexBox::AlignContent::center;
	waveformBox.justifyContent = FlexBox::JustifyContent::center;
	waveformBox.flexDirection = FlexBox::Direction::column;
	waveformBox.items.addArray(
		{
			createItem(mLFOWaveformLabel, mLabelWidht, mLabelHeight),
			createItem(toggleBox, mSliderWidth, 40)
		});

	FlexBox firstRowBox;
	firstRowBox.alignContent = FlexBox::AlignContent::center;
	firstRowBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	firstRowBox.flexDirection = FlexBox::Direction::row;
	firstRowBox.items.addArray(
		{
			createItem(speedBox, (jmax(mSliderWidth, mLabelWidht)), (mLabelHeight + mSliderHeight)),
			createItem(fbBox, (jmax(mSliderWidth, mLabelWidht)), (mLabelHeight + mSliderHeight))
		});
	
	FlexBox secondRowBox;
	secondRowBox.alignContent = FlexBox::AlignContent::center;
	secondRowBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	secondRowBox.flexDirection = FlexBox::Direction::row;
	secondRowBox.items.addArray(
		{
			createItem(mixBox, jmax(mSliderWidth, mLabelWidht), mLabelHeight + mSliderHeight),
			createItem(waveformBox, jmax(mSliderWidth, mLabelWidht),mLabelHeight + mSliderHeight)
		});

	// MASTER ======================
	int masterItemWidth = (2 * jmax(mSliderWidth, mLabelWidht)) + mSpaceBetweenW;
	int masterItemHeight = static_cast<int>(mSliderHeight + mLabelHeight + mSpaceBetweenH / 2.f);

	FlexBox masterBox;
	masterBox.alignContent = FlexBox::AlignContent::spaceBetween;
	masterBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	masterBox.flexDirection = FlexBox::Direction::column;
	masterBox.items.addArray(
		{
			createItem(firstRowBox, masterItemWidth, masterItemHeight),
			createItem(secondRowBox, masterItemWidth, masterItemHeight)
		});

	auto area = getLocalBounds();
	area = area.reduced(mWindowMarginWidth, mWindowMarginHeight);
	area.removeFromBottom(mTitleHeight + 10);
	masterBox.performLayout(area.toFloat());
}

//==============================================================================
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
	mSpeedSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
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
	mMixSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
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
	mFeedbackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mFeedbackSlider.setLookAndFeel(&mKnobLookAndFeel);
	mFeedbackSlider.setTextValueSuffix(" %");
	addAndMakeVisible(mFeedbackSlider);
	mFeedbackSliderAttachment.reset(new SliderAttachment(mState, IDs::feedback, mFeedbackSlider));

	// LFO WAVEFORM =======================================
	// Label
	mLFOWaveformLabel.setText("Intense", dontSendNotification);
	mLFOWaveformLabel.setSize(mLabelWidht, mLabelHeight);
	mLFOWaveformLabel.setJustificationType(Justification::centred);
	mLFOWaveformLabel.setFont(mLabelFont);
	addAndMakeVisible(mLFOWaveformLabel);
	// Button
	mLFOWaveformButton.setSize(40, 40);
	addAndMakeVisible(mLFOWaveformButton);
	mLFOWaveformButtonAttachment.reset(new ButtonAttachment(mState, IDs::lfoWaveform, mLFOWaveformButton));
}

//==============================================================================
FlexItem PhaserAudioProcessorEditor::createItem(Component & c,const int & width,const int & height)
{
	return FlexItem(c).withWidth(static_cast<float>(width)).withHeight(static_cast<float>(height));
}

//==============================================================================
FlexItem PhaserAudioProcessorEditor::createItem(FlexBox & fb, const int & width, const int & height)
{
	return FlexItem(fb).withWidth(static_cast<float>(width)).withHeight(static_cast<float>(height));
}

//==============================================================================
void PhaserAudioProcessorEditor::drawTitle(Graphics & g, Rectangle<float> area)
{
	Colour bgColour = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
	g.setColour(bgColour.darker(.8f));

	Path textPath;
	GlyphArrangement glyphs;
	float x = area.getX(); ;
	float y = area.getY();
	float w = area.getWidth();
	float h = area.getHeight();

	g.setColour(Colours::white);

	glyphs.addFittedText(mTitleFont, mTitleText, x, y, w, h, Justification::centred, 1);
	glyphs.createPath(textPath);

	g.setColour(bgColour.darker(.4f));
	auto textBounds = textPath.getBounds().expanded(10.f, 10.f);
	g.fillRoundedRectangle(textBounds.toFloat(), 18.f);

	g.setColour(juce::Colours::white);
	juce::PathStrokeType strokeType(2.5f);
	g.fillPath(textPath);
}

