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
	: mState(state),
	  mAPFilters({{mState, mState, mState, mState, mState, 
				   mState, mState, mState, mState, mState
				}})
{
	ValueTree APCoeffs("APCoeffs");
	for (auto i = 0; i < mAPFilters.size(); ++i)
	{
		ValueTree APCoeff (static_cast<String> (i));
		float coeffVal = -0.89f;
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
		mAPFilters[i].prepare(spec.numChannels, i);
	}
	mSampleRate = spec.sampleRate;
	mPhase = 0.f;
}

void Phaser::process(AudioBuffer<float>& buffer)
{
	float W = .5f;
	float G = 1 - W;
	float s = 50.f;

	float f_lfo = 0.069 * exp(0.04f * s);
	// float mPhase = mPhase + ((buffer.getNumSamples() / mSampleRate) / f_lfo) * 2 * M_PI;
	float A = 0.77f + 0.49f;
	float DC = -0.49;
	float newCoeff = DC + A * abs(sin(2 * M_PI * f_lfo + mPhase));

	for (auto i = 2; i < 8; ++i)
	{
		
		mState.state.getChildWithName(Identifier("APCoeffs")).getChild(i).setProperty(Identifier("coeff"), newCoeff, nullptr);
	}

	// DC Blocker
	dsp::AudioBlock<float> block(buffer);
	//mDCBlocker.process(dsp::ProcessContextReplacing<float> (block));

	AudioBuffer<float> wetBuffer;
	wetBuffer.setSize(buffer.getNumChannels(), buffer.getNumSamples());
	for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		wetBuffer.copyFrom(channel, 0, buffer.getReadPointer(channel), buffer.getNumSamples());
	}
	dsp::AudioBlock<float> wetBlock(wetBuffer);
	// Allpass filters
	for (auto i = 0; i < mAPFilters.size(); ++i)
	{
		mAPFilters[i].process(dsp::ProcessContextReplacing<float> (wetBlock));
	}

	for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		buffer.copyFromWithRamp(channel, 0, buffer.getReadPointer(channel), buffer.getNumSamples(), G, G);
		buffer.addFromWithRamp(channel, 0, wetBuffer.getReadPointer(channel), buffer.getNumSamples(), W, W);
	}
}
