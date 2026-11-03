#pragma once
#include "Common.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <vector>

namespace cosmo
{
constexpr float kPi = 3.14159265359f;
constexpr float kTwoPi = 6.28318530718f;

struct SynthParams
{
    int   o1w = 1, o2w = 1;
    float o2semi = 0.0f, o2fine = 0.0f, omix = 0.5f, sub = 0.0f, noise = 0.0f, fm = 0.0f, pw = 0.5f;
    int   ftype = 0;
    float cutoff = 8000.0f, res = 0.2f, fenv = 0.3f;
    float aA = 0.01f, aD = 0.3f, aS = 0.7f, aR = 0.3f;
    float fA = 0.01f, fD = 0.4f, fS = 0.3f, fR = 0.3f;
    float lrate = 3.0f, ldepth = 0.0f;
    int   lshape = 0, ldest = 0;
};

inline float softClip (float x) noexcept
{
    if (x >= 3.0f)  return 1.0f;
    if (x <= -3.0f) return -1.0f;
    const float x2 = x * x;
    return x * (27.0f + x2) / (27.0f + 9.0f * x2);
}

inline float polyBlep (float t, float dt) noexcept
{
    if (t < dt)         { t /= dt; return t + t - t * t - 1.0f; }
    if (t > 1.0f - dt)  { t = (t - 1.0f) / dt; return t * t + t + t + 1.0f; }
    return 0.0f;
}

inline float fract (float x) noexcept { return x - std::floor (x); }

struct EnvCoef { float aInc = 0.01f, dK = 0.999f, sus = 0.7f, rK = 0.999f; };

struct Env
{
    enum Stage { Idle, Att, Dec, Sus, Rel };
    Stage st = Idle;
    float v = 0.0f;

    void noteOn() noexcept  { st = Att; }
    void noteOff() noexcept { if (st != Idle) st = Rel; }

    float tick (const EnvCoef& c) noexcept
    {
        switch (st)
        {
            case Att:
                v += c.aInc;
                if (v >= 1.0f) { v = 1.0f; st = Dec; }
                break;
            case Dec:
                v = c.sus + (v - c.sus) * c.dK;
                if (std::abs (v - c.sus) < 1.0e-4f) { v = c.sus; st = Sus; }
                break;
            case Sus:
                v = c.sus;
                break;
            case Rel:
                v *= c.rK;
                if (v < 1.0e-4f) { v = 0.0f; st = Idle; }
                break;
            default:
                break;
        }
        return v;
    }
};

class Engine
{
public:
    static constexpr int kVoices = 8;
    static constexpr int kControl = 32;

    void prepare (double sampleRate)
    {
        sr = (float) sampleRate;
        reset();
    }

    void reset()
    {
        for (auto& v : voices) v = Voice();
        lfoPhase = 0.0f; shValue = 0.0f; lfoOut = 0.0f;
        cutSm = -1.0f; counter = 0;
        sustain = false; bend = 0.0f; modWheel = 0.0f;
        mPitch = mCutOct = mFm = mPw = 0.0f; mAmp = 1.0f;
    }

    void noteOn (int note, float vel)
    {
        Voice* v = nullptr;
        for (auto& x : voices) if (x.active && x.note == note) { v = &x; break; }
        if (v == nullptr) for (auto& x : voices) if (! x.active) { v = &x; break; }
        if (v == nullptr)
        {
            uint32_t best = 0xffffffffu;
            for (auto& x : voices) if (x.released && x.age < best) { best = x.age; v = &x; }
            if (v == nullptr)
            {
                best = 0xffffffffu;
                for (auto& x : voices) if (x.age < best) { best = x.age; v = &x; }
            }
        }
        if (v == nullptr) return;

        if (! v->active)
        {
            v->s1 = v->s2 = 0.0f;
            v->amp.v = 0.0f; v->flt.v = 0.0f;
            v->ph1 = v->ph2 = v->phS = 0.0;
        }
        v->active = true; v->released = false; v->held = false;
        v->note = note; v->vel = vel; v->age = ++counter;
        const float pan = 0.5f + 0.22f * std::sin ((float) note * 1.7f);
        v->panL = 1.4142f * std::cos (pan * 0.5f * kPi);
        v->panR = 1.4142f * std::sin (pan * 0.5f * kPi);
        v->amp.noteOn();
        v->flt.noteOn();
    }

    void noteOff (int note)
    {
        for (auto& v : voices)
            if (v.active && v.note == note && ! v.released)
            {
                if (sustain) v.held = true;
                else         releaseVoice (v);
            }
    }

    void allOff()
    {
        for (auto& v : voices) if (v.active) releaseVoice (v);
    }

    void setSustain (bool on)
    {
        sustain = on;
        if (! on)
            for (auto& v : voices)
                if (v.active && v.held) { v.held = false; releaseVoice (v); }
    }

