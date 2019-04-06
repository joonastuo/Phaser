/*
  ==============================================================================

    DCBlocker.cpp
    Created: 17 Mar 2019 10:47:42pm
    Author:  Joonas

  ==============================================================================
*/

#include "DCBlocker.h"

DCBlocker::DCBlocker()
{
	// Empty constructor
}

DCBlocker::~DCBlocker()
{
	// Empty destructor
}

void DCBlocker::prepare(const int& numChannels)
{

}

void DCBlocker::process(const dsp::ProcessContextReplacing<float>& context)
{
	dsp::AudioBlock<float> inputBlock = context.getInputBlock();
	dsp::AudioBlock<float> outputBlock = context.getOutputBlock();
	float inputValue = 0.f;

	for (auto channel = 0; channel < 2; ++channel)
	{
		float* input = inputBlock.getChannelPointer(channel);
		float* output = outputBlock.getChannelPointer(channel);

		for (auto sample = 0; sample < inputBlock.getNumSamples(); ++sample)
		{
			inputValue = input[sample];
			output[sample] = b0 * inputValue + mXh[channel][1];
			mXh[channel][1] = mXh[channel][0];
			mXh[channel][0] = b2 * inputValue + a2 * input[sample];
		}
	}
}
