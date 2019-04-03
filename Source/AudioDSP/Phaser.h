/*
  ==============================================================================

    Phaser.h
    Created: 18 Mar 2019 3:36:31pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FirstOrderAllPass.h"
#include "DCBlocker.h"
#include "LFO.h"
#include "../Helper/Identifiers.h"

class Phaser
{
public:
	Phaser(AudioProcessorValueTreeState& state);
	~Phaser();
	void prepare(dsp::ProcessSpec& spec);
	void process(AudioBuffer<float>& buffer);

private:
	float getWetness();
	float getFeedback();
	float getLfoFreq();
	float calcCoeff(const float& fc);

	AudioProcessorValueTreeState& mState;
	DCBlocker mDCBlocker;
	std::array<FirstOrderAllPass, 10> mAPFilters;
	LFO mLFO;
	AudioBuffer<float> mDryBuffer;
	float mAPCoeff = -.84f;
	float mSampleRate = 44100.f;

	float mA = 0.45;
	float mDC = -0.84;

	float mFcFirstLast = 816.075;

	float mFcMinTri = 1217.59f;
	float mFcMaxTri = 5805.80f;
	float mTriA = mFcMaxTri - mFcMinTri;

	float mFcMinSin = 4629.31f;
	float mFcMaxSin = 20236.1f;
	float mSinA = mFcMaxSin - mFcMinSin;

	float mFeedback[2] = { 0.f };
};