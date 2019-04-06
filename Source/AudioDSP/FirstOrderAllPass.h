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
	FirstOrderAllPass();
	~FirstOrderAllPass();
	void prepare(const int& numChannels);
	void processBlock(const dsp::ProcessContextReplacing<float>& context);
	float processSample(const float& sample, const int& channel);
	void updateCoefficients(const float& c);

private:
	std::vector<float> mXh;
	float mC    = -.89f;
	float mOldC = -.89f;
};