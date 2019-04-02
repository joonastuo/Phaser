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
#include "Identifiers.h"

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

	AudioProcessorValueTreeState& mState;
	DCBlocker mDCBlocker;
	std::array<FirstOrderAllPass, 10> mAPFilters;
	LFO mLFO;
	AudioBuffer<float> mDryBuffer;
	float mAPCoeff = -.84f;
	float mSampleRate = 44100.f;
	float mPhase = 0.f;
	float mOlds = 0.f;

	float mA = 0.45;
	float mDC = -0.84;

	float mFeedback[2] = { 0.f };
};