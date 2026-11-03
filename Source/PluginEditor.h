// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Danilo Pereira
#pragma once
#include "PluginProcessor.h"

#if COSPHERE_HAS_FONT
 #include <BinaryData.h>
#endif

namespace ui
{
constexpr const char* kVersion = "1.0.0";

// ---------------------------------------------------------------- Temas
struct Theme
{
    const char* name;
    juce::uint32 bgTop, bgBot, panelTop, panelBot, border, text, dim, accent, accent2,
                 knobTop, knobBot, track, field, keyWhite, keyBlack;
};

constexpr int kNumThemes = 8;

inline const Theme* themes()
{
    static const Theme t[kNumThemes] =
    {
        { "Cyan",       0xff0b0d17, 0xff05060c, 0xff141826, 0xff0c0f1a, 0xff232a40, 0xffe8f4f8, 0xff8ea0b4, 0xff19e5f7, 0xff0e8fb8,
                        0xff39425a, 0xff141826, 0xff1f2638, 0xff10131f, 0xffe9edf2, 0xff12141c },
        { "Super 8",    0xffefe8d8, 0xffe2d9c4, 0xfff7f2e6, 0xffece4d2, 0xffcfc4aa, 0xff3b362d, 0xff8a8170, 0xffe8533a, 0xff2b8fd6,
                        0xff6a645a, 0xff2f2c27, 0xffd9cfb8, 0xffe9e1cf, 0xfffbf7ee, 0xff2a2620 },
        { "Enterprise", 0xff1d1713, 0xff110d0a, 0xff2b231d, 0xff1c1612, 0xff44372c, 0xffd7f3f1, 0xff8f8378, 0xff23d9d0, 0xff1a8f8a,
                        0xff4d4137, 0xff1b1612, 0xff2e251e, 0xff17120e, 0xffe6e0d4, 0xff1a1410 },
        { "Rose",       0xff170b14, 0xff0b050a, 0xff24121f, 0xff180b14, 0xff40203a, 0xfff6e8f0, 0xffa88a9c, 0xffff4f9a, 0xffb23a8a,
                        0xff4a2c42, 0xff1a0d16, 0xff2d1727, 0xff12080f, 0xfff3e9ef, 0xff1a0d16 },
        { "Lime",       0xff0c120a, 0xff060a05, 0xff161f12, 0xff0e150b, 0xff2a3a22, 0xffeef8e6, 0xff8fa383, 0xffb6ff3c, 0xff5fd12a,
                        0xff384a2e, 0xff111a0d, 0xff1d2916, 0xff0a1008, 0xffeef3e8, 0xff10170c },
        { "Amber",      0xff14100a, 0xff0a0805, 0xff211a10, 0xff15100a, 0xff3d3220, 0xfffff0d8, 0xffa89878, 0xffffb020, 0xffe0701a,
                        0xff4b3e2a, 0xff181208, 0xff2b2214, 0xff100b06, 0xfff4ecdc, 0xff17110a },
        { "Violet",     0xff0f0b1c, 0xff070510, 0xff1a1530, 0xff100c20, 0xff30284f, 0xffeee8ff, 0xff9a90b8, 0xffa56bff, 0xff5b6bff,
                        0xff3f3666, 0xff130f26, 0xff241d42, 0xff0c0918, 0xffeeeaf6, 0xff130f20 },
        { "Vapor",      0xff1a0b2e, 0xff0a0518, 0xff2a1450, 0xff1b0c36, 0xff4b2a80, 0xfff3e8ff, 0xffa890d0, 0xffff71ce, 0xff01cdfe,
                        0xff5a3a8f, 0xff1d0f3c, 0xff341a63, 0xff150a2a, 0xfff6ecff, 0xff1b0d36 }
    };
    return t;
}

// ---------------------------------------------------------------- Formatacao de valores
enum class Fmt { Pct, Bipolar, Hz, Sec, Cents, Semis, Plain };

inline juce::String formatValue (Fmt f, double v)
{
    switch (f)
    {
        case Fmt::Pct:
            return juce::String (juce::roundToInt (v * 100.0)) + "%";
        case Fmt::Bipolar:
        {
            const int x = juce::roundToInt (v * 100.0);
            return juce::String (x > 0 ? "+" : "") + juce::String (x) + "%";
        }
        case Fmt::Hz:
            if (v >= 1000.0) return juce::String (v / 1000.0, 1) + " kHz";
            if (v >= 100.0)  return juce::String (juce::roundToInt (v)) + " Hz";
            return juce::String (v, 1) + " Hz";
        case Fmt::Sec:
            if (v <= 0.0005) return "off";
            if (v < 1.0) return juce::String (juce::roundToInt (v * 1000.0)) + " ms";
            return juce::String (v, 2) + " s";
        case Fmt::Cents:
        {
            const int x = juce::roundToInt (v);
            return juce::String (x > 0 ? "+" : "") + juce::String (x) + " ct";
        }
        case Fmt::Semis:
        {
            const int x = juce::roundToInt (v);
            return juce::String (x > 0 ? "+" : "") + juce::String (x) + " st";
        }
        default:
            return juce::String (v, 2);
    }
}

// ---------------------------------------------------------------- Look and feel
class CosphereLnF : public juce::LookAndFeel_V4
{
public:
    float scale = 1.0f;
    const Theme* theme = &themes()[0];

