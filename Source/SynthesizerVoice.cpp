/*
  ==============================================================================

    SynthesizerVoice.cpp
    Created: 5 Feb 2022 5:21:35am
    Author:  eric_

  ==============================================================================
*/

#include "SynthesizerVoice.h"

bool SynthesizerVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    /* Validates block size / type. If the class does not match expected input, a nullptr is returned */
    return dynamic_cast<juce::SynthesiserSound*> (sound) != nullptr;
}

void SynthesizerVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    this->float_oscillator_1.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    this->adsr.noteOn();
}

void SynthesizerVoice::stopNote(float velocity, bool allowTailOff)
{
    if (this->adsr.isActive() || allowTailOff)
    this->adsr.noteOff();
    else {
        clearCurrentNote();
    }


}

void SynthesizerVoice::controllerMoved(int controllerNumber, int newControllerValue)
{}

void SynthesizerVoice::pitchWheelMoved(int newPitchWheelValue)
{}

void SynthesizerVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    jassert(this->is_asserted);

    if (isVoiceActive())
    {
        double_buffer.setSize(outputBuffer.getNumChannels(), numSamples, false, true, true);
        double_buffer.clear(); /* With clearing extra set to true this might not be needed */

        /* Replace a typecasted array to the buffer zone via indirect address mode */
        juce::dsp::AudioBlock<float> audioBlock{ double_buffer };
        this->float_oscillator_1.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        this->float_gain_1.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        this->adsr.applyEnvelopeToBuffer(double_buffer, 0, double_buffer.getNumSamples());

        if (startSample != 0)
            jassertfalse;

        for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++)
        {
            if (this->adsr.isActive())
            {
                outputBuffer.addFrom(channel, startSample, double_buffer, channel, 0, numSamples);
            }
            else {
                clearCurrentNote();
            }
        }
    }
    else {
        return;
    }
}

/* Not virtual: */

void SynthesizerVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    /* This section defines live buffer limitations */
    juce::dsp::ProcessSpec pSpec;
    pSpec.sampleRate = sampleRate;
    pSpec.numChannels = outputChannels;
    pSpec.maximumBlockSize = samplesPerBlock;

    this->float_oscillator_1.prepare(pSpec);
    this->float_gain_1.prepare(pSpec);
    this->float_gain_1.setGainLinear(0.01f);

    this->adsr_parameters.attack = 0.81f;
    this->adsr_parameters.decay = 1.0f;
    this->adsr_parameters.sustain = 1.21f;
    this->adsr_parameters.release = 1.44f;
    
    this->adsr.setParameters(this->adsr_parameters);
    this->adsr.setSampleRate(sampleRate);

    this->is_asserted = true;
}

