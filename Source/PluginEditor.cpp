// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Danilo Pereira
#include "PluginEditor.h"
#include "Presets.h"

using ui::Fmt;

CosphereEditor::CosphereEditor (CosphereProcessor& p)
    : juce::AudioProcessorEditor (&p), proc (p),
      o1w (p.apvts, "o1w", "WAVE 1"),
      o2w (p.apvts, "o2w", "WAVE 2"),
      omix (p.apvts, "omix", "OSC MIX", Fmt::Pct),
      o2s (p.apvts, "o2s", "OSC2 SEMI", Fmt::Semis),
      o2f (p.apvts, "o2f", "OSC2 FINE", Fmt::Cents),
      sub (p.apvts, "sub", "SUB", Fmt::Pct),
      nse (p.apvts, "nse", "NOISE", Fmt::Pct),
      fm (p.apvts, "fm", "FM", Fmt::Pct),
      pw (p.apvts, "pw", "PULSE W", Fmt::Pct),
      ftp (p.apvts, "ftp", "TYPE"),
      cut (p.apvts, "cut", "CUTOFF", Fmt::Hz),
      res (p.apvts, "res", "RESO", Fmt::Pct),
      fam (p.apvts, "fam", "ENV AMT", Fmt::Bipolar),
      mode (p.apvts, "mode", "MODE"),
      vox (p.apvts, "vox", "MAX VOICES"),
      gld (p.apvts, "gld", "GLIDE", Fmt::Sec),
      aA (p.apvts, "aA", "ATTACK", Fmt::Sec),
      aD (p.apvts, "aD", "DECAY", Fmt::Sec),
      aS (p.apvts, "aS", "SUSTAIN", Fmt::Pct),
      aR (p.apvts, "aR", "RELEASE", Fmt::Sec),
      fA (p.apvts, "fA", "ATTACK", Fmt::Sec),
      fD (p.apvts, "fD", "DECAY", Fmt::Sec),
      fS (p.apvts, "fS", "SUSTAIN", Fmt::Pct),
      fR (p.apvts, "fR", "RELEASE", Fmt::Sec),
      lsh (p.apvts, "lsh", "SHAPE"),
      ldst (p.apvts, "ldst", "DESTINATION"),
      lrt (p.apvts, "lrt", "RATE", Fmt::Hz),
      ldp (p.apvts, "ldp", "DEPTH", Fmt::Pct),
      penv (p.apvts, "penv", "PITCH ENV", Fmt::Semis),
      pdec (p.apvts, "pdec", "PITCH DECAY", Fmt::Sec),
      drv (p.apvts, "drv", "DRIVE", Fmt::Pct),
      crsh (p.apvts, "crsh", "CRUSH", Fmt::Pct),
      chr (p.apvts, "chr", "CHORUS", Fmt::Pct),
      dlm (p.apvts, "dlm", "DELAY", Fmt::Pct),
      dlt (p.apvts, "dlt", "TIME", Fmt::Sec),
      dsy (p.apvts, "dsy", "SYNC"),
      ddiv (p.apvts, "ddiv", "BEAT"),
      dlf (p.apvts, "dlf", "FEEDBACK", Fmt::Pct),
      rmx (p.apvts, "rmx", "REVERB", Fmt::Pct),
      rsz (p.apvts, "rsz", "SIZE", Fmt::Pct),
      mst (p.apvts, "mst", "MASTER", Fmt::Pct),
      oscPanel ("OSCILLATORS", 3),
      filterPanel ("FILTER", 4),
      voicePanel ("VOICING", 3),
      ampPanel ("AMP ENVELOPE", 4),
      fltPanel ("FILTER ENVELOPE", 4),
      lfoPanel ("MODULATION", 3),
      fxPanel ("EFFECTS", 3),
      globe (p),
      keyboard (p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    setLookAndFeel (&lnf);

    oscPanel.add (o1w);  oscPanel.add (o2w);  oscPanel.add (omix);
    oscPanel.add (o2s);  oscPanel.add (o2f);  oscPanel.add (sub);
    oscPanel.add (nse);  oscPanel.add (fm);   oscPanel.add (pw);

    filterPanel.add (ftp); filterPanel.add (cut); filterPanel.add (res); filterPanel.add (fam);

    voicePanel.add (mode); voicePanel.add (vox); voicePanel.add (gld);

    ampPanel.add (aA); ampPanel.add (aD); ampPanel.add (aS); ampPanel.add (aR);
    fltPanel.add (fA); fltPanel.add (fD); fltPanel.add (fS); fltPanel.add (fR);

    lfoPanel.add (lsh); lfoPanel.add (ldst); lfoPanel.add (lrt);
    lfoPanel.add (ldp); lfoPanel.add (penv); lfoPanel.add (pdec);

    fxPanel.add (drv); fxPanel.add (crsh); fxPanel.add (chr);
    fxPanel.add (dlm); fxPanel.add (dlt); fxPanel.add (dlf);
    fxPanel.add (dsy); fxPanel.add (ddiv);
    fxPanel.add (rmx); fxPanel.add (rsz);

    addAndMakeVisible (oscPanel);
    addAndMakeVisible (filterPanel);
    addAndMakeVisible (voicePanel);
    addAndMakeVisible (ampPanel);
    addAndMakeVisible (fltPanel);
    addAndMakeVisible (lfoPanel);
    addAndMakeVisible (fxPanel);
    addAndMakeVisible (globe);

    // teclado: toca ao clicar e mostra as notas que estao soando
    keyboard.setAvailableRange (36, 96);
    keyboard.setLowestVisibleKey (36);
    keyboard.setScrollButtonsVisible (false);
    keyboard.setVelocity (0.85f, true);
    // JUCE's computer-keyboard note mappings (QWERTY), activated when focused.
    keyboard.clearKeyMappings();
    const juce::String keys = "zsxdcvgbhnjm,l.;/";
    for (int i = 0; i < keys.length(); ++i)
        keyboard.setKeyPressForNote (juce::KeyPress (keys[i]), i);
    keyboard.setKeyPressBaseOctave (4);
    keyboard.setWantsKeyboardFocus (true);
    addAndMakeVisible (keyboard);

    presetBtn.onClick = [this] { showPresetMenu(); };
    presetBtn.setButtonText (proc.getProgramName (proc.getCurrentProgram()));
    addAndMakeVisible (presetBtn);

    prevBtn.onClick = [this]
    {
        const int n = proc.getNumPrograms();
        const int cur = proc.getCurrentProgram();
        proc.setCurrentProgram ((cur + n - 1) % n);
        presetBtn.setButtonText (proc.getProgramName (proc.getCurrentProgram()));
    };
    nextBtn.onClick = [this]
    {
        const int n = proc.getNumPrograms();
        const int cur = proc.getCurrentProgram();
        proc.setCurrentProgram ((cur + 1) % n);
        presetBtn.setButtonText (proc.getProgramName (proc.getCurrentProgram()));
    };
    addAndMakeVisible (prevBtn);
    addAndMakeVisible (nextBtn);

    infoBtn.onClick = [this] { about.setVisible (true); about.toFront (true); };
    addAndMakeVisible (infoBtn);

    addAndMakeVisible (mst); // Master sempre no canto superior direito
    options.themes.onSelect = [this] (int i) { applyTheme (i); };
    options.onScale = [this] (int percent)
    {
        if (percent <= 0) return;
        proc.uiScalePercent = percent;
        setSize (percent * CosphereProcessor::kBaseW / 100,
                 percent * CosphereProcessor::kBaseH / 100);
        proc.saveUiSettings();
    };
    options.onClose = [this] { options.setVisible (false); };
    addChildComponent (options);
    optionsBtn.onClick = [this]
    {
        options.scaleBox.setSelectedId (proc.uiScalePercent, juce::dontSendNotification);
        options.setVisible (true);
        options.toFront (true);
    };
    addAndMakeVisible (optionsBtn);

    about.onClose = [this] { about.setVisible (false); };
    addChildComponent (about);

    applyTheme (proc.theme);

    setResizable (true, true);
    setResizeLimits (700, 490, 1800, 1260);
    if (auto* c = getConstrainer())
        c->setFixedAspectRatio ((double) CosphereProcessor::kBaseW / (double) CosphereProcessor::kBaseH);
    setSize (juce::jlimit (700, 1800, proc.uiScalePercent * CosphereProcessor::kBaseW / 100),
             juce::jlimit (490, 1260, proc.uiScalePercent * CosphereProcessor::kBaseH / 100));

   #if JucePlugin_Build_Standalone
    keyboard.grabKeyboardFocus();
   #endif
    startTimerHz (8);
}

