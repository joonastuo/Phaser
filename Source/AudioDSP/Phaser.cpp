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
	mSampleRate = static_cast<float>(spec.sampleRate);
	mDryBuffer.setSize(spec.numChannels, spec.maximumBlockSize);

	// Prepare DCBlocker
	mDCBlocker.prepare(spec.numChannels);

	// Prepare LFOs
	int lfoWaveform = getLFOWaveform() ? Waveforms::tri : Waveforms::rectSine;

	for (auto& lfo : mLFOs)
	{
		lfo.prepare(spec);
		lfo.setUnipolar(true);
		lfo.setWaveform(lfoWaveform);
		lfo.setFreq(getLfoFreq());
	}

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
			mAPFilters[i].prepare(spec.numChannels);
			mAPFilters[i].updateCoefficients(coeffFirstLast);
		}
		else
		{
			mAPFilters[i].prepare(spec.numChannels);
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

			// Update mod AP filter coefficients
			float coeff = getCoeff(channel);
			for (auto i = 2; i < 8; ++i)
			{
				mAPFilters[i].updateCoefficients(coeff);
			}
			// Process sample trough AP filters
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

bool Phaser::getLFOWaveform()
{
	return *mState.getRawParameterValue(IDs::lfoWaveform);
}

//==============================================================================
float Phaser::getCoeff(const int& channel)
{
	float coeff = 0.f;
	auto& lfo = mLFOs[channel];
	bool  lfoWaveform = getLFOWaveform();
	lfo.setFreq(getLfoFreq());

	// Rectified sine wave
	if (!lfoWaveform)
	{
		lfo.setWaveform(Waveforms::rectSine);
		float fc = mFcMinSin + mSinA * lfo.getValue();
		lfo.advanceSamples(1);
		coeff = calcCoeff(fc);
	}
	// Triangular
	else
	{
		lfo.setWaveform(Waveforms::tri);
		float fc = mFcMinTri + mTriA * lfo.getValue();
		lfo.advanceSamples(1);
		coeff = calcCoeff(fc);
	}
	return coeff;
}

//==============================================================================
float Phaser::calcCoeff(const float & fc)
{
	return static_cast<float>(-1.f * (1.f - tan(2.f * M_PI * fc * ((1.f / mSampleRate) / 2.f))) / (1.f + tan(2.f * M_PI * fc * ((1.f / mSampleRate) / 2.f))));
}