    CosphereLnF()
    {
       #if JUCE_WINDOWS
        setDefaultSansSerifTypefaceName ("Segoe UI");
       #elif JUCE_MAC
        setDefaultSansSerifTypefaceName ("Helvetica Neue");
       #endif

       #if COSPHERE_HAS_FONT
        titleFace = juce::Typeface::createSystemTypefaceFor (BinaryData::Redaction50_ttf,
                                                             (size_t) BinaryData::Redaction50_ttfSize);
       #endif
        applyTheme (themes()[0]);
    }

    juce::Font titleFont (float h) const
    {
        if (titleFace != nullptr) return juce::Font (titleFace).withHeight (h);
        return juce::Font (h, juce::Font::bold);
    }

    void applyTheme (const Theme& t)
    {
        theme = &t;
        setColour (juce::ResizableWindow::backgroundColourId, juce::Colour (t.bgBot));
        setColour (juce::ComboBox::backgroundColourId, juce::Colour (t.field));
        setColour (juce::ComboBox::textColourId, juce::Colour (t.text));
        setColour (juce::ComboBox::outlineColourId, juce::Colour (t.border));
        setColour (juce::ComboBox::arrowColourId, juce::Colour (t.accent));
        setColour (juce::PopupMenu::backgroundColourId, juce::Colour (t.panelTop));
        setColour (juce::PopupMenu::textColourId, juce::Colour (t.text));
        setColour (juce::PopupMenu::headerTextColourId, juce::Colour (t.accent));
        setColour (juce::PopupMenu::highlightedBackgroundColourId, juce::Colour (t.accent));
        setColour (juce::PopupMenu::highlightedTextColourId, juce::Colour (t.bgBot));
        setColour (juce::Slider::textBoxTextColourId, juce::Colour (t.text));
        setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        setColour (juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
        setColour (juce::TextButton::buttonColourId, juce::Colour (t.field));
        setColour (juce::TextButton::textColourOffId, juce::Colour (t.accent));
        setColour (juce::TextButton::textColourOnId, juce::Colour (t.accent));
    }

    juce::Font getLabelFont (juce::Label&) override           { return juce::Font (12.0f * scale); }
    juce::Font getComboBoxFont (juce::ComboBox&) override     { return juce::Font (13.0f * scale); }
    juce::Font getPopupMenuFont() override                    { return juce::Font (14.0f * scale); }
    juce::Font getTextButtonFont (juce::TextButton&, int) override { return juce::Font (16.0f * scale, juce::Font::bold); }

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float pos,
                           float startAngle, float endAngle, juce::Slider&) override
    {
        const Theme& t = *theme;
        auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) width, (float) height).reduced (4.0f * scale);
        const float radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
        const auto c = bounds.getCentre();
        const float arcR = radius - 2.0f * scale;
        const float thick = 3.5f * scale;

