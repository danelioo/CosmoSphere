#include "PluginEditor.h"

using ui::Fmt;

CosmoEditor::CosmoEditor (CosmoProcessor& p)
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
      res (p.apvts, "res", "RESONANCE", Fmt::Pct),
      fam (p.apvts, "fam", "ENV AMOUNT", Fmt::Bipolar),
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
      drv (p.apvts, "drv", "DRIVE", Fmt::Pct),
      chr (p.apvts, "chr", "CHORUS", Fmt::Pct),
      mst (p.apvts, "mst", "MASTER", Fmt::Pct),
      dlm (p.apvts, "dlm", "DELAY", Fmt::Pct),
      dlt (p.apvts, "dlt", "TIME", Fmt::Sec),
      dlf (p.apvts, "dlf", "FEEDBACK", Fmt::Pct),
      rmx (p.apvts, "rmx", "REVERB", Fmt::Pct),
      rsz (p.apvts, "rsz", "SIZE", Fmt::Pct),
      oscPanel ("OSCILLATORS", 3),
      filterPanel ("FILTER", 2),
      ampPanel ("AMP ENVELOPE", 4),
      fltPanel ("FILTER ENVELOPE", 4),
      lfoPanel ("LFO / MODULATION", 2),
      fxPanel ("EFFECTS", 3),
      sphere (p)
{
    setLookAndFeel (&lnf);

    oscPanel.add (o1w);  oscPanel.add (o2w);  oscPanel.add (omix);
    oscPanel.add (o2s);  oscPanel.add (o2f);  oscPanel.add (sub);
    oscPanel.add (nse);  oscPanel.add (fm);   oscPanel.add (pw);

    filterPanel.add (ftp); filterPanel.add (cut);
    filterPanel.add (res); filterPanel.add (fam);

    ampPanel.add (aA); ampPanel.add (aD); ampPanel.add (aS); ampPanel.add (aR);
    fltPanel.add (fA); fltPanel.add (fD); fltPanel.add (fS); fltPanel.add (fR);

    lfoPanel.add (lsh); lfoPanel.add (ldst);
    lfoPanel.add (lrt); lfoPanel.add (ldp);

    fxPanel.add (drv); fxPanel.add (chr); fxPanel.add (mst);
    fxPanel.add (dlm); fxPanel.add (dlt); fxPanel.add (dlf);
    fxPanel.add (rmx); fxPanel.add (rsz);

    addAndMakeVisible (oscPanel);
    addAndMakeVisible (filterPanel);
    addAndMakeVisible (ampPanel);
    addAndMakeVisible (fltPanel);
    addAndMakeVisible (lfoPanel);
    addAndMakeVisible (fxPanel);
    addAndMakeVisible (sphere);

    for (int i = 0; i < proc.getNumPrograms(); ++i)
        presetBox.addItem (proc.getProgramName (i), i + 1);
    presetBox.setSelectedItemIndex (proc.getCurrentProgram(), juce::dontSendNotification);
    presetBox.onChange = [this] { proc.setCurrentProgram (presetBox.getSelectedItemIndex()); };
    addAndMakeVisible (presetBox);

    prevBtn.onClick = [this]
    {
        const int n = proc.getNumPrograms();
        presetBox.setSelectedItemIndex ((presetBox.getSelectedItemIndex() + n - 1) % n, juce::sendNotification);
    };
    nextBtn.onClick = [this]
    {
        const int n = proc.getNumPrograms();
        presetBox.setSelectedItemIndex ((presetBox.getSelectedItemIndex() + 1) % n, juce::sendNotification);
    };
    addAndMakeVisible (prevBtn);
    addAndMakeVisible (nextBtn);

    setResizable (true, true);
    setResizeLimits (700, 448, 1800, 1152);
    if (auto* c = getConstrainer())
        c->setFixedAspectRatio (1000.0 / 640.0);
    setSize (juce::jlimit (700, 1800, proc.edW), juce::jlimit (448, 1152, proc.edH));

    startTimerHz (8);
}

CosmoEditor::~CosmoEditor()
{
    stopTimer();
    setLookAndFeel (nullptr);
}

void CosmoEditor::timerCallback()
{
    const int cur = proc.getCurrentProgram();
    if (presetBox.getSelectedItemIndex() != cur)
        presetBox.setSelectedItemIndex (cur, juce::dontSendNotification);
}

void CosmoEditor::paint (juce::Graphics& g)
{
    const float s = (float) getWidth() / 1000.0f;

    g.setGradientFill (juce::ColourGradient (juce::Colour (0xff0c0d26), 0.0f, 0.0f,
                                             juce::Colour (0xff05050f), 0.0f, (float) getHeight(), false));
    g.fillAll();

    g.setGradientFill (juce::ColourGradient (juce::Colour (0xff3de0ff), 18.0f * s, 0.0f,
                                             juce::Colour (0xffb44cff), 260.0f * s, 0.0f, false));
    g.setFont (juce::Font (27.0f * s, juce::Font::bold));
    g.drawText ("COSMOSPHERE",
                juce::Rectangle<int> (juce::roundToInt (18.0f * s), juce::roundToInt (5.0f * s),
                                      juce::roundToInt (320.0f * s), juce::roundToInt (32.0f * s)),
                juce::Justification::centredLeft);

    g.setColour (juce::Colour (0xff7f86c8));
    g.setFont (juce::Font (11.5f * s));
    g.drawText ("HYBRID POLY SYNTH  -  ELECTRONIC + ORGANIC",
                juce::Rectangle<int> (juce::roundToInt (20.0f * s), juce::roundToInt (36.0f * s),
                                      juce::roundToInt (420.0f * s), juce::roundToInt (16.0f * s)),
                juce::Justification::centredLeft);
}

void CosmoEditor::resized()
{
    const float s = (float) getWidth() / 1000.0f;
    lnf.scale = s;
    proc.edW = getWidth();
    proc.edH = getHeight();

    auto area = getLocalBounds();
    const int m = juce::roundToInt (10.0f * s);

    auto header = area.removeFromTop (juce::roundToInt (56.0f * s));
    auto pr = header.removeFromRight (juce::roundToInt (400.0f * s)).reduced (m, juce::roundToInt (13.0f * s));
    prevBtn.setBounds (pr.removeFromLeft (juce::roundToInt (34.0f * s)));
    nextBtn.setBounds (pr.removeFromRight (juce::roundToInt (34.0f * s)));
    presetBox.setBounds (pr.reduced (juce::roundToInt (4.0f * s), 0));

    auto body = area.reduced (m, 0).withTrimmedBottom (m);
    const int gap = m;
    const int usable = body.getWidth() - 2 * gap;
    auto left = body.removeFromLeft (usable * 30 / 100);
    body.removeFromLeft (gap);
    auto right = body.removeFromRight (usable * 30 / 100);
    body.removeFromRight (gap);
    auto centre = body;

    const int lh = left.getHeight();
    oscPanel.setBounds (left.removeFromTop (lh * 56 / 100));
    left.removeFromTop (gap);
    filterPanel.setBounds (left);

    const int ch = centre.getHeight();
    sphere.setBounds (centre.removeFromTop (ch * 46 / 100));
    centre.removeFromTop (gap);
    ampPanel.setBounds (centre.removeFromTop ((centre.getHeight() - gap) / 2));
    centre.removeFromTop (gap);
    fltPanel.setBounds (centre);

    const int rh = right.getHeight();
    lfoPanel.setBounds (right.removeFromTop (rh * 36 / 100));
    right.removeFromTop (gap);
    fxPanel.setBounds (right);
}
