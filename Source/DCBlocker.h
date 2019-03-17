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
	void process(AudioBuffer<float>& buffer);
private:

	int mNumChannels = 2;
	const float mP = 0.992;
	const float mA = (1 + mP) / 2;
	std::vector<float> mXm1;
	std::vector<float> mXm2;
	std::vector<float> mYm1;
	std::vector<float> mYm2;
};