        juce::Path track;
        track.addCentredArc (c.x, c.y, arcR, arcR, 0.0f, startAngle, endAngle, true);
        g.setColour (juce::Colour (t.track));
        g.strokePath (track, juce::PathStrokeType (thick, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        const float ang = startAngle + pos * (endAngle - startAngle);
        juce::Path val;
        val.addCentredArc (c.x, c.y, arcR, arcR, 0.0f, startAngle, ang, true);
        g.setGradientFill (juce::ColourGradient (juce::Colour (t.accent), c.x - arcR, c.y,
                                                 juce::Colour (t.accent2), c.x + arcR, c.y, false));
        g.strokePath (val, juce::PathStrokeType (thick, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        const float bodyR = radius - 7.0f * scale;
        if (bodyR > 2.0f)
        {
            g.setGradientFill (juce::ColourGradient (juce::Colour (t.knobTop), c.x - bodyR * 0.4f, c.y - bodyR * 0.5f,
                                                     juce::Colour (t.knobBot), c.x + bodyR, c.y + bodyR, true));
            g.fillEllipse (c.x - bodyR, c.y - bodyR, bodyR * 2.0f, bodyR * 2.0f);

            juce::Path ptr;
            ptr.addRoundedRectangle (-1.6f * scale, -bodyR + 2.0f * scale, 3.2f * scale, bodyR * 0.45f, 1.5f);
            g.setColour (juce::Colour (t.accent).brighter (0.5f));
            g.fillPath (ptr, juce::AffineTransform::rotation (ang).translated (c.x, c.y));
        }
    }

private:
    juce::Typeface::Ptr titleFace;
};

inline float scaleOf (juce::Component& c)
{
    if (auto* l = dynamic_cast<CosphereLnF*> (&c.getLookAndFeel())) return l->scale;
    return 1.0f;
}

inline const Theme& themeOf (juce::Component& c)
{
    if (auto* l = dynamic_cast<CosphereLnF*> (&c.getLookAndFeel())) return *l->theme;
    return themes()[0];
}

// ---------------------------------------------------------------- Controles
class Knob : public juce::Component
{
public:
    Knob (juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID,
          const juce::String& labelText, Fmt f)
        : label (labelText)
    {
        slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 60, 16);
        slider.setRotaryParameters (juce::MathConstants<float>::pi * 1.2f,
                                    juce::MathConstants<float>::pi * 2.8f, true);
        addAndMakeVisible (slider);
        attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, paramID, slider);
        slider.textFromValueFunction = [f] (double v) { return formatValue (f, v); };
        slider.valueFromTextFunction = [] (const juce::String& t) { return t.getDoubleValue(); };
        slider.updateText();
    }

    void paint (juce::Graphics& g) override
    {
        g.setColour (juce::Colour (themeOf (*this).dim));
        g.setFont (juce::Font (11.5f * scaleOf (*this), juce::Font::bold));
        g.drawText (label, nameArea, juce::Justification::centred);
    }

    void resized() override
    {
        const float s = scaleOf (*this);
        auto r = getLocalBounds();
        nameArea = r.removeFromTop (juce::roundToInt (15.0f * s));
        slider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, juce::jmax (10, r.getWidth()),
                                juce::roundToInt (15.0f * s));
        slider.setBounds (r);
    }

private:
    juce::String label;
    juce::Rectangle<int> nameArea;
    juce::Slider slider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};

class Choice : public juce::Component
{
public:
    Choice (juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID, const juce::String& labelText)
        : label (labelText)
    {
        if (auto* p = dynamic_cast<juce::AudioParameterChoice*> (apvts.getParameter (paramID)))
        {
            if (paramID == "ftp") // Short labels without changing choice indexes or saved presets
                box.addItemList ({ "LP", "BP", "HP" }, 1);
            else
                box.addItemList (p->choices, 1);
        }
        addAndMakeVisible (box);
        attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, paramID, box);
    }

    void paint (juce::Graphics& g) override
    {
        g.setColour (juce::Colour (themeOf (*this).dim));
        g.setFont (juce::Font (11.5f * scaleOf (*this), juce::Font::bold));
        g.drawText (label, nameArea, juce::Justification::centred);
    }

    void resized() override
    {
        const float s = scaleOf (*this);
        auto r = getLocalBounds();
        nameArea = r.removeFromTop (juce::roundToInt (15.0f * s));
        box.setBounds (r.withSizeKeepingCentre (juce::jmax (10, r.getWidth() - 6), juce::roundToInt (26.0f * s)));
    }

private:
    juce::String label;
    juce::Rectangle<int> nameArea;
    juce::ComboBox box;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> attachment;
};

class Panel : public juce::Component
{
public:
    Panel (const juce::String& t, int numCols) : title (t), cols (numCols) {}

    void add (juce::Component& c)
    {
        items.push_back (&c);
        addAndMakeVisible (c);
    }

    void paint (juce::Graphics& g) override
    {
        const Theme& t = themeOf (*this);
        const float s = scaleOf (*this);
        auto b = getLocalBounds().toFloat().reduced (1.0f);
        g.setGradientFill (juce::ColourGradient (juce::Colour (t.panelTop), 0.0f, 0.0f,
                                                 juce::Colour (t.panelBot), 0.0f, b.getBottom(), false));
        g.fillRoundedRectangle (b, 10.0f * s);
        g.setColour (juce::Colour (t.border));
        g.drawRoundedRectangle (b, 10.0f * s, 1.2f);
        g.setColour (juce::Colour (t.accent));
        g.setFont (juce::Font (12.5f * s, juce::Font::bold));
        g.drawText (title, titleArea, juce::Justification::centredLeft);
    }

