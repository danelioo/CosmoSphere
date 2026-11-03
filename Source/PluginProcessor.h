// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Danilo Pereira
#pragma once
#include "Common.h"
#include "Synth.h"

class CosphereProcessor : public juce::AudioProcessor
{
public:
    static constexpr int kBaseW = 1000;
    static constexpr int kBaseH = 700;

    CosphereProcessor();
    ~CosphereProcessor() override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Cosphere"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 4.0; }

    int getNumPrograms() override;
    int getCurrentProgram() override { return currentProgram; }
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void applyPreset (int index);

    // configuracoes de interface lembradas entre sessoes (tamanho e tema)
    void loadUiSettings();
    void saveUiSettings();

    juce::AudioProcessorValueTreeState apvts;
    juce::MidiKeyboardState keyboardState;

    // dados para o globo animado (lidos pela interface)
    std::atomic<float> visLevel { 0.0f };
    std::atomic<float> visLfo { 0.0f };
    std::atomic<int>   visVoices { 0 };

    int edW = kBaseW, edH = kBaseH;
    int theme = 0;
    int uiScalePercent = 100;

private:
    cosphere::SynthParams readParams() const;
    void handleMidi (const juce::MidiMessage& m);

    struct Ptrs
    {
        std::atomic<float> *o1w, *o2w, *o2s, *o2f, *omix, *sub, *nse, *fm, *pw;
        std::atomic<float> *ftp, *cut, *res, *fam;
        std::atomic<float> *aA, *aD, *aS, *aR, *fA, *fD, *fS, *fR;
        std::atomic<float> *lrt, *ldp, *lsh, *ldst, *lsy, *ldiv, *penv, *pdec;
        std::atomic<float> *mode, *gld, *vox;
        std::atomic<float> *drv, *crsh, *chr, *dlt, *dlf, *dlm, *dsy, *ddiv, *rsz, *rmx, *mst;
    } pp {};

    cosphere::Engine engine;
    cosphere::StereoDelay delay;
    cosphere::BitCrusher crusher;
    juce::dsp::Chorus<float> chorus;
    juce::Reverb reverb;
    juce::SmoothedValue<float> masterGain;
    int currentProgram = 0;
    double sampleRateHz = 44100.0;
    juce::int64 silentSamples = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CosphereProcessor)
};
