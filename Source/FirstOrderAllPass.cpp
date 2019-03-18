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

void FirstOrderAllPass::prepare(const int& numChannels, const int& filterNum)
{
	for (auto channel = 0; channel < numChannels; ++channel)
	{
		mXh.push_back(0.f);
	}
	mFilterNum = filterNum;
}

void FirstOrderAllPass::process(dsp::ProcessContextReplacing<float>& context)
{
	dsp::AudioBlock<float> inputBlock = context.getInputBlock();
	dsp::AudioBlock<float> outputBlock = context.getOutputBlock();

	float newC = mState.state.getChildWithName(Identifier("APCoeffs")).getChild(mFilterNum).getProperty(Identifier("coeff"));
	float xh = 0.f;
	float c = 0.f;

	for (auto channel = 0; channel < inputBlock.getNumChannels(); ++channel)
	{
		const float* input = inputBlock.getChannelPointer(channel);
		float* output = outputBlock.getChannelPointer(channel);

		for (auto sample = 0; sample < inputBlock.getNumSamples(); ++sample)
		{
			c = mOldC + ((newC - mOldC) / inputBlock.getNumSamples()) * sample;

			xh = input[sample] - c * mXh[channel];
			output[sample] = c * xh + mXh[channel];
			mXh[channel] = xh;
		}
		mOldC = newC;
	}
}
