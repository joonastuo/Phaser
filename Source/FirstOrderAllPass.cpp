/*
  ==============================================================================

    FirstOrderAllPass.cpp
    Created: 17 Mar 2019 10:47:21pm
    Author:  Joonas

  ==============================================================================
*/

#include "FirstOrderAllPass.h"

FirstOrderAllPass::FirstOrderAllPass(AudioProcessorValueTreeState& state)
	: mState(state)
{
	// Empty constructor
}

FirstOrderAllPass::~FirstOrderAllPass()
{
	// Empty destructor
}

void FirstOrderAllPass::prepare(const int& numChannels)
{
	for (auto channel = 0; channel < numChannels; ++channel)
	{
		mPreviousInput.push_back(0.f);
		mPreviousOutput.push_back(0.f);
	}
}

void FirstOrderAllPass::process(AudioBuffer<float>& buffer)
{
	float a1 = -0.89;

	for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		const float* input = buffer.getReadPointer(channel);
		float* output = buffer.getWritePointer(channel);

		for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			output[sample] = a1 * input[sample] + mPreviousInput[channel] - a1 * mPreviousOutput[channel];
			mPreviousInput[channel] = input[sample];
			mPreviousOutput[channel] = output[sample];
		}
	}
}
