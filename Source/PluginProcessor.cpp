// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Danilo Pereira
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Presets.h"

namespace
{
juce::AudioProcessorValueTreeState::ParameterLayout createLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    auto F = [&layout] (const char* id, const char* name, float lo, float hi, float def, float skew = 1.0f)
    {
        layout.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { id, 1 }, name,
            juce::NormalisableRange<float> (lo, hi, 0.0f, skew), def));
    };
    auto C = [&layout] (const char* id, const char* name, juce::StringArray items, int def)
    {
        layout.add (std::make_unique<juce::AudioParameterChoice> (
            juce::ParameterID { id, 1 }, name, items, def));
    };

    const juce::StringArray waves { "Sine", "Saw", "Square", "Triangle", "Organ", "Noise",
                                    "Pulse 25", "Chip Tri", "Vowel", "Reed", "Glass", "Digital" };

    // Osciladores
    C ("o1w", "Osc1 Wave", waves, 1);
    C ("o2w", "Osc2 Wave", waves, 1);
    layout.add (std::make_unique<juce::AudioParameterInt> (juce::ParameterID { "o2s", 1 }, "Osc2 Semitone", -24, 24, 0));
    F ("o2f", "Osc2 Fine", -100.0f, 100.0f, 8.0f);
    F ("omix", "Osc Mix", 0.0f, 1.0f, 0.5f);
    F ("sub", "Sub", 0.0f, 1.0f, 0.0f);
    F ("nse", "Noise", 0.0f, 1.0f, 0.0f);
    F ("fm", "FM", 0.0f, 1.0f, 0.0f);
    F ("pw", "Pulse Width", 0.05f, 0.95f, 0.5f);

    // Filtro
    C ("ftp", "Filter Type", { "Low-pass", "Band-pass", "High-pass" }, 0);
    F ("cut", "Cutoff", 20.0f, 20000.0f, 8000.0f, 0.3f);
    F ("res", "Resonance", 0.0f, 1.0f, 0.2f);
    F ("fam", "Filter Env Amt", -1.0f, 1.0f, 0.3f);

    // Envelopes
    F ("aA", "Amp Attack", 0.001f, 5.0f, 0.01f, 0.3f);
    F ("aD", "Amp Decay", 0.001f, 5.0f, 0.3f, 0.3f);
    F ("aS", "Amp Sustain", 0.0f, 1.0f, 0.7f);
    F ("aR", "Amp Release", 0.005f, 8.0f, 0.3f, 0.3f);
    F ("fA", "Filter Attack", 0.001f, 5.0f, 0.01f, 0.3f);
    F ("fD", "Filter Decay", 0.001f, 5.0f, 0.4f, 0.3f);
    F ("fS", "Filter Sustain", 0.0f, 1.0f, 0.3f);
    F ("fR", "Filter Release", 0.005f, 8.0f, 0.3f, 0.3f);

    // Modulacao
    F ("lrt", "LFO Rate", 0.05f, 25.0f, 3.0f, 0.4f);
    F ("ldp", "LFO Depth", 0.0f, 1.0f, 0.0f);
    C ("lsh", "LFO Shape", { "Sine", "Triangle", "Saw", "Random" }, 0);
    C ("ldst", "LFO Dest", { "Pitch", "Cutoff", "Amp", "FM", "PWM" }, 0);
    F ("penv", "Pitch Env Amount", -36.0f, 36.0f, 0.0f);
    F ("pdec", "Pitch Env Decay", 0.005f, 2.0f, 0.08f, 0.4f);

    // Vozes
    C ("mode", "Voice Mode", { "Poly", "Mono" }, 0);
    F ("gld", "Glide", 0.0f, 2.0f, 0.0f, 0.4f);
    C ("vox", "Max Voices", { "8", "12", "16", "24", "32" }, 2);

    // Efeitos
    F ("drv", "Drive", 0.0f, 1.0f, 0.0f);
    F ("crsh", "Crush", 0.0f, 1.0f, 0.0f);
    F ("chr", "Chorus", 0.0f, 1.0f, 0.0f);
    F ("dlt", "Delay Time", 0.02f, 1.0f, 0.35f, 0.5f);
    F ("dlf", "Delay Feedback", 0.0f, 0.9f, 0.35f);
    F ("dlm", "Delay Mix", 0.0f, 1.0f, 0.0f);
    F ("rsz", "Reverb Size", 0.0f, 1.0f, 0.5f);
    F ("rmx", "Reverb Mix", 0.0f, 1.0f, 0.15f);
    F ("mst", "Master", 0.0f, 1.0f, 0.7f);

    return layout;
}