    void resized() override
    {
        const float s = scaleOf (*this);
        auto r = getLocalBounds().reduced (juce::roundToInt (8.0f * s), juce::roundToInt (6.0f * s));
        titleArea = r.removeFromTop (juce::roundToInt (18.0f * s));
        const int n = (int) items.size();
        if (n == 0) return;
        const int rows = (n + cols - 1) / cols;
        const int cw = r.getWidth() / cols;
        const int ch = r.getHeight() / rows;
        for (int i = 0; i < n; ++i)
            items[(size_t) i]->setBounds (r.getX() + (i % cols) * cw, r.getY() + (i / cols) * ch, cw, ch);
    }

private:
    juce::String title;
    int cols;
    juce::Rectangle<int> titleArea;
    std::vector<juce::Component*> items;
};

// Effects have purpose-built sections so Reverb and Size stay together.
class EffectsPanel : public Panel
{
public:
    EffectsPanel() : Panel ("EFFECTS", 3) {}
    void configure (Knob& drive, Knob& crush, Knob& chorus,
                    Knob& delayMix, Knob& time, Knob& feedback,
                    Choice& sync, Choice& division, Knob& reverb, Knob& size)
    {
        controls = { &drive, &crush, &chorus, &delayMix, &time, &feedback,
                     &sync, &division, &reverb, &size };
        for (auto* c : controls) add (*c);
    }
    void paint (juce::Graphics& g) override
    {
        Panel::paint (g);
        auto& t = themeOf (*this);
        const float s = scaleOf (*this);
        g.setColour (juce::Colour (t.accent));
        g.setFont (juce::Font (12.5f*s, juce::Font::bold));
        g.drawText ("EFFECTS", juce::Rectangle<int> (juce::roundToInt(12*s), juce::roundToInt(6*s),
                     getWidth() - juce::roundToInt(24*s), juce::roundToInt(18*s)), juce::Justification::centredLeft);
        g.setColour (juce::Colour (t.dim));
        g.setFont (juce::Font (10.0f * s, juce::Font::bold));
        g.drawText ("DELAY", delayHeader, juce::Justification::centredLeft);
        g.drawText ("REVERB", reverbHeader, juce::Justification::centredLeft);
        g.setColour (juce::Colour (t.border));
        g.fillRect (delayLine);
        g.fillRect (reverbLine);
    }
    void resized() override
    {
        // Only use Panel::paint; Panel::resized would impose a uniform grid.
        if (controls.size() != 10) return;
        const float s = scaleOf (*this);
        auto r = getLocalBounds().reduced (juce::roundToInt (8 * s), juce::roundToInt (6 * s));
        r.removeFromTop (juce::roundToInt (20 * s));
        const int gap = juce::roundToInt (3 * s);
        auto row = [&] (int h) { return r.removeFromTop (h); };
        auto arrange = [&] (juce::Rectangle<int> bounds, int offset, int count)
        {
            const int width = bounds.getWidth() / count;
            for (int i = 0; i < count; ++i)
                controls[(size_t) (offset + i)]->setBounds (bounds.getX() + i * width,
                    bounds.getY(), i == count-1 ? bounds.getRight() - (bounds.getX()+i*width) : width, bounds.getHeight());
        };
        const int available = r.getHeight();
        const int topH = available * 23 / 100;
        const int delayH = available * 23 / 100;
        const int syncH = available * 16 / 100;
        const int labelH = juce::jmax (12, juce::roundToInt (16 * s));
        arrange (row (topH), 0, 3);
        delayHeader = row (labelH);
        delayLine = { delayHeader.getX() + juce::roundToInt (45*s), delayHeader.getCentreY(),
                      juce::jmax (0, delayHeader.getWidth()-juce::roundToInt (48*s)), juce::jmax (1, juce::roundToInt(s)) };
        arrange (row (delayH), 3, 3);
        auto syncRow = row (syncH).reduced (gap, 0);
        controls[6]->setBounds (syncRow.removeFromLeft (syncRow.getWidth()/2));
        controls[7]->setBounds (syncRow);
        reverbHeader = row (labelH);
        reverbLine = { reverbHeader.getX() + juce::roundToInt (55*s), reverbHeader.getCentreY(),
                       juce::jmax (0, reverbHeader.getWidth()-juce::roundToInt (58*s)), juce::jmax (1, juce::roundToInt(s)) };
        auto revRow = r;
        revRow.reduce (juce::roundToInt (15*s), 0);
        arrange (revRow, 8, 2);
    }
private:
    std::vector<juce::Component*> controls;
    juce::Rectangle<int> delayHeader, reverbHeader, delayLine, reverbLine;
};

// ---------------------------------------------------------------- Seletor de tema (bolinhas)
class ThemeDots : public juce::Component
{
public:
    std::function<void (int)> onSelect;
    int selected = 0;

    ThemeDots() { setMouseCursor (juce::MouseCursor::PointingHandCursor); }

