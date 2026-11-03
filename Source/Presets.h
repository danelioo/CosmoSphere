#pragma once
#include <vector>

// Cada preset e uma lista "id=valor". Parametros nao citados voltam ao padrao.
// Ondas: 0 Sine, 1 Saw, 2 Square, 3 Triangle, 4 Organ, 5 Noise
// Filtro: 0 LP, 1 BP, 2 HP | LFO shape: 0 Sine, 1 Tri, 2 Saw, 3 Random
// LFO dest: 0 Pitch, 1 Cutoff, 2 Amp, 3 FM, 4 PWM
struct Preset { const char* name; const char* values; };

inline const std::vector<Preset>& getPresets()
{
    static const std::vector<Preset> list =
    {
        { "Cosmic Pad",       "o1w=1 o2w=1 o2f=12 omix=0.5 sub=0.2 cut=1800 res=0.15 fam=0.25 aA=1.2 aD=1.5 aS=0.8 aR=2.5 fA=1.5 fD=2 fS=0.5 fR=2 lrt=0.3 ldp=0.25 ldst=1 chr=0.5 dlm=0.2 dlt=0.45 dlf=0.4 rsz=0.8 rmx=0.45" },
        { "Neon Bass",        "o1w=1 o2w=2 o2f=6 omix=0.4 sub=0.6 cut=500 res=0.35 fam=0.6 aA=0.003 aD=0.25 aS=0.8 aR=0.1 fA=0.003 fD=0.22 fS=0.1 fR=0.1 drv=0.35 rmx=0.05" },
        { "Glass Bell",       "o1w=0 o2w=0 o2s=19 omix=0.15 fm=0.55 cut=12000 res=0 fam=0 aA=0.002 aD=2.2 aS=0 aR=2 dlm=0.15 dlt=0.4 dlf=0.35 rsz=0.85 rmx=0.4" },
        { "Pluck Pop",        "o1w=1 o2w=2 o2s=12 omix=0.3 cut=900 res=0.25 fam=0.7 aA=0.002 aD=0.35 aS=0 aR=0.25 fA=0.002 fD=0.18 fS=0 fR=0.2 dlm=0.25 dlt=0.28 dlf=0.35 rmx=0.2" },
        { "Warm Strings",     "o1w=1 o2w=1 o2f=14 omix=0.5 cut=3500 res=0.1 fam=0.15 aA=0.5 aD=0.6 aS=0.85 aR=1 ldst=0 lrt=5.5 ldp=0.1 chr=0.6 rmx=0.35" },
        { "Breathy Flute",    "o1w=0 o2w=3 o2s=12 omix=0.25 nse=0.2 cut=4500 res=0.25 fam=0.2 aA=0.12 aD=0.3 aS=0.8 aR=0.3 ldst=0 lrt=5 ldp=0.12 rmx=0.35" },
        { "Wood Marimba",     "o1w=0 o2w=0 o2s=24 omix=0.18 fm=0.1 cut=6000 res=0 fam=0 aA=0.001 aD=0.5 aS=0 aR=0.3 rmx=0.2" },
        { "Tonewheel Organ",  "o1w=4 o2w=4 o2s=12 omix=0.4 sub=0.3 cut=9000 res=0.05 fam=0 aA=0.005 aD=0.1 aS=1 aR=0.08 ldst=2 lrt=6 ldp=0.15 chr=0.5 drv=0.2 rmx=0.15" },
        { "Acid Lead",        "o1w=1 o2w=2 o2f=10 omix=0.3 cut=700 res=0.75 fam=0.8 aA=0.002 aD=0.2 aS=0.7 aR=0.15 fA=0.002 fD=0.25 fS=0.15 fR=0.15 drv=0.5 dlm=0.3 dlt=0.3 dlf=0.45 rmx=0.1" },
        { "Super Lead",       "o1w=1 o2w=1 o2f=22 omix=0.6 cut=5000 res=0.2 fam=0.3 aA=0.01 aD=0.2 aS=0.8 aR=0.3 ldst=0 lrt=5.5 ldp=0.08 chr=0.4 dlm=0.3 dlt=0.375 dlf=0.4 rmx=0.25" },
        { "Deep Space Drone", "o1w=1 o2w=0 o2s=-12 o2f=3 omix=0.5 sub=0.5 fm=0.15 cut=700 res=0.4 fam=0.2 aA=3 aD=3 aS=1 aR=5 ldst=1 lrt=0.12 ldp=0.6 lsh=1 chr=0.3 dlm=0.35 dlt=0.7 dlf=0.6 rsz=0.95 rmx=0.8" },
        { "Soft Keys",        "o1w=0 o2w=0 o2s=12 omix=0.1 fm=0.3 cut=7000 fam=0 aA=0.002 aD=1.4 aS=0.15 aR=0.5 chr=0.5 rmx=0.25" },
        { "Wind Texture",     "o1w=5 o2w=5 omix=0.5 nse=0.6 ftp=1 cut=1200 res=0.6 fam=0.3 aA=1.5 aD=1 aS=1 aR=2 ldst=1 lrt=0.25 ldp=0.7 lsh=1 rsz=0.9 rmx=0.5" },
        { "Kalimba Pluck",    "o1w=3 o2w=0 o2s=19 omix=0.15 fm=0.2 cut=5000 res=0 fam=0.3 aA=0.001 aD=0.8 aS=0 aR=0.5 fA=0.001 fD=0.15 fS=0 dlm=0.3 dlt=0.3 dlf=0.35 rmx=0.3" },
        { "Wobble Bass",      "o1w=1 o2w=1 o2s=-12 o2f=10 omix=0.5 sub=0.4 cut=450 res=0.5 fam=0.2 aA=0.005 aD=0.3 aS=0.9 aR=0.12 ldst=1 lrt=3 ldp=0.75 lsh=1 drv=0.4 chr=0.15 rmx=0.05" },
        { "Choir Aah",        "o1w=1 o2w=3 o2s=12 omix=0.4 nse=0.06 cut=1400 res=0.45 fam=0.1 aA=0.7 aD=0.5 aS=0.9 aR=1.2 ldst=0 lrt=5 ldp=0.08 chr=0.5 rsz=0.8 rmx=0.5" },
        { "Init Saw",         "" }
    };
    return list;
}