juce::PropertiesFile::Options settingsOptions()
{
    juce::PropertiesFile::Options o;
    o.applicationName = "Cosphere";
    o.filenameSuffix = ".settings";
    o.folderName = "Cosphere";
    o.osxLibrarySubFolder = "Application Support";
    o.commonToAllUsers = false;
    o.ignoreCaseOfKeyNames = true;
    o.storageFormat = juce::PropertiesFile::storeAsXML;
    return o;
}

const int kVoiceChoices[5] = { 8, 12, 16, 24, 32 };
} // namespace

CosphereProcessor::CosphereProcessor()
    : juce::AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "STATE", createLayout())
{
    auto g = [this] (const char* id) { return apvts.getRawParameterValue (id); };
    pp.o1w = g ("o1w"); pp.o2w = g ("o2w"); pp.o2s = g ("o2s"); pp.o2f = g ("o2f");
    pp.omix = g ("omix"); pp.sub = g ("sub"); pp.nse = g ("nse"); pp.fm = g ("fm"); pp.pw = g ("pw");
    pp.ftp = g ("ftp"); pp.cut = g ("cut"); pp.res = g ("res"); pp.fam = g ("fam");
    pp.aA = g ("aA"); pp.aD = g ("aD"); pp.aS = g ("aS"); pp.aR = g ("aR");
    pp.fA = g ("fA"); pp.fD = g ("fD"); pp.fS = g ("fS"); pp.fR = g ("fR");
    pp.lrt = g ("lrt"); pp.ldp = g ("ldp"); pp.lsh = g ("lsh"); pp.ldst = g ("ldst");
    pp.penv = g ("penv"); pp.pdec = g ("pdec");
    pp.mode = g ("mode"); pp.gld = g ("gld"); pp.vox = g ("vox");
    pp.drv = g ("drv"); pp.crsh = g ("crsh"); pp.chr = g ("chr"); pp.dlt = g ("dlt");
    pp.dlf = g ("dlf"); pp.dlm = g ("dlm"); pp.rsz = g ("rsz"); pp.rmx = g ("rmx"); pp.mst = g ("mst");

    loadUiSettings();
    applyPreset (0);
}

void CosphereProcessor::loadUiSettings()
{
    juce::PropertiesFile pf (settingsOptions());
    edW = juce::jlimit (700, 1800, pf.getIntValue ("w", kBaseW));
    uiScalePercent = juce::jlimit (70, 180, pf.getIntValue ("scale", juce::roundToInt (100.0f * edW / kBaseW)));
    edW = uiScalePercent * kBaseW / 100;
    edH = uiScalePercent * kBaseH / 100;
    theme = juce::jlimit (0, ui::kNumThemes - 1, pf.getIntValue ("theme", 0));
}

void CosphereProcessor::saveUiSettings()
{
    juce::PropertiesFile pf (settingsOptions());
    pf.setValue ("w", edW);
    pf.setValue ("scale", uiScalePercent);
    pf.setValue ("theme", theme);
    pf.saveIfNeeded();
}

bool CosphereProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void CosphereProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sampleRateHz = sampleRate;
    engine.prepare (sampleRate);
    delay.prepare (sampleRate);
    crusher.reset();

    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32) samplesPerBlock, 2 };
    chorus.prepare (spec);
    chorus.setRate (0.9f);
    chorus.setDepth (0.35f);
    chorus.setCentreDelay (8.0f);
    chorus.setFeedback (0.12f);
    chorus.reset();

    reverb.setSampleRate (sampleRate);
    reverb.reset();

    masterGain.reset (sampleRate, 0.03);
    masterGain.setCurrentAndTargetValue (pp.mst->load() * pp.mst->load() * 1.5f);
    silentSamples = 0;
}