    void paint (juce::Graphics& g) override
    {
        const Theme& cur = themeOf (*this);
        const float s = scaleOf (*this);
        auto b = getLocalBounds().toFloat();
        const float cell = b.getWidth() / (float) kNumThemes;
        const float d = juce::jmin (cell * 0.72f, b.getHeight() * 0.5f);
        const float cy = b.getY() + b.getHeight() * 0.38f;

        for (int i = 0; i < kNumThemes; ++i)
        {
            const float cx = b.getX() + cell * ((float) i + 0.5f);
            const bool sel = (i == selected);
            const float dd = sel ? d * 1.12f : d;
            g.setColour (juce::Colour (themes()[i].accent));
            g.fillEllipse (cx - dd * 0.5f, cy - dd * 0.5f, dd, dd);
            if (sel)
            {
                g.setColour (juce::Colour (cur.text));
                g.drawEllipse (cx - dd * 0.5f - 2.5f * s, cy - dd * 0.5f - 2.5f * s, dd + 5.0f * s, dd + 5.0f * s, 1.6f * s);
            }
        }

        g.setColour (juce::Colour (cur.dim));
        g.setFont (juce::Font (10.5f * s, juce::Font::bold));
        g.drawText (juce::String ("THEME  ") + themes()[juce::jlimit (0, kNumThemes - 1, selected)].name,
                    getLocalBounds().removeFromBottom (juce::roundToInt (15.0f * s)),
                    juce::Justification::centred);
    }

    void mouseDown (const juce::MouseEvent& e) override
    {
        const int i = juce::jlimit (0, kNumThemes - 1, (int) (e.position.x / ((float) getWidth() / (float) kNumThemes)));
        if (onSelect) onSelect (i);
    }
};

// ---------------------------------------------------------------- Globo animado
class GlobeView : public juce::Component, private juce::Timer
{
public:
    explicit GlobeView (CosphereProcessor& p) : proc (p)
    {
        for (int i = 0; i <= kSeg; ++i)
        {
            cosU[i] = std::cos (cosphere::kTwoPi * (float) i / (float) kSeg);
            sinU[i] = std::sin (cosphere::kTwoPi * (float) i / (float) kSeg);
        }
        startTimerHz (30);
    }
    ~GlobeView() override { stopTimer(); }

    void paint (juce::Graphics& g) override
    {
        const Theme& t = themeOf (*this);
        const float s = scaleOf (*this);
        auto b = getLocalBounds().toFloat();

        juce::Path clip;
        clip.addRoundedRectangle (b.reduced (1.0f), 12.0f * s);
        g.saveState();
        g.reduceClipRegion (clip, juce::AffineTransform());

        g.setGradientFill (juce::ColourGradient (juce::Colour (t.panelTop), 0.0f, b.getY(),
                                                 juce::Colour (t.panelBot), 0.0f, b.getBottom(), false));
        g.fillRect (b);

        const float lvl = juce::jlimit (0.0f, 1.0f, level * 2.5f);
        cx = b.getCentreX();
        cy = b.getCentreY();
        R = juce::jmin (b.getWidth(), b.getHeight()) * 0.36f * (1.0f + lvl * 0.05f);

        // pontinhos de fundo
        for (int i = 0; i < 60; ++i)
        {
            const float sx = b.getX() + cosphere::fract ((float) i * 0.7548777f) * b.getWidth();
            const float sy = b.getY() + cosphere::fract ((float) i * 0.5698403f) * b.getHeight();
            const float a = 0.10f + 0.20f * (0.5f + 0.5f * std::sin (rot * 3.0f + (float) i));
            g.setColour (juce::Colour (t.dim).withAlpha (a));
            const float r = (0.9f + (float) (i % 3) * 0.5f) * s;
            g.fillEllipse (sx - r, sy - r, r * 2.0f, r * 2.0f);
        }

        // brilho externo
        for (int k = 6; k >= 0; --k)
        {
            const float rr = R * (1.03f + 0.09f * (float) k * (0.6f + lvl));
            g.setColour (juce::Colour (t.accent).withAlpha (0.030f * (1.0f - (float) k / 8.0f)));
            g.fillEllipse (cx - rr, cy - rr, rr * 2.0f, rr * 2.0f);
        }

        // miolo translucido
        g.setGradientFill (juce::ColourGradient (juce::Colour (t.accent).withAlpha (0.16f + 0.10f * lvl), cx - R * 0.3f, cy - R * 0.35f,
                                                 juce::Colour (t.accent).withAlpha (0.02f), cx + R, cy + R, true));
        g.fillEllipse (cx - R, cy - R, R * 2.0f, R * 2.0f);

        // grade: meridianos e paralelos
        cr = std::cos (rot); sr = std::sin (rot);
        const float tilt = 0.42f + lfo * 0.12f;
        ct = std::cos (tilt); st = std::sin (tilt);

        const float lw = 1.1f * s;
        g.setColour (juce::Colour (t.accent));
        for (int m = 0; m < 6; ++m)
        {
            const float lam = cosphere::kPi * (float) m / 6.0f;
            const float sl = std::sin (lam), cl = std::cos (lam);
            float px = 0, py = 0, pd = 0;
            for (int j = 0; j <= kSeg; ++j)
            {
                float x = cosU[j] * sl, y = sinU[j], z = cosU[j] * cl;
                float sx, sy, d;
                project (x, y, z, sx, sy, d);
                if (j > 0) strokeSeg (g, px, py, sx, sy, 0.5f * (pd + d), lw);
                px = sx; py = sy; pd = d;
            }
        }
        const float lats[5] = { -60.0f, -30.0f, 0.0f, 30.0f, 60.0f };
        for (int l = 0; l < 5; ++l)
        {
            const float ph = lats[l] * cosphere::kPi / 180.0f;
            const float cp = std::cos (ph), sp = std::sin (ph);
            const bool eq = (l == 2);
            g.setColour (eq ? juce::Colour (t.accent2).brighter (0.2f) : juce::Colour (t.accent));
            float px = 0, py = 0, pd = 0;
            for (int j = 0; j <= kSeg; ++j)
            {
                float x = cp * cosU[j], y = sp, z = cp * sinU[j];
                float sx, sy, d;
                project (x, y, z, sx, sy, d);
                if (j > 0) strokeSeg (g, px, py, sx, sy, 0.5f * (pd + d), eq ? lw * 1.7f : lw);
                px = sx; py = sy; pd = d;
            }
        }

        // contorno
        g.setColour (juce::Colour (t.accent).withAlpha (0.85f));
        g.drawEllipse (cx - R, cy - R, R * 2.0f, R * 2.0f, 1.6f * s);

        g.setColour (juce::Colour (t.dim));
        g.setFont (juce::Font (11.0f * s, juce::Font::bold));
        g.drawText ("VOICES " + juce::String (voices),
                    getLocalBounds().reduced (juce::roundToInt (10.0f * s)),
                    juce::Justification::bottomRight);
        g.restoreState();

        g.setColour (juce::Colour (t.border));
        g.drawRoundedRectangle (b.reduced (1.0f), 12.0f * s, 1.2f);
    }

private:
    static constexpr int kSeg = 36;

