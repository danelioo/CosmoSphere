#pragma once
#include "Common.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <vector>

namespace cosphere
{
constexpr float kPi = 3.14159265359f;
constexpr float kTwoPi = 6.28318530718f;
constexpr int kMaxVoices = 32;

// Formas de onda: 0 Sine, 1 Saw, 2 Square, 3 Triangle, 4 Organ, 5 Noise,
// 6 Pulse 25, 7 Chip Tri, 8 Vowel, 9 Reed, 10 Glass, 11 Digital
constexpr int kNumWaves = 12;

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
    float penv = 0.0f, pdec = 0.08f;   // envelope de pitch (semitons) e decaimento (s)
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

// ---------------------------------------------------------------- Wavetables
// Tabelas de ciclo unico com 4 niveis de largura de banda (menos harmonicos nas notas agudas).
class WaveTables
{
public:
    static constexpr int N = 2048;
    static constexpr int kTypes = 6;   // sine, organ, vowel, reed, glass, digital
    static constexpr int kMips = 4;

    static const WaveTables& instance()
    {
        static const WaveTables t;
        return t;
    }

    const float* get (int type, int mip) const noexcept
    {
        return data.data() + ((size_t) type * kMips + (size_t) mip) * (size_t) (N + 2);
    }

    static float read (const float* t, float p) noexcept
    {
        const float x = p * (float) N;
        const int i = (int) x;
        const float f = x - (float) i;
        return t[i] + f * (t[i + 1] - t[i]);
    }

private:
    WaveTables()
    {
        data.assign ((size_t) kTypes * kMips * (size_t) (N + 2), 0.0f);
        static const int hmax[kMips] = { 48, 24, 12, 6 };

        for (int type = 0; type < kTypes; ++type)
        {
            float amp[49] = {};
            float ph[49] = {};
            fillHarmonics (type, amp, ph);

            for (int mip = 0; mip < kMips; ++mip)
            {
                float* t = data.data() + ((size_t) type * kMips + (size_t) mip) * (size_t) (N + 2);
                const int H = (type == 0) ? 1 : hmax[mip];
                float peak = 0.0f;
                for (int i = 0; i < N; ++i)
                {
                    double s = 0.0;
                    for (int h = 1; h <= H; ++h)
                        if (amp[h] != 0.0f)
                            s += (double) amp[h] * std::sin (6.283185307179586 * (double) h * (double) i / (double) N + (double) ph[h]);
                    t[i] = (float) s;
                    peak = std::max (peak, std::abs (t[i]));
                }
                if (peak > 0.0f)
                    for (int i = 0; i < N; ++i) t[i] /= peak;
                t[N] = t[0];
                t[N + 1] = t[1];
            }
        }
    }

    static void fillHarmonics (int type, float* amp, float* ph)
    {
        switch (type)
        {
            case 0: amp[1] = 1.0f; break;
            case 1: // organ (drawbars)
                amp[1] = 1.0f; amp[2] = 0.55f; amp[3] = 0.35f; amp[4] = 0.2f; amp[6] = 0.12f; amp[8] = 0.08f;
                break;
            case 2: // vowel (formantes aproximados)
                for (int h = 1; h <= 48; ++h)
                {
                    const float f = (float) h;
                    const float g1 = std::exp (-std::pow ((f - 5.0f) / 2.2f, 2.0f));
                    const float g2 = std::exp (-std::pow ((f - 9.0f) / 2.8f, 2.0f));
                    const float g3 = std::exp (-std::pow ((f - 16.0f) / 3.0f, 2.0f));
                    amp[h] = (1.0f / std::pow (f, 0.7f)) * (g1 + 0.7f * g2 + 0.25f * g3 + 0.2f);
                }
                amp[1] += 0.5f;
                break;
            case 3: // reed (harmonicos impares)
                for (int h = 1; h <= 48; ++h)
                    amp[h] = (h % 2 == 1) ? 1.0f / std::pow ((float) h, 0.85f) : 0.04f / (float) h;
                break;
            case 4: // glass
                for (int h = 1; h <= 48; ++h)
                    amp[h] = (h % 3 == 0 ? 0.0f : 1.0f) / std::pow ((float) h, 0.9f);
                amp[1] += 0.4f;
                break;
            default: // digital (amplitudes e fases pseudo-aleatorias fixas)
            {
                uint32_t seed = 12345u;
                auto rnd = [&seed]() { seed = seed * 1664525u + 1013904223u; return (float) (seed >> 8) / 16777216.0f; };
                for (int h = 1; h <= 48; ++h)
                {
                    amp[h] = (0.2f + 0.8f * rnd()) / std::pow ((float) h, 0.6f);
                    ph[h] = rnd() * kTwoPi;
                }
                break;
            }
        }
    }

    std::vector<float> data;
};

// ---------------------------------------------------------------- Envelope
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

// ---------------------------------------------------------------- Engine
class Engine
{
public:
    static constexpr int kControl = 32;

    Engine() { tables = &WaveTables::instance(); }

