/*
  ==============================================================================

    Phaser.h
    Created: 18 Mar 2019 3:36:31pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FirstOrderAllPass.h"
#include "DCBlocker.h"

class Phaser
{
public:
	Phaser(AudioProcessorValueTreeState& state);
	~Phaser();
	void prepare(dsp::ProcessSpec& spec);
	void process(AudioBuffer<float>& buffer);

private:
	AudioProcessorValueTreeState& mState;
	DCBlocker mDCBlocker;
	std::array<FirstOrderAllPass, 10> mAPFilters;
	float mAPCoeff = -.49f;
	float mSampleRate = 44100.f;
	float mPhase = 0.f;

};