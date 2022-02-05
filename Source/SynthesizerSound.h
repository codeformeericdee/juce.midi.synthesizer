/*
  ==============================================================================

    SynthesizerSound.h
    Created: 5 Feb 2022 5:23:44am
    Author:  eric_

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/* Extends SynthesiserSound */
class SynthesizerSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; };
    bool appliesToChannel(int midiChannel) override { return true; };
};