    void project (float x, float y, float z, float& sx, float& sy, float& depth) const
    {
        const float x1 = x * cr + z * sr;
        const float z1 = -x * sr + z * cr;
        const float y2 = y * ct - z1 * st;
        const float z2 = y * st + z1 * ct;
        sx = cx + R * x1;
        sy = cy - R * y2;
        depth = z2;
    }

    static void strokeSeg (juce::Graphics& g, float x0, float y0, float x1, float y1, float depth, float lw)
    {
        const float k = 0.5f * (depth + 1.0f);
        g.setOpacity (0.06f + 0.64f * k * k);
        g.drawLine (x0, y0, x1, y1, lw);
    }

    void timerCallback() override
    {
        level += (proc.visLevel.load() - level) * 0.35f;
        lfo = proc.visLfo.load();
        voices = proc.visVoices.load();

        // gira mais rapido com mais vozes e mais som
        const float target = 0.010f + (float) juce::jmin (voices, 12) * 0.010f + juce::jmin (1.0f, level * 2.5f) * 0.05f;
        speed += (target - speed) * 0.08f;
        rot += speed;
        if (rot > cosphere::kTwoPi) rot -= cosphere::kTwoPi;
        repaint();
    }

    CosphereProcessor& proc;
    float cosU[kSeg + 1] = {}, sinU[kSeg + 1] = {};
    float level = 0.0f, lfo = 0.0f, rot = 0.0f, speed = 0.010f;
    int voices = 0;
    float cx = 0, cy = 0, R = 1, cr = 1, sr = 0, ct = 1, st = 0;
};

// ---------------------------------------------------------------- Preferencias da interface
class OptionsPanel : public juce::Component
{
public:
    ThemeDots themes;
    juce::ComboBox scaleBox;
    juce::TextButton closeBtn { "Done" };
    std::function<void (int)> onScale;
    std::function<void()> onClose;

    OptionsPanel()
    {
        setInterceptsMouseClicks (true, true);
        addAndMakeVisible (themes);
        const int sizes[] = { 70, 80, 90, 100, 110, 125, 140, 160, 180 };
        for (auto size : sizes)
            scaleBox.addItem (juce::String (size) + "%", size);
        scaleBox.onChange = [this] { if (onScale) onScale (scaleBox.getSelectedId()); };
        addAndMakeVisible (scaleBox);
        closeBtn.onClick = [this] { if (onClose) onClose(); };
        addAndMakeVisible (closeBtn);
    }