CosphereEditor::~CosphereEditor()
{
    stopTimer();
    proc.saveUiSettings();
    setLookAndFeel (nullptr);
}

void CosphereEditor::showPresetMenu()
{
    juce::PopupMenu menu;
    const auto& presets = getPresets();
    juce::String lastCategory;
    juce::PopupMenu submenu;
    for (int i = 0; i < (int) presets.size(); ++i)
    {
        const juce::String category (presets[(size_t) i].category);
        if (category != lastCategory)
        {
            if (lastCategory.isNotEmpty()) menu.addSubMenu (lastCategory, submenu);
            submenu = juce::PopupMenu();
            lastCategory = category;
        }
        submenu.addItem (i + 1, presets[(size_t) i].name, true, i == proc.getCurrentProgram());
    }
    if (lastCategory.isNotEmpty()) menu.addSubMenu (lastCategory, submenu);
    juce::Component::SafePointer<CosphereEditor> safe (this);
    menu.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (&presetBtn),
                        [safe] (int id)
                        {
                            if (safe != nullptr && id > 0)
                            {
                                safe->proc.setCurrentProgram (id - 1);
                                safe->presetBtn.setButtonText (safe->proc.getProgramName (id - 1));
                            }
                        });
}

void CosphereEditor::applyTheme (int index)
{
    index = juce::jlimit (0, ui::kNumThemes - 1, index);
    if (index == currentTheme) return;
    currentTheme = index;
    proc.theme = index;

    const ui::Theme& t = ui::themes()[index];
    lnf.applyTheme (t);
    options.themes.selected = index;

    keyboard.setColour (juce::MidiKeyboardComponent::whiteNoteColourId, juce::Colour (t.keyWhite));
    keyboard.setColour (juce::MidiKeyboardComponent::blackNoteColourId, juce::Colour (t.keyBlack));
    keyboard.setColour (juce::MidiKeyboardComponent::keySeparatorLineColourId, juce::Colour (t.border));
    keyboard.setColour (juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId, juce::Colour (t.accent).withAlpha (0.25f));
    keyboard.setColour (juce::MidiKeyboardComponent::keyDownOverlayColourId, juce::Colour (t.accent).withAlpha (0.75f));
    keyboard.setColour (juce::MidiKeyboardComponent::shadowColourId, juce::Colours::transparentBlack);

    sendLookAndFeelChange();
    repaint();
    dirty = true;
}