cosphere::SynthParams CosphereProcessor::readParams() const
{
    cosphere::SynthParams p;
    p.o1w = (int) pp.o1w->load();   p.o2w = (int) pp.o2w->load();
    p.o2semi = pp.o2s->load();      p.o2fine = pp.o2f->load();
    p.omix = pp.omix->load();       p.sub = pp.sub->load();
    p.noise = pp.nse->load();       p.fm = pp.fm->load();
    p.pw = pp.pw->load();
    p.ftype = (int) pp.ftp->load(); p.cutoff = pp.cut->load();
    p.res = pp.res->load();         p.fenv = pp.fam->load();
    p.aA = pp.aA->load(); p.aD = pp.aD->load(); p.aS = pp.aS->load(); p.aR = pp.aR->load();
    p.fA = pp.fA->load(); p.fD = pp.fD->load(); p.fS = pp.fS->load(); p.fR = pp.fR->load();
    p.lrate = pp.lrt->load();       p.ldepth = pp.ldp->load();
    p.lshape = (int) pp.lsh->load(); p.ldest = (int) pp.ldst->load();
    p.penv = pp.penv->load();       p.pdec = pp.pdec->load();
    return p;
}

void CosphereProcessor::handleMidi (const juce::MidiMessage& m)
{
    if (m.isNoteOn())
        engine.noteOn (m.getNoteNumber(), m.getFloatVelocity());
    else if (m.isNoteOff())
        engine.noteOff (m.getNoteNumber());
    else if (m.isAllNotesOff() || m.isAllSoundOff())
        engine.allOff();
    else if (m.isPitchWheel())
        engine.setPitchBend ((float) (m.getPitchWheelValue() - 8192) / 8192.0f * 2.0f);
    else if (m.isController())
    {
        if (m.getControllerNumber() == 64) engine.setSustain (m.getControllerValue() >= 64);
        else if (m.getControllerNumber() == 1) engine.setModWheel ((float) m.getControllerValue() / 127.0f);
    }
}

void CosphereProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    const int n = buffer.getNumSamples();
    buffer.clear();

    // mistura as teclas clicadas no teclado da interface e atualiza as teclas acesas
    keyboardState.processNextMidiBuffer (midi, 0, n, true);

    if (buffer.getNumChannels() < 2 || n <= 0) return;

    // economia de CPU: sem notas, sem MIDI e sem cauda de efeitos -> nada a processar
    if (engine.numActive() == 0 && midi.isEmpty() && silentSamples > (juce::int64) (sampleRateHz * 0.5))
    {
        visLevel.store (visLevel.load() * 0.85f);
        visVoices.store (0);
        return;
    }

    float* L = buffer.getWritePointer (0);
    float* R = buffer.getWritePointer (1);

    engine.setVoicing (kVoiceChoices[juce::jlimit (0, 4, (int) pp.vox->load())],
                       pp.mode->load() > 0.5f, pp.gld->load());
    const cosphere::SynthParams sp = readParams();

    int pos = 0;
    for (const auto metadata : midi)
    {
        const int t = juce::jlimit (pos, n, metadata.samplePosition);
        if (t > pos)
        {
            engine.render (L + pos, R + pos, t - pos, sp);
            pos = t;
        }
        handleMidi (metadata.getMessage());
    }
    if (pos < n) engine.render (L + pos, R + pos, n - pos, sp);

    // ---- Drive
    const float drive = pp.drv->load();
    if (drive > 0.001f)
    {
        const float gain = 1.0f + drive * 6.0f;
        const float comp = 1.0f / (1.0f + drive * 1.5f);
        for (int i = 0; i < n; ++i)
        {
            L[i] = cosphere::softClip (L[i] * gain) * comp;
            R[i] = cosphere::softClip (R[i] * gain) * comp;
        }
    }

    // ---- Crush (bit/sample rate)
    const float crush = pp.crsh->load();
    if (crush > 0.01f) crusher.process (L, R, n, crush);

    // ---- Chorus
    const float chorusMix = pp.chr->load();
    if (chorusMix > 0.001f)
    {
        chorus.setMix (chorusMix);
        juce::dsp::AudioBlock<float> block (buffer);
        juce::dsp::ProcessContextReplacing<float> ctx (block);
        chorus.process (ctx);
    }

    // ---- Delay
    delay.process (L, R, n, pp.dlt->load(), pp.dlf->load(), pp.dlm->load());

    // ---- Reverb
    const float rmix = pp.rmx->load();
    if (rmix > 0.001f)
    {
        juce::Reverb::Parameters rp;
        rp.roomSize = pp.rsz->load();
        rp.damping = 0.45f;
        rp.wetLevel = rmix * 0.3f;
        rp.dryLevel = 0.5f;
        rp.width = 1.0f;
        rp.freezeMode = 0.0f;
        reverb.setParameters (rp);
        reverb.processStereo (L, R, n);
    }

    // ---- Master + limitador suave
    const float m = pp.mst->load();
    masterGain.setTargetValue (m * m * 1.5f);
    float peak = 0.0f;
    for (int i = 0; i < n; ++i)
    {
        const float gm = masterGain.getNextValue();
        L[i] = cosphere::softClip (L[i] * gm);
        R[i] = cosphere::softClip (R[i] * gm);
        peak = juce::jmax (peak, std::abs (L[i]), std::abs (R[i]));
    }

    const int active = engine.numActive();
    if (active == 0 && peak < 1.0e-5f) silentSamples += n;
    else                               silentSamples = 0;

    visLevel.store (juce::jmax (peak, visLevel.load() * 0.85f));
    visLfo.store (engine.getLfoOut());
    visVoices.store (active);
}

