#pragma once
#include "Common.h"
#include "Synth.h"

class CosmoProcessor : public juce::AudioProcessor
{
public:
    CosmoProcessor();
    ~CosmoProcessor() override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "CosmoSphere"; }
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

    juce::AudioProcessorValueTreeState apvts;

    // dados para a esfera animada (lidos pela interface)
    std::atomic<float> visLevel { 0.0f };
    std::atomic<float> visLfo { 0.0f };
    std::atomic<int>   visVoices { 0 };

    int edW = 1000, edH = 640;

private:
    cosmo::SynthParams readParams() const;
    void handleMidi (const juce::MidiMessage& m);

    struct Ptrs
    {
        std::atomic<float> *o1w, *o2w, *o2s, *o2f, *omix, *sub, *nse, *fm, *pw;
        std::atomic<float> *ftp, *cut, *res, *fam;
        std::atomic<float> *aA, *aD, *aS, *aR, *fA, *fD, *fS, *fR;
        std::atomic<float> *lrt, *ldp, *lsh, *ldst;
        std::atomic<float> *drv, *chr, *dlt, *dlf, *dlm, *rsz, *rmx, *mst;
    } pp {};

    cosmo::Engine engine;
    cosmo::StereoDelay delay;
    juce::dsp::Chorus<float> chorus;
    juce::Reverb reverb;
    juce::SmoothedValue<float> masterGain;
    int currentProgram = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CosmoProcessor)
};
