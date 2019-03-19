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
	ValueTree APCoeffs("APCoeffs");
	for (auto i = 0; i < mAPFilters.size(); ++i)
	{
		ValueTree APCoeff (static_cast<String> (i));

		float coeffVal = 0.f;
		if (i == 0 || i == 1 || i == 8 || i == 9)
			coeffVal = -0.89f;
		else
			coeffVal = -0.49f;

		APCoeff.setProperty(Identifier("coeff"), coeffVal, nullptr);
		APCoeffs.addChild(APCoeff, -1, nullptr);
	}
	mState.state.addChild(APCoeffs, -1, nullptr);
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
		double coeff = mState.state.getChildWithName(Identifier("APCoeffs")).getChild(i).getProperty(Identifier("coeff"));
		mAPFilters[i].prepare(spec.numChannels, i);
		mAPFilters[i].updateCoefficients(coeff);
	}
	mSampleRate = spec.sampleRate;
	mPhase = 0.f;
	mDryBuffer.setSize(spec.numChannels, spec.maximumBlockSize);
}

void Phaser::process(AudioBuffer<float>& buffer)
{
	float W = .5f;
	float G = 1 - W;
	float s = 50.f;

	// LFO
	float f_lfo = 0.069f * exp(0.04f * s);
	mPhase = mPhase + (static_cast<float>(buffer.getNumSamples()) / mSampleRate) * f_lfo * 2.f * M_PI;
	float newCoeff = mDC + mA * abs(sin(2.f * M_PI * f_lfo + mPhase));

	for (auto i = 2; i < 8; ++i)
	{
		mAPFilters[i].updateCoefficients(newCoeff);
	}

	mOldCoeff = newCoeff;

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