void CosphereEditor::timerCallback()
{
    const int cur = proc.getCurrentProgram();
    if (presetBtn.getButtonText() != proc.getProgramName (cur))
        presetBtn.setButtonText (proc.getProgramName (cur));

    if (proc.theme != currentTheme)
        applyTheme (proc.theme);

    if (dirty)
    {
        dirty = false;
        proc.saveUiSettings();
    }
}

void CosphereEditor::paint (juce::Graphics& g)
{
    const ui::Theme& t = ui::themes()[juce::jlimit (0, ui::kNumThemes - 1, currentTheme)];
    const float s = (float) getWidth() / (float) CosphereProcessor::kBaseW;

    g.setGradientFill (juce::ColourGradient (juce::Colour (t.bgTop), 0.0f, 0.0f,
                                             juce::Colour (t.bgBot), 0.0f, (float) getHeight(), false));
    g.fillAll();

    g.setColour (juce::Colour (t.accent));
    g.setFont (lnf.titleFont (69.0f * s));
    g.drawText ("Cosphere",
                juce::Rectangle<int> (juce::roundToInt (22.0f * s), juce::roundToInt (4.0f * s),
                                      juce::roundToInt (425.0f * s), juce::roundToInt (75.0f * s)),
                juce::Justification::centredLeft);

    g.setColour (juce::Colour (t.border));
    g.fillRect (juce::Rectangle<int> (juce::roundToInt (10.0f * s), juce::roundToInt (80.0f * s),
                                      getWidth() - juce::roundToInt (20.0f * s), juce::jmax (1, juce::roundToInt (1.0f * s))));
}

