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
	for (auto channel = 0; channel < numChannels; ++channel)
	{
		mXh.push_back(std::vector<float> {0.f, 0.f});
	}
}

void DCBlocker::process(AudioBuffer<float>& buffer)
{
	// Filter coefficients
	const float p = 0.992f;
	const float b0 = (1.f + p) / 2.f;
	const float b2 = -1.f * b0;
	const float a2 = p;

	for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		const float* input = buffer.getReadPointer(channel);
		float* output = buffer.getWritePointer(channel);

		for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			output[sample] = b0 * input[sample] + mXh[channel][1];
			mXh[channel][1] = mXh[channel][0];
			mXh[channel][0] = b2 * input[sample] + a2 * output[sample];
		}
	}
}
