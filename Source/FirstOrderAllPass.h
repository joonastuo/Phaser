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
	void prepare(const int& numChannels, const int& filterNum);
	void process(dsp::ProcessContextReplacing<float>& context);

private:
	AudioProcessorValueTreeState& mState;
	std::vector<float> mXh;
	int mFilterNum;
	float mOldC = -.89;
};