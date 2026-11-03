#pragma once
#include "PluginProcessor.h"

namespace ui
{
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

class CosmoLnF : public juce::LookAndFeel_V4
{
public:
    float scale = 1.0f;

    CosmoLnF()
    {
        setColour (juce::ResizableWindow::backgroundColourId, juce::Colour (0xff070812));
        setColour (juce::ComboBox::backgroundColourId, juce::Colour (0xff1a1c3a));
        setColour (juce::ComboBox::textColourId, juce::Colour (0xffe6e9ff));
        setColour (juce::ComboBox::outlineColourId, juce::Colour (0xff3a3e80));
        setColour (juce::ComboBox::arrowColourId, juce::Colour (0xff3de0ff));
        setColour (juce::PopupMenu::backgroundColourId, juce::Colour (0xff14162f));
        setColour (juce::PopupMenu::textColourId, juce::Colour (0xffe6e9ff));
        setColour (juce::PopupMenu::highlightedBackgroundColourId, juce::Colour (0xff3a3e9a));
        setColour (juce::PopupMenu::highlightedTextColourId, juce::Colours::white);
        setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffe6e9ff));
        setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        setColour (juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
        setColour (juce::TextButton::buttonColourId, juce::Colour (0xff1a1c3a));
        setColour (juce::TextButton::textColourOffId, juce::Colour (0xff3de0ff));
        setColour (juce::TextButton::textColourOnId, juce::Colour (0xff3de0ff));
    }

