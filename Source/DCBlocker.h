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
	std::vector<std::vector<float>> mXh;
};