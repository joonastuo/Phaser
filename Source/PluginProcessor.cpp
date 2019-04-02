/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PhaserAudioProcessor::PhaserAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
	mState(*this, nullptr, Identifier("PhaserPlugin"),
		{
			  std::make_unique<AudioParameterFloat>(IDs::speed,
													"Speed",
													NormalisableRange<float> (0.0, 100.0),
												    40.0,
													String(),
												    AudioProcessorParameter::genericParameter,
												    [](float value, int maxStringLength) {return static_cast<String>(round(0.069f * exp(0.04f * value) * 100.f) / 100.f); },
												    [](const String& text) {return log(100.f * text.getFloatValue()) / 0.04f; }
													 ),
			  std::make_unique<AudioParameterFloat>(IDs::wetness,
													"Wetness",
													NormalisableRange<float> (0.00, 0.5),
												    0.5,
												    String(),
												    AudioProcessorParameter::genericParameter,
												    [](float value, int maxStringLength) {return static_cast<String>(round((value * 2) * 100.f * 100.f) / 100.f); },
												    [](const String& text) {return round(((text.getFloatValue() / 2.f) / 100.f) * 100.f) / 100.f; }
													),
			  std::make_unique<AudioParameterFloat>(IDs::feedback,
													"Feedback",
													NormalisableRange<float>(-0.99, 0.99),
												    0.0,
												    String(),
												    AudioProcessorParameter::genericParameter,
												    [](float value, int maxStringLength) {return static_cast<String>(round(value * 100.f * 100.f) / 100.f); },
												    [](const String& text) {return round((text.getFloatValue() / 100.f) * 100.f) / 100.f; }
													)
		}),
	mPhaser(mState)
#endif
{
}

PhaserAudioProcessor::~PhaserAudioProcessor()
{
}

//==============================================================================
const String PhaserAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PhaserAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PhaserAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PhaserAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PhaserAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PhaserAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PhaserAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PhaserAudioProcessor::setCurrentProgram (int index)
{
}

const String PhaserAudioProcessor::getProgramName (int index)
{
    return {};
}

void PhaserAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PhaserAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getNumInputChannels();
	spec.sampleRate = sampleRate;
	mPhaser.prepare(spec);
}

void PhaserAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PhaserAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PhaserAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	mPhaser.process(buffer);
}

//==============================================================================
bool PhaserAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PhaserAudioProcessor::createEditor()
{
    return new PhaserAudioProcessorEditor (*this);
}

//==============================================================================
void PhaserAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	auto state = mState.copyState();
	std::unique_ptr<XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void PhaserAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(mState.state.getType()))
			mState.replaceState(ValueTree::fromXml(*xmlState));
}

AudioProcessorValueTreeState & PhaserAudioProcessor::getState()
{
	return mState;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PhaserAudioProcessor();
}
