/*
  ==============================================================================

    Phaser.cpp
    Created: 18 Mar 2019 3:36:31pm
    Author:  Joonas

  ==============================================================================
*/

#include "Phaser.h"
#define _USE_MATH_DEFINES
#include <math.h>

Phaser::Phaser(AudioProcessorValueTreeState& state)
	: mState(state)
{
	// Empty constructor
}

Phaser::~Phaser()
{
	// Empty destructor
}

void Phaser::prepare(dsp::ProcessSpec& spec)
{
	mDCBlocker.prepare(spec.numChannels);
	for (auto i = 0; i < mAPFilters.size(); ++i)
	{
		float coeff = 0.0f;
		if (i == 0 || i == 1 || i == 8 || i == 9)
			coeff = -0.89;
		else
			coeff = -0.49;

		mAPFilters[i].prepare(spec.numChannels, i);
		mAPFilters[i].updateCoefficients(coeff);
	}
	mSampleRate = spec.sampleRate;
	mPhase = 0.f;
	mDryBuffer.setSize(spec.numChannels, spec.maximumBlockSize);
	mLFO.prepare(spec);
	mLFO.setUnipolar(true);
	mLFO.setWaveform(3);
	mOlds = *mState.getRawParameterValue("speed");
	float lfoFreq = 0.069f * exp(0.04f * mOlds);
	mLFO.setFreq(lfoFreq);
}

void Phaser::process(AudioBuffer<float>& buffer)
{
	float wetnessPercentage = *mState.getRawParameterValue("wetness");
	float W = .5f * (wetnessPercentage / 100);
	float G = 1 - W;
	float s = *mState.getRawParameterValue("speed");

	// LFO
	if (s != mOlds)
	{
		float lfoFreq = 0.069f * exp(0.04f * s);
		mLFO.setFreq(lfoFreq);
	}
	mOlds = s;
	float newCoeff = mDC + mA * mLFO.getValue();
	mLFO.advanceBlock();

	for (auto i = 2; i < 8; ++i)
	{
		mAPFilters[i].updateCoefficients(newCoeff);
	}

	// DC Blocker
	dsp::AudioBlock<float> block(buffer);
	mDCBlocker.process(dsp::ProcessContextReplacing<float> (block));

	// Copy current buffer to dry buffer
	for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		mDryBuffer.copyFrom(channel, 0, buffer.getReadPointer(channel), buffer.getNumSamples());
	}

	// Allpass filters
	for (auto i = 0; i < mAPFilters.size(); ++i)
	{
		mAPFilters[i].process(dsp::ProcessContextReplacing<float> (block));
	}

	// Apply W
	buffer.applyGain(W);

	for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		buffer.addFromWithRamp(channel, 0, mDryBuffer.getReadPointer(channel), mDryBuffer.getNumSamples(), G, G);
	}
}
