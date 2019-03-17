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
		mXm1.push_back(0.f);
		mXm2.push_back(0.f);
		mYm1.push_back(0.f);
		mYm2.push_back(0.f);
	}
}

void DCBlocker::process(AudioBuffer<float>& buffer)
{
	for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		const float* input = buffer.getReadPointer(channel);
		float* output = buffer.getWritePointer(channel);

		for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			output[sample] = mA * input[sample] - mA * mXm2[channel] + mP * mYm2[channel];
			mXm2[channel] = mXm1[channel];
			mYm2[channel] = mYm1[channel];
			mXm1[channel] = input[sample];
			mYm1[channel] = output[sample];
		}
	}
}