    void setPitchBend (float semitones) { bend = semitones; }
    void setModWheel (float v01)        { modWheel = v01; }

    int   numActive() const { int n = 0; for (auto& v : voices) if (v.active) ++n; return n; }
    float getLfoOut() const { return lfoOut; }

    // soma o resultado em L/R (que devem estar zerados antes)
    void render (float* L, float* R, int n, const SynthParams& p)
    {
        const EnvCoef ac = makeCoef (p.aA, p.aD, p.aS, p.aR);
        const EnvCoef fc = makeCoef (p.fA, p.fD, p.fS, p.fR);
        if (cutSm < 0.0f) cutSm = p.cutoff;

        int done = 0;
        while (done < n)
        {
            const int len = std::min (kControl, n - done);
            controlStep (p, len);
            for (auto& v : voices)
                if (v.active)
                    renderVoice (v, L + done, R + done, len, p, ac, fc);
            done += len;
        }
    }

private:
    struct Voice
    {
        bool active = false, held = false, released = false;
        int note = 0;
        float vel = 0.0f;
        uint32_t age = 0;
        double ph1 = 0.0, ph2 = 0.0, phS = 0.0;
        Env amp, flt;
        float s1 = 0.0f, s2 = 0.0f;
        float panL = 1.0f, panR = 1.0f;
    };

    void releaseVoice (Voice& v)
    {
        v.released = true;
        v.amp.noteOff();
        v.flt.noteOff();
    }

    EnvCoef makeCoef (float a, float d, float s, float r) const
    {
        EnvCoef c;
        c.aInc = 1.0f / (std::max (a, 0.001f) * sr);
        c.dK   = std::exp (-4.6f / (std::max (d, 0.001f) * sr));
        c.sus  = s;
        c.rK   = std::exp (-6.9f / (std::max (r, 0.003f) * sr));
        return c;
    }

    float nextNoise() noexcept
    {
        rng ^= rng << 13; rng ^= rng >> 17; rng ^= rng << 5;
        return (float) (rng & 0xffffffu) / 8388608.0f - 1.0f;
    }

    void controlStep (const SynthParams& p, int len)
    {
        cutSm += (p.cutoff - cutSm) * 0.2f;

        lfoPhase += p.lrate * (float) len / sr;
        if (lfoPhase >= 1.0f) { lfoPhase -= std::floor (lfoPhase); shValue = nextNoise(); }

        float raw = 0.0f;
        switch (p.lshape)
        {
            case 0:  raw = std::sin (kTwoPi * lfoPhase); break;
            case 1:  raw = 4.0f * std::abs (lfoPhase - 0.5f) - 1.0f; break;
            case 2:  raw = 2.0f * lfoPhase - 1.0f; break;
            default: raw = shValue; break;
        }

        const float depth = std::min (1.0f, std::max (0.0f, p.ldepth + modWheel));
        const float m = raw * depth;
        lfoOut = m;

        mPitch = mCutOct = mFm = mPw = 0.0f;
        mAmp = 1.0f;
        switch (p.ldest)
        {
            case 0:  mPitch  = m * 2.0f; break;
            case 1:  mCutOct = m * 2.5f; break;
            case 2:  mAmp    = 1.0f - depth * (0.5f - 0.5f * raw); break;
            case 3:  mFm     = m * 0.5f; break;
            default: mPw     = m * 0.4f; break;
        }
    }

    float wave (int type, float p, float dt, float pw) noexcept
    {
        switch (type)
        {
            case 0:  return std::sin (kTwoPi * p);
            case 1:  return 2.0f * p - 1.0f - polyBlep (p, dt);
            case 2:
            {
                float v = p < pw ? 1.0f : -1.0f;
                v += polyBlep (p, dt);
                v -= polyBlep (fract (p + 1.0f - pw), dt);
                return v;
            }
            case 3:  return 4.0f * std::abs (p - 0.5f) - 1.0f;
            case 4:
            {
                const float x = kTwoPi * p;
                float r = std::sin (x);
                if (dt * 2.0f < 0.5f) r += 0.55f * std::sin (2.0f * x);
                if (dt * 3.0f < 0.5f) r += 0.35f * std::sin (3.0f * x);
                if (dt * 4.0f < 0.5f) r += 0.20f * std::sin (4.0f * x);
                if (dt * 6.0f < 0.5f) r += 0.12f * std::sin (6.0f * x);
                return r * 0.45f;
            }
            default: return nextNoise();
        }
    }