    void paint (juce::Graphics& g) override
    {
        const auto& t = themeOf (*this);
        const float s = scaleOf (*this);
        g.fillAll (juce::Colour (t.bgBot).withAlpha (0.94f));
        auto card = getLocalBounds().toFloat().withSizeKeepingCentre (460.0f * s, 290.0f * s);
        g.setColour (juce::Colour (t.panelTop));
        g.fillRoundedRectangle (card, 12.0f * s);
        g.setColour (juce::Colour (t.accent));
        g.drawRoundedRectangle (card, 12.0f * s, 1.5f * s);
        auto content = card.toNearestInt().reduced (juce::roundToInt (26*s));
        g.setFont (juce::Font (24.0f * s, juce::Font::bold));
        g.drawText ("Options", content.removeFromTop (juce::roundToInt (45*s)), juce::Justification::centredLeft);
        g.setColour (juce::Colour (t.dim));
        g.setFont (juce::Font (12.0f * s));
        g.drawText ("Appearance and display", content.removeFromTop (juce::roundToInt (25*s)), juce::Justification::centredLeft);
        g.setColour (juce::Colour (t.border));
        g.fillRect (content.removeFromTop (juce::jmax (1, juce::roundToInt(s))));
        g.setColour (juce::Colour (t.text));
        g.setFont (juce::Font (13.0f*s, juce::Font::bold));
        g.drawText ("Window scale", scaleBox.getBounds().translated (0, -juce::roundToInt (24*s)), juce::Justification::centredLeft);
        g.drawText ("Color theme", themes.getBounds().translated (0, -juce::roundToInt (21*s)), juce::Justification::centredLeft);
    }

    void resized() override
    {
        const float s = scaleOf (*this);
        auto card = getLocalBounds().withSizeKeepingCentre (juce::roundToInt (460.0f * s), juce::roundToInt (290.0f * s));
        scaleBox.setBounds (card.getX() + juce::roundToInt (32*s), card.getY() + juce::roundToInt (115*s), juce::roundToInt (160*s), juce::roundToInt (30*s));
        themes.setBounds (card.getX() + juce::roundToInt (32*s), card.getY() + juce::roundToInt (189*s), card.getWidth() - juce::roundToInt (64*s), juce::roundToInt (43*s));
        closeBtn.setBounds (card.getCentreX() - juce::roundToInt (60.0f * s), card.getBottom() - juce::roundToInt (48.0f * s), juce::roundToInt (120.0f * s), juce::roundToInt (30.0f * s));
    }
};

// ---------------------------------------------------------------- Sobre
class AboutPanel : public juce::Component
{
public:
    std::function<void()> onClose;
    juce::HyperlinkButton githubLink { "github.com/danelioo/Cosphere", juce::URL ("https://github.com/danelioo/Cosphere") };
    AboutPanel()
    {
        addAndMakeVisible (githubLink);
        githubLink.setTooltip ("Open Cosphere on GitHub");
    }
    void resized() override
    {
        const float s = scaleOf (*this);
        auto card = getLocalBounds().withSizeKeepingCentre (juce::roundToInt(540*s), juce::roundToInt(330*s));
        githubLink.setBounds (card.getX() + juce::roundToInt(88*s), card.getY() + juce::roundToInt(240*s),
                              card.getWidth() - juce::roundToInt(120*s), juce::roundToInt(30*s));
    }

