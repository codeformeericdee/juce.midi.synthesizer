/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Juce_midi_synthesizerAudioProcessor::Juce_midi_synthesizerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), value_tree_state (*this, nullptr, "Parameters", createParams())
#endif
{
    /* Sound and voice de-references are handled by the Synthesizer class */
    synthesizer.addSound(new SynthesizerSound());
    synthesizer.addVoice(new SynthesizerVoice());
}

Juce_midi_synthesizerAudioProcessor::~Juce_midi_synthesizerAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout Juce_midi_synthesizerAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> oscillator_parameters;

    oscillator_parameters.push_back(std::make_unique<juce::AudioParameterChoice>("osci", "Oscillator", juce::StringArray{ "Square", "Saw", "Sine" }, 0));

    oscillator_parameters.push_back(std::make_unique<juce::AudioParameterFloat>("atta", "Attack",  juce::NormalisableRange<float> {0.1f, 1.0f}, 0.1f));
    oscillator_parameters.push_back(std::make_unique<juce::AudioParameterFloat>("deca", "Decay",   juce::NormalisableRange<float> {0.1f, 1.0f}, 0.1f));
    oscillator_parameters.push_back(std::make_unique<juce::AudioParameterFloat>("sust", "Sustain", juce::NormalisableRange<float> {0.1f, 1.0f}, 1.0f));
    oscillator_parameters.push_back(std::make_unique<juce::AudioParameterFloat>("rele", "Release", juce::NormalisableRange<float> {0.1f, 4.0f}, 0.4f));
    
    return { oscillator_parameters.begin(), oscillator_parameters.end()  };
}

//==============================================================================
const juce::String Juce_midi_synthesizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Juce_midi_synthesizerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Juce_midi_synthesizerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Juce_midi_synthesizerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Juce_midi_synthesizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Juce_midi_synthesizerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Juce_midi_synthesizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Juce_midi_synthesizerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Juce_midi_synthesizerAudioProcessor::getProgramName (int index)
{
    return {};
}

void Juce_midi_synthesizerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Juce_midi_synthesizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synthesizer.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synthesizer.getNumVoices(); i++)
    {
        /* If voice data comes from the SynthesizerVoice address markup */
        if (auto voice = dynamic_cast<SynthesizerVoice*> (synthesizer.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void Juce_midi_synthesizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Juce_midi_synthesizerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void Juce_midi_synthesizerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int i = 0; i < synthesizer.getNumVoices(); i++)
    {
        /* Datatype is within boundary, or synthesizer was correctly cast */
        if (auto voice = dynamic_cast<juce::SynthesiserVoice*> (synthesizer.getVoice(i)));
    }

    int startSample = 0;
    synthesizer.renderNextBlock(buffer, midiMessages, startSample, buffer.getNumSamples());

    for (const juce::MidiMessageMetadata metadata : midiMessages)
        if (metadata.numBytes == 3)
            juce::Logger::writeToLog("Time stamp: " + juce::String(metadata.getMessage().getTimeStamp()));

}

//==============================================================================
bool Juce_midi_synthesizerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Juce_midi_synthesizerAudioProcessor::createEditor()
{
    return new Juce_midi_synthesizerAudioProcessorEditor (*this);
}

//==============================================================================
void Juce_midi_synthesizerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Juce_midi_synthesizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Juce_midi_synthesizerAudioProcessor();
}