    void renderVoice (Voice& v, float* L, float* R, int len, const SynthParams& p,
                      const EnvCoef& ac, const EnvCoef& fcf)
    {
        const float semis = (float) v.note + bend + mPitch;
        const float f1 = 440.0f * std::exp2 ((semis - 69.0f) / 12.0f);
        const float f2 = f1 * std::exp2 ((p.o2semi + p.o2fine * 0.01f) / 12.0f);
        const float inc1 = std::min (f1 / sr, 0.45f);
        const float inc2 = std::min (f2 / sr, 0.45f);
        const float incS = std::min (f1 * 0.5f / sr, 0.45f);
        const float pw = std::min (0.95f, std::max (0.05f, p.pw + mPw));
        const float fm = std::max (0.0f, p.fm + mFm);

        float fcut = cutSm * std::exp2 (v.flt.v * p.fenv * 5.0f + mCutOct);
        fcut = std::min (std::max (fcut, 20.0f), std::min (18000.0f, sr * 0.45f));
        const float g  = std::tan (kPi * fcut / sr);
        const float k  = 1.0f / (0.55f + p.res * p.res * 14.0f);
        const float a1 = 1.0f / (1.0f + g * (g + k));
        const float gain = (0.25f + 0.75f * v.vel) * 0.35f * mAmp;

        for (int i = 0; i < len; ++i)
        {
            const float ae = v.amp.tick (ac);
            v.flt.tick (fcf);

            const float o2 = wave (p.o2w, (float) v.ph2, inc2, pw);
            float p1 = (float) v.ph1;
            if (fm > 0.0f) p1 = fract (p1 + fm * 1.2f * o2);
            const float o1 = wave (p.o1w, p1, inc1, pw);

            float s = o1 * (1.0f - p.omix) + o2 * p.omix;
            if (p.sub > 0.0f)   s += p.sub * 0.7f * std::sin (kTwoPi * (float) v.phS);
            if (p.noise > 0.0f) s += p.noise * 0.6f * nextNoise();

            v.ph1 += inc1; if (v.ph1 >= 1.0) v.ph1 -= 1.0;
            v.ph2 += inc2; if (v.ph2 >= 1.0) v.ph2 -= 1.0;
            v.phS += incS; if (v.phS >= 1.0) v.phS -= 1.0;

            const float hp = (s - (g + k) * v.s1 - v.s2) * a1;
            const float bp = g * hp + v.s1;  v.s1 = g * hp + bp;
            const float lp = g * bp + v.s2;  v.s2 = g * bp + lp;

            float y = lp;
            if (p.ftype == 1)      y = bp * k;
            else if (p.ftype == 2) y = hp;
            y = softClip (y);

            const float out = y * ae * gain;
            L[i] += out * v.panL;
            R[i] += out * v.panR;

            if (v.amp.st == Env::Idle) { v.active = false; break; }
        }
    }

    std::array<Voice, kVoices> voices {};
    float sr = 44100.0f;
    uint32_t counter = 0;
    uint32_t rng = 22222u;
    bool sustain = false;
    float bend = 0.0f, modWheel = 0.0f;
    float cutSm = -1.0f;
    float lfoPhase = 0.0f, shValue = 0.0f, lfoOut = 0.0f;
    float mPitch = 0.0f, mCutOct = 0.0f, mFm = 0.0f, mPw = 0.0f, mAmp = 1.0f;
};

// ---------------------------------------------------------------- Efeitos

class StereoDelay
{
public:
    void prepare (double sampleRate)
    {
        sr = (float) sampleRate;
        size = (int) (sr * 2.0f) + 4;
        bl.assign ((size_t) size, 0.0f);
        br.assign ((size_t) size, 0.0f);
        w = 0; dsm = 0.35f * sr; lpL = lpR = 0.0f;
    }

    void process (float* L, float* R, int n, float timeSec, float fb, float mix)
    {
        if (size == 0) return;
        const float target = std::min (timeSec * sr, (float) (size - 4));
        for (int i = 0; i < n; ++i)
        {
            dsm += (target - dsm) * 0.0008f;
            float rp = (float) w - dsm;
            if (rp < 0.0f) rp += (float) size;
            const int i0 = (int) rp;
            const float fr = rp - (float) i0;
            const int i1 = (i0 + 1) % size;
            const float dl = bl[(size_t) i0] + fr * (bl[(size_t) i1] - bl[(size_t) i0]);
            const float dr = br[(size_t) i0] + fr * (br[(size_t) i1] - br[(size_t) i0]);

            lpL += 0.35f * (dl - lpL);
            lpR += 0.35f * (dr - lpR);

            const float inL = L[i], inR = R[i];
            bl[(size_t) w] = inL + fb * lpR;
            br[(size_t) w] = inR + fb * lpL;

            L[i] = inL + mix * dl;
            R[i] = inR + mix * dr;
            if (++w >= size) w = 0;
        }
    }

private:
    std::vector<float> bl, br;
    int size = 0, w = 0;
    float sr = 44100.0f, dsm = 0.0f, lpL = 0.0f, lpR = 0.0f;
};

} // namespace cosmo