    void paint (juce::Graphics& g) override
    {
        const Theme& t = themeOf (*this);
        const float s = scaleOf (*this);
        g.fillAll (juce::Colour (t.bgBot).withAlpha (0.88f));

        auto card = getLocalBounds().toFloat().withSizeKeepingCentre (540.0f * s, 330.0f * s);
        g.setGradientFill (juce::ColourGradient (juce::Colour (t.panelTop), 0.0f, card.getY(),
                                                 juce::Colour (t.panelBot), 0.0f, card.getBottom(), false));
        g.fillRoundedRectangle (card, 14.0f * s);
        g.setColour (juce::Colour (t.accent).withAlpha (0.7f));
        g.drawRoundedRectangle (card, 14.0f * s, 1.5f * s);

        auto r = card.toNearestInt().reduced (juce::roundToInt (24.0f * s));
        auto* lnf = dynamic_cast<CosphereLnF*> (&getLookAndFeel());

        g.setColour (juce::Colour (t.accent));
        g.setFont (lnf != nullptr ? lnf->titleFont (64.0f * s) : juce::Font (64.0f * s, juce::Font::bold));
        g.drawText ("Cosphere", r.removeFromTop (juce::roundToInt (76.0f * s)), juce::Justification::centred);

        g.setColour (juce::Colour (t.dim));
        g.setFont (juce::Font (14.0f * s));
        g.drawText (juce::String ("Hybrid polyphonic synthesizer  -  version ") + kVersion,
                    r.removeFromTop (juce::roundToInt (24.0f * s)), juce::Justification::centred);

        r.removeFromTop (juce::roundToInt (16.0f * s));
        g.setColour (juce::Colour (t.text));
        g.setFont (juce::Font (19.0f * s, juce::Font::bold));
        g.drawText ("Created by Danilo Pereira", r.removeFromTop (juce::roundToInt (30.0f * s)), juce::Justification::centred);

        g.setFont (juce::Font (14.0f * s));
        g.drawText ("A homemade project, free to download.", r.removeFromTop (juce::roundToInt (24.0f * s)),
                    juce::Justification::centred);

        r.removeFromTop (juce::roundToInt (14.0f * s));
        g.setColour (juce::Colour (t.dim));
        g.setFont (juce::Font (12.0f * s));
        g.drawText ("Built with JUCE.  Title font: Redaction 50 (SIL Open Font License).",
                    r.removeFromTop (juce::roundToInt (20.0f * s)), juce::Justification::centred);

        // GitHub mark (Octicon silhouette) next to a clickable project link.
        juce::Path githubIcon;
        if (auto svg = juce::XmlDocument::parse (R"SVG(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 16 16"><path d="M8 0C3.58 0 0 3.58 0 8c0 3.54 2.29 6.53 5.47 7.59.4.07.55-.17.55-.38 0-.19-.01-.82-.01-1.49-2.01.37-2.53-.49-2.69-.94-.09-.23-.48-.94-.82-1.13-.28-.15-.68-.52-.01-.53.63-.01 1.08.58 1.23.82.72 1.21 1.87.87 2.33.66.07-.52.28-.87.51-1.07-1.78-.2-3.64-.89-3.64-3.95 0-.87.31-1.59.82-2.15-.08-.2-.36-1.02.08-2.12 0 0 .67-.21 2.2.82a7.65 7.65 0 0 1 2-.27c.68 0 1.36.09 2 .27 1.53-1.04 2.2-.82 2.2-.82.44 1.1.16 1.92.08 2.12.51.56.82 1.27.82 2.15 0 3.07-1.87 3.75-3.65 3.95.29.25.54.73.54 1.48 0 1.07-.01 1.93-.01 2.2 0 .21.15.46.55.38A8.013 8.013 0 0 0 16 8c0-4.42-3.58-8-8-8z"/></svg>)SVG"))
            if (auto drawable = juce::Drawable::createFromSVG (*svg))
            {
                auto iconRect = githubLink.getBounds().removeFromLeft (juce::roundToInt(24*s)).toFloat();
                drawable->drawWithin (g, iconRect, juce::RectanglePlacement::centred, 1.0f);
            }
        g.setColour (juce::Colour (t.dim));
        g.setFont (juce::Font (11.5f * s, juce::Font::bold));
        g.drawText ("CLICK OUTSIDE LINK TO CLOSE", r.removeFromBottom (juce::roundToInt (18.0f * s)), juce::Justification::centred);
    }

    void mouseDown (const juce::MouseEvent&) override
    {
        if (onClose) onClose();
    }
};
} // namespace ui

// ---------------------------------------------------------------- Editor
class CosphereEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    explicit CosphereEditor (CosphereProcessor&);
    ~CosphereEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void applyTheme (int index);

    CosphereProcessor& proc;
    ui::CosphereLnF lnf;
    int currentTheme = -1;
    bool dirty = false;
    bool readyToPersist = false;

    // Osciladores
    ui::Choice o1w, o2w;
    ui::Knob omix, o2s, o2f, sub, nse, fm, pw;
    // Filtro
    ui::Choice ftp;
    ui::Knob cut, res, fam;
    // Vozes
    ui::Choice mode, vox;
    ui::Knob gld;
    // Envelopes
    ui::Knob aA, aD, aS, aR, fA, fD, fS, fR;
    // Modulacao
    ui::Choice lsh, ldst, lsy, ldiv;
    ui::Knob lrt, ldp, penv, pdec;
    // Efeitos
    ui::Knob drv, crsh, chr, dlm, dlt, dlf, rmx, rsz, mst;
    ui::Choice dsy, ddiv;

    ui::Panel oscPanel, filterPanel, voicePanel, ampPanel, fltPanel, lfoPanel;
    ui::EffectsPanel fxPanel;
    ui::GlobeView globe;
    ui::OptionsPanel options;
    ui::AboutPanel about;
    juce::MidiKeyboardComponent keyboard;

    juce::TextButton presetBtn { "Init Saw" };
    void showPresetMenu();
    juce::TextButton prevBtn { "<" }, nextBtn { ">" }, infoBtn { "i" }, optionsBtn { "Options" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CosphereEditor)
};