// ---------------------------------------------------------------- Presets / estado

int CosphereProcessor::getNumPrograms() { return (int) getPresets().size(); }

const juce::String CosphereProcessor::getProgramName (int index)
{
    const auto& list = getPresets();
    if (index < 0 || index >= (int) list.size()) return {};
    return list[(size_t) index].name;
}

void CosphereProcessor::setCurrentProgram (int index)
{
    if (index < 0 || index >= getNumPrograms()) return;
    currentProgram = index;
    applyPreset (index);
}

void CosphereProcessor::applyPreset (int index)
{
    const auto& list = getPresets();
    if (index < 0 || index >= (int) list.size()) return;

    // "vox" (vozes) e "mst" (master) sao ajustes do usuario: o preset nao mexe neles
    for (auto* prm : getParameters())
        if (auto* rp = dynamic_cast<juce::RangedAudioParameter*> (prm))
        {
            if (rp->paramID == "vox" || rp->paramID == "mst") continue;
            rp->setValueNotifyingHost (rp->getDefaultValue());
        }

    juce::StringArray tokens;
    tokens.addTokens (juce::String (list[(size_t) index].values), " ", "");
    tokens.removeEmptyStrings();

    for (const auto& t : tokens)
    {
        const int eq = t.indexOfChar ('=');
        if (eq <= 0) continue;
        if (auto* prm = apvts.getParameter (t.substring (0, eq)))
            prm->setValueNotifyingHost (prm->convertTo0to1 (t.substring (eq + 1).getFloatValue()));
    }
}

void CosphereProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    state.setProperty ("prog", currentProgram, nullptr);
    state.setProperty ("w", edW, nullptr);
    state.setProperty ("scale", uiScalePercent, nullptr);
    state.setProperty ("theme", theme, nullptr);
    if (auto xml = state.createXml())
        copyXmlToBinary (*xml, destData);
}

void CosphereProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (apvts.state.getType()))
        {
            auto state = juce::ValueTree::fromXml (*xml);
            currentProgram = juce::jlimit (0, getNumPrograms() - 1, (int) state.getProperty ("prog", 0));
            if (state.hasProperty ("w"))
            {
                edW = juce::jlimit (700, 1800, (int) state.getProperty ("w", kBaseW));
                edH = (int) ((double) edW * (double) kBaseH / (double) kBaseW);
            }
            // Preferencia global tem prioridade sobre o tamanho salvo no projeto.
            // A escala so vem do projeto se nenhuma preferencia global existir.
            juce::PropertiesFile pf (settingsOptions());
            if (! pf.containsKey ("scale"))
                uiScalePercent = juce::jlimit (70, 180, (int) state.getProperty ("scale", juce::roundToInt (100.0f * edW / kBaseW)));
            if (state.hasProperty ("theme"))
                theme = juce::jlimit (0, ui::kNumThemes - 1, (int) state.getProperty ("theme", 0));
            apvts.replaceState (state);
        }
}

juce::AudioProcessorEditor* CosphereProcessor::createEditor()
{
    return new CosphereEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CosphereProcessor();
}
