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

//==============================================================================
Phaser::Phaser(AudioProcessorValueTreeState& state)
	: mState(state)
{
	// Empty constructor
}

//==============================================================================
Phaser::~Phaser()
{
	// Empty destructor
}

//==============================================================================
void Phaser::prepare(dsp::ProcessSpec& spec)
{
	// Prepare class members
	mSampleRate = spec.sampleRate;
	mDryBuffer.setSize(spec.numChannels, spec.maximumBlockSize);

	// Prepare DCBlocker
	mDCBlocker.prepare(spec.numChannels);

	// Prepare LFO
	mLFO.prepare(spec);
	mLFO.setUnipolar(true);
	mLFO.setWaveform(3);
	float lfoFreq = getLfoFreq();
	mLFO.setFreq(lfoFreq);

	// Prepare AP filters
	// Coefficient for A1, A2, A9 and A10
	float coeffFirstLast = calcCoeff(mFcFirstLast);
	// Coefficient for modulated filters
	float fc = mFcMinTri + mTriA * mLFO.getValue();
	float coeffMod = calcCoeff(fc);

	for (auto i = 0; i < mAPFilters.size(); ++i)
	{
		if (i == 0 || i == 1 || i == 8 || i == 9)
		{
			mAPFilters[i].prepare(spec.numChannels, i);
			mAPFilters[i].updateCoefficients(coeffFirstLast);
		}
		else
		{
			mAPFilters[i].prepare(spec.numChannels, i);
			mAPFilters[i].updateCoefficients(coeffMod);
		}

	}
}

//==============================================================================
void Phaser::process(AudioBuffer<float>& buffer)
{
	// Gains
	float W  = getWetness();
	float G  = 1 - W;
	float FB = getFeedback();
	// LFO
	float lfoFreq = getLfoFreq();
	mLFO.setFreq(lfoFreq);

	float fc = mFcMinTri + mTriA * mLFO.getValue();
	float newCoeff = calcCoeff(fc);
	mLFO.advanceBlock();

	for (auto i = 2; i < 8; ++i)
	{
		mAPFilters[i].updateCoefficients(newCoeff);
	}

	// DC Blocker ============================
	dsp::AudioBlock<float> block(buffer);
	mDCBlocker.process(dsp::ProcessContextReplacing<float> (block));

	// Copy current buffer to dry buffer =====
	for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		mDryBuffer.copyFrom(channel, 0, buffer.getReadPointer(channel), buffer.getNumSamples());
	}

	// AP Filters ============================
	for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		const float* input = buffer.getReadPointer(channel);
		float* output = buffer.getWritePointer(channel);

		for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			float outputSample = 0.f;
			float inputSample = input[sample] + mFeedback[channel] * FB;

			for (auto i = 0; i < mAPFilters.size(); ++i)
			{
				outputSample = mAPFilters[i].processSample(inputSample, channel);
				inputSample = outputSample;
			}
			output[sample] = outputSample;
			mFeedback[channel] = outputSample;
		}
	}

	// Dry / wet ============================
	buffer.applyGain(W);
	for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		buffer.addFromWithRamp(channel, 0, mDryBuffer.getReadPointer(channel), mDryBuffer.getNumSamples(), G, G);
	}
}

//==============================================================================
float Phaser::getWetness()
{
	return *mState.getRawParameterValue(IDs::wetness);
}

//==============================================================================
float Phaser::getFeedback()
{
	return *mState.getRawParameterValue(IDs::feedback);
}

//==============================================================================
float Phaser::getLfoFreq()
{
	float s = *mState.getRawParameterValue(IDs::speed);
	return 0.069f * exp(0.04f * s);
}

//==============================================================================
float Phaser::calcCoeff(const float & fc)
{
	return (tan(M_PI * fc / mSampleRate) - 1) / (tan(M_PI * fc / mSampleRate) + 1);
}
