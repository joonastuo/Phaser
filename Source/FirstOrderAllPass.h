/*
  ==============================================================================

    FirstOrderAllPass.h
    Created: 17 Mar 2019 10:47:21pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>

class FirstOrderAllPass
{
public:
	FirstOrderAllPass(AudioProcessorValueTreeState& state);
	~FirstOrderAllPass();
	void prepare(const int& numChannels);
	void process(AudioBuffer<float>& buffer);

private:
	AudioProcessorValueTreeState& mState;
	std::vector<float> mXh;
};