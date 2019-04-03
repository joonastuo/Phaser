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
	void prepare(const int& numChannels, const int& filterNum);
	void processBlock(dsp::ProcessContextReplacing<float>& context);
	float processSample(const float& sample, const int& channel);
	void updateCoefficients(const float& c);

private:
	float mXh[2] = { 0.0 };
	int mFilterNum;
	float mC = -.89;
	float mOldC = -.89;
};