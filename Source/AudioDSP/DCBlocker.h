/*
  ==============================================================================

    DCBlocker.h
    Created: 17 Mar 2019 10:47:42pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>

class DCBlocker
{
public:
	DCBlocker();
	~DCBlocker();
	void prepare(const int& numChannels);
	void process(const dsp::ProcessContextReplacing<float>& context);
private:

	int mNumChannels = 2;
	float mXh[2][2] = { 0.f };
	const float p = 0.992f;
	const float b0 = (1.f + p) / 2.f;
	const float b2 = -1.f * b0;
	const float a2 = p;
};