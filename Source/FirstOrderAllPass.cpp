/*
  ==============================================================================

    FirstOrderAllPass.cpp
    Created: 17 Mar 2019 10:47:21pm
    Author:  Joonas

  ==============================================================================
*/

#include "FirstOrderAllPass.h"

FirstOrderAllPass::FirstOrderAllPass()
{
	// Empty constructor
}

FirstOrderAllPass::~FirstOrderAllPass()
{
	// Empty destructor
}

void FirstOrderAllPass::prepare(const int& numChannels, const int& filterNum)
{
	mFilterNum = filterNum;
}

void FirstOrderAllPass::process(dsp::ProcessContextReplacing<float>& context)
{
	dsp::AudioBlock<float> inputBlock = context.getInputBlock();

	float xh = 0.f;

	for (auto channel = 0; channel < 2; ++channel)
	{
		float* input = inputBlock.getChannelPointer(channel);

		for (auto sample = 0; sample < inputBlock.getNumSamples(); ++sample)
		{
			xh = input[sample] - mC * mXh[channel];
			input[sample] = mC * xh + mXh[channel];
			mXh[channel] = xh;
		}
	}
}

float FirstOrderAllPass::processSample(const float & input, const int & channel)
{
	//float output = 0.f;
	//float xh = input - mC * mXh[channel];
	//output = mC * xh + mXh[channel];
	//mXh[channel] = xh;
	//return output;

	float output = mC * input + mXh[channel];
	mXh[channel] = input - mC * output;
	return output;
}

void FirstOrderAllPass::updateCoefficients(const float& c)
{
	mC = c;
}