    void prepare (double sampleRate)
    {
        sr = (float) sampleRate;
        tables = &WaveTables::instance();
        reset();
    }

    void reset()
    {
        for (auto& v : voices) v = Voice();
        lfoPhase = 0.0f; shValue = 0.0f; lfoOut = 0.0f;
        cutSm = -1.0f; counter = 0;
        sustain = false; bend = 0.0f; modWheel = 0.0f;
        mPitch = mCutOct = mFm = mPw = 0.0f; mAmp = 1.0f;
        stackCount = 0; lastSemi = -1.0f;
    }

    void setVoicing (int maxV, bool mono, float glideSeconds)
    {
        maxVoices = std::min (std::max (maxV, 1), kMaxVoices);
        monoMode = mono;
        glide = glideSeconds;
    }

    void noteOn (int note, float vel)
    {
        const float semi = (float) note;

        if (monoMode)
        {
            stackRemove (note);
            stackPush (note);
            Voice& v = voices[0];
            if (v.active && ! v.released)           // legato: so troca o pitch
            {
                v.tgt = semi; v.note = note; v.vel = vel;
                lastSemi = semi;
                return;
            }
            startVoice (v, note, vel, (glide > 0.0f && lastSemi >= 0.0f) ? lastSemi : semi);
            lastSemi = semi;
            return;
        }

        Voice* v = nullptr;
        for (int i = 0; i < maxVoices; ++i)
            if (voices[(size_t) i].active && voices[(size_t) i].note == note) { v = &voices[(size_t) i]; break; }
        if (v == nullptr)
            for (int i = 0; i < maxVoices; ++i)
                if (! voices[(size_t) i].active) { v = &voices[(size_t) i]; break; }
        if (v == nullptr)
        {
            uint32_t best = 0xffffffffu;
            for (int i = 0; i < maxVoices; ++i)
                if (voices[(size_t) i].released && voices[(size_t) i].age < best) { best = voices[(size_t) i].age; v = &voices[(size_t) i]; }
            if (v == nullptr)
            {
                best = 0xffffffffu;
                for (int i = 0; i < maxVoices; ++i)
                    if (voices[(size_t) i].age < best) { best = voices[(size_t) i].age; v = &voices[(size_t) i]; }
            }
        }
        if (v == nullptr) return;

        startVoice (*v, note, vel, (glide > 0.0f && lastSemi >= 0.0f) ? lastSemi : semi);
        lastSemi = semi;
    }

    void noteOff (int note)
    {
        if (monoMode)
        {
            stackRemove (note);
            Voice& v = voices[0];
            if (! v.active) return;
            if (stackCount > 0)
            {
                v.tgt = (float) stack[stackCount - 1];
                v.note = stack[stackCount - 1];
                return;
            }
            if (! v.released)
            {
                if (sustain) v.held = true;
                else         releaseVoice (v);
            }
            return;
        }

        for (auto& v : voices)
            if (v.active && v.note == note && ! v.released)
            {
                if (sustain) v.held = true;
                else         releaseVoice (v);
            }
    }

    void allOff()
    {
        stackCount = 0;
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

    // soma o resultado em L/R (devem estar zerados antes)
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
        float cur = 60.0f, tgt = 60.0f;   // pitch atual/alvo em semitons (glide)
        float pe = 0.0f;                  // envelope de pitch
    };

    void startVoice (Voice& v, int note, float vel, float startSemi)
    {
        if (! v.active)
        {
            v.s1 = v.s2 = 0.0f;
            v.amp.v = 0.0f; v.flt.v = 0.0f;
            v.ph1 = v.ph2 = v.phS = 0.0;
            v.cur = startSemi;
        }
        v.tgt = (float) note;
        v.note = note; v.vel = vel;
        v.pe = 1.0f;
        v.active = true; v.released = false; v.held = false;
        v.age = ++counter;
        v.amp.noteOn();
        v.flt.noteOn();
    }

    void releaseVoice (Voice& v)
    {
        v.released = true;
        v.amp.noteOff();
        v.flt.noteOff();
    }

    void stackRemove (int note)
    {
        for (int i = 0; i < stackCount; ++i)
            if (stack[i] == note)
            {
                for (int j = i; j < stackCount - 1; ++j) stack[j] = stack[j + 1];
                --stackCount;
                return;
            }
    }