    juce::Font getLabelFont (juce::Label&) override           { return juce::Font (12.0f * scale); }
    juce::Font getComboBoxFont (juce::ComboBox&) override     { return juce::Font (13.0f * scale); }
    juce::Font getPopupMenuFont() override                    { return juce::Font (14.0f * scale); }
    juce::Font getTextButtonFont (juce::TextButton&, int) override { return juce::Font (16.0f * scale, juce::Font::bold); }

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float pos,
                           float startAngle, float endAngle, juce::Slider&) override
    {
        auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) width, (float) height).reduced (4.0f * scale);
        const float radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
        const auto c = bounds.getCentre();
        const float arcR = radius - 2.0f * scale;
        const float thick = 3.5f * scale;

        juce::Path track;
        track.addCentredArc (c.x, c.y, arcR, arcR, 0.0f, startAngle, endAngle, true);
        g.setColour (juce::Colour (0xff24264a));
        g.strokePath (track, juce::PathStrokeType (thick, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        const float ang = startAngle + pos * (endAngle - startAngle);
        juce::Path val;
        val.addCentredArc (c.x, c.y, arcR, arcR, 0.0f, startAngle, ang, true);
        g.setGradientFill (juce::ColourGradient (juce::Colour (0xff3de0ff), c.x - arcR, c.y,
                                                 juce::Colour (0xffb44cff), c.x + arcR, c.y, false));
        g.strokePath (val, juce::PathStrokeType (thick, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        const float bodyR = radius - 7.0f * scale;
        if (bodyR > 2.0f)
        {
            g.setGradientFill (juce::ColourGradient (juce::Colour (0xff3a3d6b), c.x - bodyR * 0.4f, c.y - bodyR * 0.5f,
                                                     juce::Colour (0xff15162e), c.x + bodyR, c.y + bodyR, true));
            g.fillEllipse (c.x - bodyR, c.y - bodyR, bodyR * 2.0f, bodyR * 2.0f);

            juce::Path ptr;
            ptr.addRoundedRectangle (-1.6f * scale, -bodyR + 2.0f * scale, 3.2f * scale, bodyR * 0.45f, 1.5f);
            g.setColour (juce::Colours::white);
            g.fillPath (ptr, juce::AffineTransform::rotation (ang).translated (c.x, c.y));
        }
    }
};

inline float scaleOf (juce::Component& c)
{
    if (auto* l = dynamic_cast<CosmoLnF*> (&c.getLookAndFeel())) return l->scale;
    return 1.0f;
}

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
        g.setColour (juce::Colour (0xff9aa0d8));
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
            box.addItemList (p->choices, 1);
        addAndMakeVisible (box);
        attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, paramID, box);
    }

    void paint (juce::Graphics& g) override
    {
        g.setColour (juce::Colour (0xff9aa0d8));
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
        const float s = scaleOf (*this);
        auto b = getLocalBounds().toFloat().reduced (1.0f);
        g.setGradientFill (juce::ColourGradient (juce::Colour (0xff16183a), 0.0f, 0.0f,
                                                 juce::Colour (0xff0d0e24), 0.0f, b.getBottom(), false));
        g.fillRoundedRectangle (b, 10.0f * s);
        g.setColour (juce::Colour (0xff2b2e63));
        g.drawRoundedRectangle (b, 10.0f * s, 1.2f);
        g.setColour (juce::Colour (0xff3de0ff));
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

class SphereView : public juce::Component, private juce::Timer
{
public:
    explicit SphereView (CosmoProcessor& p) : proc (p) { startTimerHz (30); }
    ~SphereView() override { stopTimer(); }

    void paint (juce::Graphics& g) override
    {
        const float s = scaleOf (*this);
        auto b = getLocalBounds().toFloat();

        juce::Path clip;
        clip.addRoundedRectangle (b.reduced (1.0f), 12.0f * s);
        g.saveState();
        g.reduceClipRegion (clip, juce::AffineTransform());

        g.setGradientFill (juce::ColourGradient (juce::Colour (0xff0e1034), b.getCentreX(), b.getCentreY(),
                                                 juce::Colour (0xff04040f), b.getX(), b.getY(), true));
        g.fillRect (b);

        const float lvl = juce::jlimit (0.0f, 1.0f, level * 2.5f);
        const float cx = b.getCentreX(), cy = b.getCentreY();
        const float R = juce::jmin (b.getWidth(), b.getHeight()) * 0.28f * (1.0f + lvl * 0.10f);

        // estrelas
        for (int i = 0; i < 70; ++i)
        {
            const float sx = b.getX() + cosmo::fract ((float) i * 0.7548777f) * b.getWidth();
            const float sy = b.getY() + cosmo::fract ((float) i * 0.5698403f) * b.getHeight();
            const float a = 0.12f + 0.25f * (0.5f + 0.5f * std::sin (phase * 2.0f + (float) i));
            g.setColour (juce::Colours::white.withAlpha (a));
            const float r = (1.0f + (float) (i % 3) * 0.5f) * s;
            g.fillEllipse (sx - r, sy - r, r * 2.0f, r * 2.0f);
        }

        // brilho
        const auto glow = juce::Colour (0xff3de0ff).interpolatedWith (juce::Colour (0xffb44cff),
                                                                       0.5f + 0.5f * std::sin (phase * 0.5f));
        for (int k = 7; k >= 0; --k)
        {
            const float rr = R * (1.08f + 0.13f * (float) k * (1.0f + lvl));
            g.setColour (glow.withAlpha (0.045f * (1.0f - (float) k / 9.0f)));
            g.fillEllipse (cx - rr, cy - rr, rr * 2.0f, rr * 2.0f);
        }

        // aneis orbitais (parte de tras)
        drawOrbits (g, cx, cy, R, s, false);

        // corpo da esfera
        juce::ColourGradient body (juce::Colour (0xffc8f6ff), cx - R * 0.35f, cy - R * 0.40f,
                                   juce::Colour (0xff1b1050), cx + R * 0.75f, cy + R * 0.95f, true);
        body.addColour (0.35, juce::Colour (0xff3a7bd5).interpolatedWith (juce::Colour (0xff8a4cff), lvl));
        g.setGradientFill (body);
        g.fillEllipse (cx - R, cy - R, R * 2.0f, R * 2.0f);

        // meridianos e paralelos
        g.setColour (juce::Colours::white.withAlpha (0.16f));
        for (int k = 0; k < 4; ++k)
        {
            const float w = R * std::abs (std::cos (phase + (float) k * cosmo::kPi * 0.25f));
            g.drawEllipse (cx - w, cy - R, w * 2.0f, R * 2.0f, 1.0f * s);
        }
        const float lat[] = { -0.6f, -0.3f, 0.0f, 0.3f, 0.6f };
        for (float t : lat)
        {
            const float half = R * std::sqrt (1.0f - t * t);
            const float yy = cy + t * R;
            g.drawEllipse (cx - half, yy - half * 0.16f, half * 2.0f, half * 0.32f, 1.0f * s);
        }

        // aneis orbitais (frente) + particulas
        drawOrbits (g, cx, cy, R, s, true);

        g.setColour (juce::Colour (0xff7f86c8));
        g.setFont (juce::Font (11.0f * s, juce::Font::bold));
        g.drawText ("VOICES " + juce::String (voices) + " / 8",
                    getLocalBounds().reduced (juce::roundToInt (10.0f * s)),
                    juce::Justification::bottomRight);
        g.restoreState();

        g.setColour (juce::Colour (0xff2b2e63));
        g.drawRoundedRectangle (b.reduced (1.0f), 12.0f * s, 1.2f);
    }

private:
    void drawOrbits (juce::Graphics& g, float cx, float cy, float R, float s, bool front)
    {
        const float tilt = 0.5f + lfo * 0.6f;
        const float rots[2] = { tilt, -tilt * 0.8f + 0.4f };
        const float rxs[2]  = { R * 1.75f, R * 1.45f };
        const float rys[2]  = { R * 0.45f, R * 0.60f };
        const float spd[2]  = { 1.0f, 1.25f };

        if (! front)
        {
            for (int ring = 0; ring < 2; ++ring)
            {
                juce::Path p;
                p.addEllipse (cx - rxs[ring], cy - rys[ring], rxs[ring] * 2.0f, rys[ring] * 2.0f);
                g.setColour (juce::Colour (0xff3de0ff).withAlpha (0.28f));
                g.strokePath (p, juce::PathStrokeType (1.2f * s), juce::AffineTransform::rotation (rots[ring], cx, cy));
            }
            return;
        }

        const int nShow = juce::jmax (3, voices);
        for (int i = 0; i < nShow; ++i)
        {
            const int ring = i % 2;
            const float a = phase * spd[ring] * (ring == 0 ? 1.0f : -1.0f) + (float) i * cosmo::kTwoPi / (float) nShow;
            const float lx = rxs[ring] * std::cos (a);
            const float ly = rys[ring] * std::sin (a);
            const float cr = std::cos (rots[ring]), sr = std::sin (rots[ring]);
            const float px = cx + lx * cr - ly * sr;
            const float py = cy + lx * sr + ly * cr;
            const float al = voices > 0 ? 1.0f : 0.35f;
            g.setColour (juce::Colour (0xffb44cff).withAlpha (0.25f * al));
            g.fillEllipse (px - 7.0f * s, py - 7.0f * s, 14.0f * s, 14.0f * s);
            g.setColour (juce::Colours::white.withAlpha (0.9f * al));
            g.fillEllipse (px - 3.0f * s, py - 3.0f * s, 6.0f * s, 6.0f * s);
        }
    }

    void timerCallback() override
    {
        level += (proc.visLevel.load() - level) * 0.35f;
        lfo = proc.visLfo.load();
        voices = proc.visVoices.load();
        phase += 0.025f + juce::jmin (1.0f, level * 2.5f) * 0.06f;
        if (phase > 40.0f * cosmo::kPi) phase -= 40.0f * cosmo::kPi;
        repaint();
    }

    CosmoProcessor& proc;
    float level = 0.0f, lfo = 0.0f, phase = 0.0f;
    int voices = 0;
};
} // namespace ui

class CosmoEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    explicit CosmoEditor (CosmoProcessor&);
    ~CosmoEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    CosmoProcessor& proc;
    ui::CosmoLnF lnf;

    // Osciladores
    ui::Choice o1w, o2w;
    ui::Knob omix, o2s, o2f, sub, nse, fm, pw;
    // Filtro
    ui::Choice ftp;
    ui::Knob cut, res, fam;
    // Envelopes
    ui::Knob aA, aD, aS, aR, fA, fD, fS, fR;
    // LFO
    ui::Choice lsh, ldst;
    ui::Knob lrt, ldp;
    // Efeitos
    ui::Knob drv, chr, mst, dlm, dlt, dlf, rmx, rsz;

    ui::Panel oscPanel, filterPanel, ampPanel, fltPanel, lfoPanel, fxPanel;
    ui::SphereView sphere;

    juce::ComboBox presetBox;
    juce::TextButton prevBtn { "<" }, nextBtn { ">" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CosmoEditor)
};
