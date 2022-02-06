/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Juce_midi_synthesizerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Juce_midi_synthesizerAudioProcessorEditor (Juce_midi_synthesizerAudioProcessor&);
    ~Juce_midi_synthesizerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Juce_midi_synthesizerAudioProcessor& audioProcessor;

    juce::Slider attack_slider;
    juce::Slider decay_slider;
    juce::Slider sustain_slider;
    juce::Slider release_slider;

    juce::ComboBox wave_selection;

    using ValueTreeState_SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<ValueTreeState_SliderAttachment> attack_attachment;
    std::unique_ptr<ValueTreeState_SliderAttachment> decay_attachment;
    std::unique_ptr<ValueTreeState_SliderAttachment> sustain_attachment;
    std::unique_ptr<ValueTreeState_SliderAttachment> release_attachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> wave_selector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Juce_midi_synthesizerAudioProcessorEditor)
};
