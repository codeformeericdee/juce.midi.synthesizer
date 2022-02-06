/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Juce_midi_synthesizerAudioProcessorEditor::Juce_midi_synthesizerAudioProcessorEditor (Juce_midi_synthesizerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    using ValueTreeState_SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    this->attack_attachment  = std::make_unique<ValueTreeState_SliderAttachment>(this->audioProcessor.value_tree_state, "atta", this->attack_slider);
    this->decay_attachment   = std::make_unique<ValueTreeState_SliderAttachment>(this->audioProcessor.value_tree_state, "deca", this->decay_slider);
    this->sustain_attachment = std::make_unique<ValueTreeState_SliderAttachment>(this->audioProcessor.value_tree_state, "sust", this->sustain_slider);
    this->release_attachment = std::make_unique<ValueTreeState_SliderAttachment>(this->audioProcessor.value_tree_state, "rele", this->release_slider);

    this->wave_selector = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(this->audioProcessor.value_tree_state, "osci", this->wave_selection);
}

Juce_midi_synthesizerAudioProcessorEditor::~Juce_midi_synthesizerAudioProcessorEditor()
{
}

//==============================================================================
void Juce_midi_synthesizerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void Juce_midi_synthesizerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
