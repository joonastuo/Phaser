/*
  ==============================================================================

    FirstOrderAllPass.cpp
    Created: 17 Mar 2019 10:47:21pm
    Author:  Joonas

  ==============================================================================
*/

#include "FirstOrderAllPass.h"

//==============================================================================
FirstOrderAllPass::FirstOrderAllPass()
{
	// Empty constructor
}

//==============================================================================
FirstOrderAllPass::~FirstOrderAllPass()
{
	// Empty destructor
}

//==============================================================================
void FirstOrderAllPass::prepare(const int& numChannels, const int& filterNum)
{
	mFilterNum = filterNum;
}

//==============================================================================
void FirstOrderAllPass::processBlock(const dsp::ProcessContextReplacing<float>& context)
{
	dsp::AudioBlock<float> inputBlock = context.getInputBlock();
	dsp::AudioBlock<float> outputBlock = context.getOutputBlock();

	for (auto channel = 0; channel < 2; ++channel)
	{
		const float* input = inputBlock.getChannelPointer(channel);
		float* output = outputBlock.getChannelPointer(channel);

		for (auto sample = 0; sample < inputBlock.getNumSamples(); ++sample)
		{
			// Transposed Direct Form II
			float inputSample = input[sample];
			output[sample] = mC * inputSample + mXh[channel];
			mXh[channel] = inputSample - mC * output[sample];
		}
	}
}

//==============================================================================
float FirstOrderAllPass::processSample(const float& input, const int& channel)
{
	// Transposed Direct Form II
	float output = mC * input + mXh[channel];
	mXh[channel] = input - mC * output;
	return output;
}

//==============================================================================
void FirstOrderAllPass::updateCoefficients(const float& c)
{
	mC = c;
}
