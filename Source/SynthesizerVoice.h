/*
  ==============================================================================

    SynthesizerVoice.h
    Created: 5 Feb 2022 5:21:35am
    Author:  eric_

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SynthesizerSound.h"

class SynthesizerVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
private:

    juce::AudioBuffer<float> double_buffer;

    juce::dsp::Gain<float> float_gain_1;

    /* Declares an oscillator class that is set to operate on float values
    * currently it is using a lambda function to generate a sine wave, so it needs to be changed
    * to an actual wave generator class function */
    juce::dsp::Oscillator<float> float_oscillator_1{ [](float x) {return std::sin(x); } };

    juce::ADSR adsr;
    juce::ADSR::Parameters adsr_parameters;

    bool is_asserted = false;
};