    void stackPush (int note)
    {
        if (stackCount == 16)
        {
            for (int j = 0; j < 15; ++j) stack[j] = stack[j + 1];
            --stackCount;
        }
        stack[stackCount++] = note;
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

        glideK = (glide > 0.0f) ? 1.0f - std::exp (-3.0f * (float) len / (sr * glide)) : 1.0f;
        peK = std::exp (-(float) len / (sr * std::max (p.pdec, 0.002f)));

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

    const float* tableFor (int type, float dt) const noexcept
    {
        int id = -1;
        switch (type)
        {
            case 0:  id = 0; break;
            case 4:  id = 1; break;
            case 8:  id = 2; break;
            case 9:  id = 3; break;
            case 10: id = 4; break;
            case 11: id = 5; break;
            default: return nullptr;
        }
        int mip = 0;
        if (id != 0)
            mip = (dt * 48.0f < 0.5f) ? 0 : (dt * 24.0f < 0.5f) ? 1 : (dt * 12.0f < 0.5f) ? 2 : 3;
        return tables->get (id, mip);
    }

    float osc (int type, float p, float dt, float pw, const float* tab) noexcept
    {
        switch (type)
        {
            case 1:  return 2.0f * p - 1.0f - polyBlep (p, dt);
            case 2:
            case 6:
            {
                const float w = (type == 6) ? 0.25f : pw;
                float v = p < w ? 1.0f : -1.0f;
                v += polyBlep (p, dt);
                v -= polyBlep (fract (p + 1.0f - w), dt);
                return v;
            }
            case 3:  return 4.0f * std::abs (p - 0.5f) - 1.0f;
            case 5:  return nextNoise();
            case 7:
            {
                const float tri = 4.0f * std::abs (p - 0.5f) - 1.0f;
                return std::floor ((tri + 1.0f) * 7.5f + 0.5f) / 7.5f - 1.0f;
            }
            default: return WaveTables::read (tab, p);
        }
    }

    void renderVoice (Voice& v, float* L, float* R, int len, const SynthParams& p,
                      const EnvCoef& ac, const EnvCoef& fcf)
    {
        v.cur += (v.tgt - v.cur) * glideK;
        v.pe *= peK;
        if (v.pe < 1.0e-5f) v.pe = 0.0f;

        const float semis = v.cur + bend + mPitch + p.penv * v.pe;
        const float f1 = 440.0f * std::exp2 ((semis - 69.0f) / 12.0f);
        const float f2 = f1 * std::exp2 ((p.o2semi + p.o2fine * 0.01f) / 12.0f);
        const float inc1 = std::min (f1 / sr, 0.45f);
        const float inc2 = std::min (f2 / sr, 0.45f);
        const float incS = std::min (f1 * 0.5f / sr, 0.45f);
        const float pw = std::min (0.95f, std::max (0.05f, p.pw + mPw));
        const float fm = std::max (0.0f, p.fm + mFm);
        const float* tab1 = tableFor (p.o1w, inc1);
        const float* tab2 = tableFor (p.o2w, inc2);
        const float* sineTab = tables->get (0, 0);
        const bool useSub = p.sub > 0.0f, useNoise = p.noise > 0.0f;
        const float mix1 = 1.0f - p.omix, mix2 = p.omix;

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

            const float o2 = (mix2 > 0.0f || fm > 0.0f) ? osc (p.o2w, (float) v.ph2, inc2, pw, tab2) : 0.0f;
            float p1 = (float) v.ph1;
            if (fm > 0.0f) p1 = fract (p1 + fm * 1.2f * o2);
            const float o1 = (mix1 > 0.0f) ? osc (p.o1w, p1, inc1, pw, tab1) : 0.0f;

            float s = o1 * mix1 + o2 * mix2;
            if (useSub)   s += p.sub * 0.7f * WaveTables::read (sineTab, (float) v.phS);
            if (useNoise) s += p.noise * 0.6f * nextNoise();

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

            const float out = y * ae * gain;   // mono: sem pan por nota
            L[i] += out;
            R[i] += out;

            if (v.amp.st == Env::Idle) { v.active = false; break; }
        }
    }

    std::array<Voice, kMaxVoices> voices {};
    const WaveTables* tables = nullptr;
    float sr = 44100.0f;
    uint32_t counter = 0;
    uint32_t rng = 22222u;
    bool sustain = false;
    float bend = 0.0f, modWheel = 0.0f;
    float cutSm = -1.0f;
    float lfoPhase = 0.0f, shValue = 0.0f, lfoOut = 0.0f;
    float mPitch = 0.0f, mCutOct = 0.0f, mFm = 0.0f, mPw = 0.0f, mAmp = 1.0f;
    float glideK = 1.0f, peK = 0.99f;

    int maxVoices = 16;
    bool monoMode = false;
    float glide = 0.0f, lastSemi = -1.0f;
    int stack[16] = {};
    int stackCount = 0;
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

class BitCrusher
{
public:
    void reset() { holdL = holdR = 0.0f; cnt = 0; }

    void process (float* L, float* R, int n, float amt)
    {
        const float bits = 16.0f - amt * 12.0f;
        const float levels = std::exp2 (bits - 1.0f);
        const int factor = 1 + (int) (amt * amt * 15.0f);
        for (int i = 0; i < n; ++i)
        {
            if (cnt <= 0)
            {
                holdL = std::round (L[i] * levels) / levels;
                holdR = std::round (R[i] * levels) / levels;
                cnt = factor;
            }
            --cnt;
            L[i] = holdL;
            R[i] = holdR;
        }
    }

private:
    float holdL = 0.0f, holdR = 0.0f;
    int cnt = 0;
};

} // namespace cosphere