void CosphereEditor::resized()
{
    const float s = (float) getWidth() / (float) CosphereProcessor::kBaseW;
    lnf.scale = s;
    proc.edW = getWidth();
    proc.edH = getHeight();
    proc.uiScalePercent = juce::jlimit (70, 180, juce::roundToInt (100.0f * s));
    dirty = true;

    auto area = getLocalBounds();
    const int m = juce::roundToInt (10.0f * s);

    // ---- cabecalho
    auto header = area.removeFromTop (juce::roundToInt (82.0f * s));
    auto hr = header.reduced (m, juce::roundToInt (8.0f * s));
    // Dedicated master area: 56px knob with its own label and value.
    auto masterArea = header.removeFromRight (juce::roundToInt (95.0f * s));
    mst.setBounds (masterArea.reduced (juce::roundToInt (4.0f * s), 0));
    hr.removeFromRight (juce::roundToInt (95.0f * s));
    hr.removeFromRight (juce::roundToInt (8.0f * s));
    infoBtn.setBounds (hr.removeFromRight (juce::roundToInt (34.0f * s)).withSizeKeepingCentre (juce::roundToInt (34.0f * s), juce::roundToInt (34.0f * s)));
    hr.removeFromRight (juce::roundToInt (8.0f * s));
    optionsBtn.setBounds (hr.removeFromRight (juce::roundToInt (88.0f * s)).withSizeKeepingCentre (juce::roundToInt (88.0f * s), juce::roundToInt (34.0f * s)));
    hr.removeFromRight (juce::roundToInt (8.0f * s));

    auto presetArea = hr.removeFromRight (juce::roundToInt (310.0f * s)).withSizeKeepingCentre (juce::roundToInt (310.0f * s), juce::roundToInt (34.0f * s));
    prevBtn.setBounds (presetArea.removeFromLeft (juce::roundToInt (34.0f * s)));
    nextBtn.setBounds (presetArea.removeFromRight (juce::roundToInt (34.0f * s)));
    presetBtn.setBounds (presetArea.reduced (juce::roundToInt (5.0f * s), 0));


    // ---- teclado
    auto kb = area.removeFromBottom (juce::roundToInt (78.0f * s)).reduced (m, 0).withTrimmedBottom (m);
    keyboard.setBounds (kb);
    keyboard.setKeyWidth ((float) kb.getWidth() / 36.0f);

    // ---- corpo
    auto body = area.reduced (m, 0).withTrimmedBottom (m).withTrimmedTop (juce::roundToInt (4.0f * s));
    const int gap = m;
    const int usable = body.getWidth() - 2 * gap;
    auto left = body.removeFromLeft (usable * 30 / 100);
    body.removeFromLeft (gap);
    auto right = body.removeFromRight (usable * 30 / 100);
    body.removeFromRight (gap);
    auto centre = body;

    const int lh = left.getHeight();
    oscPanel.setBounds (left.removeFromTop (lh * 54 / 100));
    left.removeFromTop (gap);
    filterPanel.setBounds (left.removeFromTop (lh * 22 / 100));
    left.removeFromTop (gap);
    voicePanel.setBounds (left);

    const int ch = centre.getHeight();
    globe.setBounds (centre.removeFromTop (ch * 40 / 100));
    centre.removeFromTop (gap);
    ampPanel.setBounds (centre.removeFromTop ((centre.getHeight() - gap) / 2));
    centre.removeFromTop (gap);
    fltPanel.setBounds (centre);

    const int rh = right.getHeight();
    lfoPanel.setBounds (right.removeFromTop (rh * 38 / 100));
    right.removeFromTop (gap);
    fxPanel.setBounds (right);

    about.setBounds (getLocalBounds());
    options.setBounds (getLocalBounds());
}
