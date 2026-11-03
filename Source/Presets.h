// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Danilo Pereira
#pragma once
#include <vector>

// Cada preset e uma lista "id=valor". Parametros nao citados voltam ao padrao
// (exceto "vox" e "mst", que sao ajustes do usuario e nao mudam com o preset).
//
// Ondas: 0 Sine, 1 Saw, 2 Square, 3 Triangle, 4 Organ, 5 Noise, 6 Pulse 25,
//        7 Chip Tri, 8 Vowel, 9 Reed, 10 Glass, 11 Digital
// Filtro (ftp): 0 LP, 1 BP, 2 HP | LFO shape (lsh): 0 Sine, 1 Tri, 2 Saw, 3 Random
// LFO dest (ldst): 0 Pitch, 1 Cutoff, 2 Amp, 3 FM, 4 PWM
// mode: 0 Poly, 1 Mono | penv: semitons | gld/pdec/aX/fX: segundos
struct Preset { const char* category; const char* name; const char* values; };

inline const std::vector<Preset>& getPresets()
{
    static const std::vector<Preset> list =
    {
        // ------------------------------------------------------------ Init
        { "Init", "Init Saw", "" },
        { "Init", "Init Sine", "o1w=0 o2w=0 omix=0 cut=12000 res=0 fam=0 aS=1 rmx=0" },

        // ------------------------------------------------------------ Trap & Rap
        { "Hip-Hop & Trap", "808 Sub",        "o1w=0 o2w=0 omix=0 o2f=0 cut=1200 res=0 fam=0 aA=0.002 aD=1.8 aS=0 aR=0.3 penv=10 pdec=0.05 mode=1 gld=0.08 drv=0.3 rmx=0" },
        { "Hip-Hop & Trap", "808 Distorted",  "o1w=0 o2w=3 omix=0.2 o2f=0 cut=2500 res=0.1 fam=0 aA=0.002 aD=1.4 aS=0 aR=0.25 penv=14 pdec=0.04 mode=1 gld=0.06 drv=0.8 crsh=0.1 rmx=0" },
        { "Hip-Hop & Trap", "Drill Slide",    "o1w=0 o2w=2 omix=0.15 o2f=0 cut=1500 res=0.2 fam=0.3 aA=0.003 aD=1.2 aS=0 aR=0.3 penv=6 pdec=0.05 mode=1 gld=0.2 drv=0.5 rmx=0.05" },
        { "Hip-Hop & Trap", "Trap Bell",      "o1w=10 o2w=0 o2s=12 omix=0.25 o2f=0 cut=9000 res=0 fam=0 aA=0.001 aD=1.1 aS=0 aR=0.8 dlm=0.25 dlt=0.375 dlf=0.4 rsz=0.7 rmx=0.35 chr=0.2" },
        { "Hip-Hop & Trap", "Dark Pluck",     "o1w=2 o2w=1 o2s=-12 omix=0.4 cut=1200 res=0.3 fam=0.6 aA=0.002 aD=0.4 aS=0 aR=0.2 fD=0.25 fS=0 dlm=0.2 dlt=0.375 rsz=0.6 rmx=0.25" },
        { "Hip-Hop & Trap", "Trap Flute",     "o1w=0 o2w=9 o2s=12 omix=0.3 nse=0.08 cut=4000 res=0.15 fam=0.2 aA=0.05 aD=0.4 aS=0.7 aR=0.4 ldst=0 lrt=5 ldp=0.1 dlm=0.25 dlt=0.375 rmx=0.35" },
        { "Hip-Hop & Trap", "Trap Choir",     "o1w=8 o2w=8 o2s=12 omix=0.3 cut=3000 res=0.2 fam=0.2 aA=0.4 aD=0.8 aS=0.7 aR=1.5 chr=0.5 dlm=0.2 rsz=0.9 rmx=0.55" },

        // ------------------------------------------------------------ Plugg, Rage & Jersey
        { "Hip-Hop & Trap", "Plugg Dream",    "o1w=6 o2w=3 o2s=12 omix=0.35 o2f=6 cut=3500 res=0.2 fam=0.5 aA=0.003 aD=0.7 aS=0 aR=0.9 fD=0.35 fS=0.1 chr=0.7 dlm=0.3 dlt=0.375 dlf=0.45 rsz=0.9 rmx=0.55" },
        { "Hip-Hop & Trap", "Pluggnb Keys",   "o1w=0 o2w=10 o2s=12 omix=0.3 fm=0.2 cut=5000 res=0.1 fam=0.2 aA=0.003 aD=1.5 aS=0.1 aR=1.2 chr=0.6 dlm=0.25 rsz=0.9 rmx=0.6" },
        { "Hip-Hop & Trap", "Jugg Pluck",     "o1w=2 o2w=6 o2s=12 omix=0.4 o2f=5 cut=2800 res=0.35 fam=0.7 aA=0.001 aD=0.22 aS=0 aR=0.12 fD=0.15 fS=0 dlm=0.2 dlt=0.19 dlf=0.3 rmx=0.15" },
        { "Hip-Hop & Trap", "Rage Lead",      "o1w=1 o2w=2 o2f=15 omix=0.5 cut=6000 res=0.15 fam=0.3 aA=0.002 aD=0.3 aS=0.8 aR=0.15 drv=0.85 crsh=0.15 chr=0.3 rmx=0.1" },
        { "Hip-Hop & Trap", "Rage Stab",      "o1w=1 o2w=1 o2s=12 o2f=20 omix=0.5 cut=3500 res=0.2 fam=0.5 aA=0.001 aD=0.25 aS=0.1 aR=0.1 fD=0.18 fS=0.1 drv=0.8 dlm=0.2 dlt=0.25 rmx=0.1" },
        { "Hip-Hop & Trap", "Jersey Stab",    "o1w=1 o2w=2 o2s=12 omix=0.45 cut=4500 res=0.25 fam=0.5 aA=0.001 aD=0.18 aS=0 aR=0.1 fD=0.12 fS=0 dlm=0.25 dlt=0.19 dlf=0.3 rmx=0.12" },
        { "Hip-Hop & Trap", "Jersey Bell",    "o1w=0 o2w=10 o2s=12 omix=0.3 fm=0.4 cut=10000 res=0 fam=0 aA=0.001 aD=0.9 aS=0 aR=0.5 dlm=0.2 dlt=0.19 rmx=0.25" },
        { "Hip-Hop & Trap", "Hyperpop Saw",   "o1w=1 o2w=1 o2f=15 omix=0.5 sub=0.1 cut=9000 res=0.15 fam=0.2 aA=0.005 aD=0.3 aS=0.8 aR=0.3 drv=0.5 crsh=0.1 chr=0.5 rmx=0.25" },

        // ------------------------------------------------------------ New Jazz, R&B & Lo-Fi
        { "R&B, Soul & Jazz", "Rhodes Keys",     "o1w=0 o2w=0 o2s=12 o2f=0 omix=0.1 fm=0.3 cut=6000 res=0 fam=0 aA=0.002 aD=1.6 aS=0.2 aR=0.5 chr=0.5 ldst=2 lrt=4.5 ldp=0.1 rmx=0.25" },
        { "R&B, Soul & Jazz", "Neo Soul Wurli",  "o1w=3 o2w=0 o2s=12 omix=0.2 fm=0.15 cut=3500 res=0.15 fam=0.25 aA=0.002 aD=1 aS=0.3 aR=0.35 drv=0.15 chr=0.35 rmx=0.2" },
        { "R&B, Soul & Jazz", "Smooth Sax",      "o1w=9 o2w=1 omix=0.25 nse=0.05 cut=2600 res=0.2 fam=0.35 aA=0.04 aD=0.3 aS=0.8 aR=0.2 ldst=0 lrt=5 ldp=0.08 rmx=0.25" },
        { "R&B, Soul & Jazz", "Muted Trumpet",   "o1w=1 o2w=2 omix=0.3 ftp=1 cut=1800 res=0.45 fam=0.3 aA=0.03 aD=0.2 aS=0.8 aR=0.15 ldst=0 lrt=5.5 ldp=0.06 rmx=0.2" },
        { "R&B, Soul & Jazz", "Upright Bass",    "o1w=0 o2w=3 omix=0.3 o2f=0 sub=0.3 cut=700 res=0.1 fam=0.3 aA=0.004 aD=0.5 aS=0.2 aR=0.25 fD=0.2 fS=0.1 rmx=0.05" },
        { "R&B, Soul & Jazz", "Vibraphone",      "o1w=0 o2w=0 o2s=24 omix=0.12 o2f=0 cut=7000 fam=0 aA=0.002 aD=2 aS=0 aR=1.2 ldst=2 lrt=4.2 ldp=0.35 rmx=0.3" },
        { "R&B, Soul & Jazz", "Lo-Fi Keys",      "o1w=3 o2w=0 o2s=12 omix=0.2 fm=0.2 cut=2200 res=0.1 fam=0.1 aA=0.003 aD=1 aS=0.2 aR=0.4 crsh=0.35 chr=0.6 ldst=0 lrt=0.5 ldp=0.08 rmx=0.2" },
        { "R&B, Soul & Jazz", "Nu Jazz Pad",     "o1w=3 o2w=1 o2s=12 omix=0.4 cut=2200 res=0.1 fam=0.15 aA=0.8 aD=1 aS=0.8 aR=1.5 chr=0.5 rmx=0.4 dlm=0.15" },

        // ------------------------------------------------------------ House, Techno & EDM
        { "Electronic & Dance", "Deep House Chord", "o1w=1 o2w=2 o2f=10 omix=0.4 cut=1500 res=0.2 fam=0.5 aA=0.005 aD=0.35 aS=0.3 aR=0.3 fD=0.3 fS=0.15 dlm=0.2 dlt=0.375 rmx=0.25 chr=0.3" },
        { "Electronic & Dance", "Techno Stab",      "o1w=1 o2w=2 o2s=12 omix=0.5 cut=900 res=0.45 fam=0.7 aA=0.001 aD=0.18 aS=0 aR=0.12 fD=0.14 fS=0 dlm=0.3 dlt=0.375 dlf=0.45 rmx=0.2" },
        { "Electronic & Dance", "Sine Kick",        "o1w=0 o2w=0 omix=0 o2f=0 cut=3000 res=0 fam=0 aA=0.001 aD=0.35 aS=0 aR=0.05 penv=24 pdec=0.03 drv=0.3 rmx=0" },
        { "Electronic & Dance", "Trance Pluck",     "o1w=1 o2w=2 o2s=12 omix=0.4 cut=2500 res=0.3 fam=0.6 aA=0.002 aD=0.3 aS=0 aR=0.2 fD=0.2 fS=0 dlm=0.3 dlt=0.375 dlf=0.5 rmx=0.35" },
        { "Electronic & Dance", "Super Saw Lead",   "o1w=1 o2w=1 o2f=15 omix=0.6 sub=0.1 cut=7000 res=0.15 fam=0.2 aA=0.01 aD=0.3 aS=0.8 aR=0.3 ldst=0 lrt=5.5 ldp=0.06 chr=0.6 dlm=0.25 dlt=0.375 rmx=0.3" },
        { "Electronic & Dance", "Acid Lead",        "o1w=1 o2w=2 o2f=10 omix=0.3 cut=700 res=0.75 fam=0.8 aA=0.002 aD=0.2 aS=0.7 aR=0.15 fA=0.002 fD=0.25 fS=0.15 fR=0.15 drv=0.5 dlm=0.3 dlt=0.3 dlf=0.45 rmx=0.1" },
        { "Electronic & Dance", "Reese Bass",       "o1w=1 o2w=1 o2f=20 omix=0.5 sub=0.4 cut=700 res=0.25 fam=0.25 aA=0.005 aD=0.5 aS=0.9 aR=0.2 drv=0.3 chr=0.3 rmx=0.05" },
        { "Electronic & Dance", "Wobble Bass",      "o1w=1 o2w=1 o2s=-12 o2f=10 omix=0.5 sub=0.4 cut=450 res=0.5 fam=0.2 aA=0.005 aD=0.3 aS=0.9 aR=0.12 ldst=1 lrt=3 ldp=0.75 lsh=1 drv=0.4 chr=0.15 rmx=0.05" },
        { "Electronic & Dance", "Neon Bass",        "o1w=1 o2w=2 o2f=6 omix=0.4 sub=0.6 cut=500 res=0.35 fam=0.6 aA=0.003 aD=0.25 aS=0.8 aR=0.1 fA=0.003 fD=0.22 fS=0.1 fR=0.1 drv=0.35 rmx=0.05" },
        { "Electronic & Dance", "Pluck Pop",        "o1w=1 o2w=2 o2s=12 omix=0.3 cut=900 res=0.25 fam=0.7 aA=0.002 aD=0.35 aS=0 aR=0.25 fA=0.002 fD=0.18 fS=0 fR=0.2 dlm=0.25 dlt=0.28 dlf=0.35 rmx=0.2" },
        { "Electronic & Dance", "Digital Stab",     "o1w=11 o2w=1 o2s=12 omix=0.3 cut=5000 res=0.25 fam=0.4 aA=0.001 aD=0.25 aS=0.1 aR=0.15 dlm=0.2 dlt=0.25 rmx=0.2" },

        // ------------------------------------------------------------ Synthwave & Vaporwave
        { "Synthwave & Retro", "Vapor Pad",         "o1w=1 o2w=3 o2f=14 omix=0.5 cut=1400 res=0.15 fam=0.2 aA=1.4 aD=1.5 aS=0.8 aR=2.5 ldst=0 lrt=0.5 ldp=0.05 chr=0.9 dlm=0.3 dlt=0.45 dlf=0.5 rsz=0.95 rmx=0.6 crsh=0.1" },
        { "Synthwave & Retro", "Neon Drive Bass",   "o1w=1 o2w=2 o2s=-12 o2f=8 omix=0.4 sub=0.3 cut=900 res=0.3 fam=0.4 aA=0.004 aD=0.25 aS=0.7 aR=0.15 drv=0.2 rmx=0.05" },
        { "Synthwave & Retro", "Outrun Lead",       "o1w=1 o2w=2 o2f=12 omix=0.4 cut=4500 res=0.2 fam=0.3 aA=0.02 aD=0.3 aS=0.8 aR=0.3 ldst=0 lrt=5.5 ldp=0.08 chr=0.5 dlm=0.3 dlt=0.375 dlf=0.45 rmx=0.35" },
        { "Synthwave & Retro", "Miami Keys",        "o1w=0 o2w=0 o2s=12 omix=0.12 fm=0.5 cut=8000 res=0 fam=0 aA=0.002 aD=1.2 aS=0.1 aR=0.5 chr=0.5 rmx=0.3" },
        { "Synthwave & Retro", "Mall Soft Bells",   "o1w=10 o2w=0 o2s=12 omix=0.3 cut=5000 res=0 fam=0 aA=0.005 aD=2 aS=0.1 aR=2 chr=0.7 dlm=0.3 dlt=0.5 rsz=0.95 rmx=0.65 crsh=0.1" },
        { "Synthwave & Retro", "Cassette Strings",  "o1w=1 o2w=1 o2f=18 omix=0.5 cut=2200 res=0.1 fam=0.15 aA=0.6 aD=0.8 aS=0.85 aR=1.2 crsh=0.25 chr=0.8 ldst=0 lrt=0.6 ldp=0.08 rmx=0.4" },
        { "Synthwave & Retro", "Sunset Brass",      "o1w=1 o2w=1 o2f=12 omix=0.5 cut=1500 res=0.15 fam=0.6 aA=0.06 aD=0.35 aS=0.7 aR=0.25 fA=0.06 fD=0.4 fS=0.4 chr=0.3 rmx=0.25" },

        // ------------------------------------------------------------ Video Game
        { "Games & Sound FX", "8-Bit Lead",        "o1w=6 o2w=6 omix=0 o2f=0 cut=12000 res=0 fam=0 aA=0.001 aD=0.1 aS=0.8 aR=0.05 ldst=0 lrt=6 ldp=0.06 crsh=0.2 rmx=0" },
        { "Games & Sound FX", "Chip Square PWM",   "o1w=2 o2w=2 o2f=0 omix=0 pw=0.5 cut=14000 res=0 fam=0 aA=0.001 aD=0.15 aS=0.7 aR=0.06 ldst=4 lrt=4 ldp=0.5 rmx=0" },
        { "Games & Sound FX", "Chip Bass",         "o1w=7 o2w=7 omix=0 o2f=0 cut=2500 res=0 fam=0 aA=0.001 aD=0.2 aS=0.8 aR=0.05 crsh=0.1 rmx=0" },
        { "Games & Sound FX", "Coin Pickup",       "o1w=6 omix=0 o2f=0 cut=14000 res=0 fam=0 aA=0.001 aD=0.25 aS=0 aR=0.1 penv=-12 pdec=0.04 rmx=0.1" },
        { "Games & Sound FX", "Laser Zap",         "o1w=1 o2w=2 omix=0.3 cut=9000 res=0.3 fam=0 aA=0.001 aD=0.22 aS=0 aR=0.05 penv=30 pdec=0.12 rmx=0.05" },
        { "Games & Sound FX", "Power Up",          "o1w=2 omix=0 cut=12000 res=0 fam=0 aA=0.001 aD=0.5 aS=0 aR=0.1 penv=-24 pdec=0.35 ldst=0 lrt=14 ldp=0.04 crsh=0.1 rmx=0" },
        { "Games & Sound FX", "Game Over Slide",   "o1w=7 omix=0 cut=6000 fam=0 aA=0.001 aD=1.2 aS=0 aR=0.2 penv=14 pdec=0.9 crsh=0.2 rmx=0.1" },
        { "Games & Sound FX", "Boss Battle Bass",  "o1w=2 o2w=7 o2s=-12 omix=0.4 o2f=0 cut=1800 res=0.35 fam=0.4 aA=0.001 aD=0.2 aS=0.7 aR=0.08 fD=0.15 fS=0.3 drv=0.3 crsh=0.15 rmx=0.05" },
        { "Games & Sound FX", "Arcade FM Bell",    "o1w=0 o2w=0 o2s=12 omix=0 fm=0.7 cut=12000 res=0 fam=0 aA=0.001 aD=0.6 aS=0 aR=0.4 chr=0.2 crsh=0.1 rmx=0.1" },
        { "Games & Sound FX", "Overworld Pluck",   "o1w=6 o2w=3 o2s=12 omix=0.3 cut=6000 res=0.1 fam=0.3 aA=0.001 aD=0.3 aS=0 aR=0.15 dlm=0.2 dlt=0.19 rmx=0.15 crsh=0.1" },
        { "Games & Sound FX", "Warp Whistle",      "o1w=0 o2w=3 o2s=12 omix=0.3 cut=8000 aA=0.01 aD=0.3 aS=0.7 aR=0.2 ldst=0 lrt=7 ldp=0.2 mode=1 gld=0.1 rmx=0.2" },
        { "Games & Sound FX", "Dungeon Drone",     "o1w=1 o2w=5 omix=0.2 nse=0.2 sub=0.4 cut=500 res=0.5 fam=0.2 aA=1.5 aD=2 aS=1 aR=3 ldst=1 lrt=0.2 ldp=0.7 lsh=1 dlm=0.3 dlt=0.6 rsz=0.9 rmx=0.6" },

        // ------------------------------------------------------------ Ambient & Cinematic
        { "Ambient & Cinematic", "Cosmic Pad",       "o1w=1 o2w=1 o2f=12 omix=0.5 sub=0.2 cut=1800 res=0.15 fam=0.25 aA=1.2 aD=1.5 aS=0.8 aR=2.5 fA=1.5 fD=2 fS=0.5 fR=2 lrt=0.3 ldp=0.25 ldst=1 chr=0.5 dlm=0.2 dlt=0.45 dlf=0.4 rsz=0.8 rmx=0.45" },
        { "Ambient & Cinematic", "Deep Space Drone", "o1w=1 o2w=0 o2s=-12 o2f=3 omix=0.5 sub=0.5 fm=0.15 cut=700 res=0.4 fam=0.2 aA=3 aD=3 aS=1 aR=5 ldst=1 lrt=0.12 ldp=0.6 lsh=1 chr=0.3 dlm=0.35 dlt=0.7 dlf=0.6 rsz=0.95 rmx=0.8" },
        { "Ambient & Cinematic", "Wind Texture",     "o1w=5 o2w=5 omix=0.5 nse=0.6 ftp=1 cut=1200 res=0.6 fam=0.3 aA=1.5 aD=1 aS=1 aR=2 ldst=1 lrt=0.25 ldp=0.7 lsh=1 rsz=0.9 rmx=0.5" },
        { "Ambient & Cinematic", "Glass Choir",      "o1w=10 o2w=8 o2s=12 omix=0.4 cut=6000 res=0.1 fam=0.1 aA=1.2 aD=1.5 aS=0.8 aR=3 chr=0.6 dlm=0.3 dlt=0.55 rsz=0.95 rmx=0.65" },
        { "Ambient & Cinematic", "Dark Atmosphere",  "o1w=1 o2w=9 o2s=-12 omix=0.4 nse=0.1 cut=600 res=0.35 fam=0.3 aA=2.5 aD=3 aS=0.9 aR=4 ldst=1 lrt=0.15 ldp=0.5 lsh=1 chr=0.4 rsz=1 rmx=0.7" },
        { "Ambient & Cinematic", "Cinematic Swell",  "o1w=1 o2w=1 o2f=20 omix=0.5 cut=500 res=0.2 fam=0.8 aA=3 aD=2 aS=0.9 aR=3 fA=3 fD=2 fS=0.8 chr=0.5 rsz=0.9 rmx=0.55" },
        { "Ambient & Cinematic", "Shimmer Pad",      "o1w=0 o2w=10 o2s=12 omix=0.5 cut=9000 res=0 fam=0 aA=1.5 aD=1.5 aS=0.8 aR=3 chr=0.8 dlm=0.35 dlt=0.5 dlf=0.55 rsz=0.95 rmx=0.7" },

        // ------------------------------------------------------------ Organic
        { "Acoustic & Orchestral", "Warm Strings",    "o1w=1 o2w=1 o2f=14 omix=0.5 cut=3500 res=0.1 fam=0.15 aA=0.5 aD=0.6 aS=0.85 aR=1 ldst=0 lrt=5.5 ldp=0.1 chr=0.6 rmx=0.35" },
        { "Acoustic & Orchestral", "Breathy Flute",   "o1w=0 o2w=3 o2s=12 omix=0.25 nse=0.2 cut=4500 res=0.25 fam=0.2 aA=0.12 aD=0.3 aS=0.8 aR=0.3 ldst=0 lrt=5 ldp=0.12 rmx=0.35" },
        { "Acoustic & Orchestral", "Pan Flute",       "o1w=0 o2w=3 o2s=12 omix=0.2 nse=0.25 cut=3000 res=0.3 aA=0.08 aD=0.2 aS=0.8 aR=0.2 ldst=0 lrt=5 ldp=0.07 rmx=0.35" },
        { "Acoustic & Orchestral", "Clarinet",        "o1w=9 o2w=9 o2s=12 o2f=0 omix=0.2 nse=0.04 cut=3500 res=0.1 fam=0.2 aA=0.06 aD=0.3 aS=0.85 aR=0.2 ldst=0 lrt=5 ldp=0.06 rmx=0.25" },
        { "Acoustic & Orchestral", "Wood Marimba",    "o1w=0 o2w=0 o2s=24 omix=0.18 fm=0.1 cut=6000 res=0 fam=0 aA=0.001 aD=0.5 aS=0 aR=0.3 rmx=0.2" },
        { "Acoustic & Orchestral", "Tonewheel Organ", "o1w=4 o2w=4 o2s=12 omix=0.4 sub=0.3 cut=9000 res=0.05 fam=0 aA=0.005 aD=0.1 aS=1 aR=0.08 ldst=2 lrt=6 ldp=0.15 chr=0.5 drv=0.2 rmx=0.15" },
        { "Acoustic & Orchestral", "Kalimba Pluck",   "o1w=3 o2w=0 o2s=12 omix=0.15 fm=0.2 cut=5000 res=0 fam=0.3 aA=0.001 aD=0.8 aS=0 aR=0.5 fA=0.001 fD=0.15 fS=0 dlm=0.3 dlt=0.3 dlf=0.35 rmx=0.3" },
        { "Acoustic & Orchestral", "Choir Aah",       "o1w=8 o2w=8 o2s=12 omix=0.4 nse=0.04 cut=4000 res=0.2 fam=0.1 aA=0.7 aD=0.5 aS=0.9 aR=1.2 ldst=0 lrt=5 ldp=0.08 chr=0.5 rsz=0.8 rmx=0.5" },
        { "Acoustic & Orchestral", "Soft Keys",       "o1w=0 o2w=0 o2s=12 omix=0.1 fm=0.3 cut=7000 fam=0 aA=0.002 aD=1.4 aS=0.15 aR=0.5 chr=0.5 rmx=0.25" },
        // ------------------------------------------------------------ Pop & Mainstream
        { "Pop & Mainstream", "Bright Pop Keys", "o1w=3 o2w=0 o2s=12 omix=0.28 fm=0.12 cut=6300 res=0.13 fam=0.25 aA=0.003 aD=0.8 aS=0.35 aR=0.32 chr=0.3 rmx=0.22" },
        { "Pop & Mainstream", "Radio Synth Lead", "o1w=1 o2w=2 o2f=9 omix=0.4 cut=5900 res=0.16 fam=0.35 aA=0.007 aD=0.23 aS=0.72 aR=0.19 chr=0.35 dlm=0.16 rmx=0.16" },
        { "Pop & Mainstream", "Dream Pop Pad", "o1w=1 o2w=3 o2f=16 omix=0.4 cut=2400 res=0.15 fam=0.2 aA=0.65 aD=1.1 aS=0.8 aR=1.8 chr=0.68 rsz=0.82 rmx=0.42" },
        { "Pop & Mainstream", "Soft Pop Pluck", "o1w=3 o2w=10 o2s=12 omix=0.19 cut=5700 fam=0.5 aA=0.002 aD=0.42 aS=0 aR=0.37 dlm=0.19 rmx=0.27" },
        { "Pop & Mainstream", "Vocal Style Synth", "o1w=8 o2w=1 o2f=8 omix=0.25 cut=3400 res=0.3 fam=0.18 aA=0.12 aD=0.35 aS=0.76 aR=0.32 ldst=0 ldp=0.07 rmx=0.3" },

        // ------------------------------------------------------------ Rock & Alternative
        { "Rock & Alternative", "Vintage Organ", "o1w=4 o2w=4 o2s=12 omix=0.45 cut=7000 res=0.08 aA=0.005 aD=0.12 aS=1 aR=0.12 chr=0.4 drv=0.2 rmx=0.17" },
        { "Rock & Alternative", "Analog Bass", "o1w=1 o2w=2 o2s=-12 omix=0.35 sub=0.37 cut=1200 res=0.21 fam=0.3 aA=0.003 aD=0.3 aS=0.8 aR=0.13 drv=0.24 rmx=0.04" },
        { "Rock & Alternative", "Gritty Lead", "o1w=1 o2w=2 o2f=14 omix=0.35 cut=4200 res=0.3 fam=0.4 aA=0.008 aD=0.27 aS=0.75 aR=0.2 drv=0.6 chr=0.1 rmx=0.14" },
        { "Rock & Alternative", "Indie Chorus Keys", "o1w=3 o2w=0 o2s=12 omix=0.22 fm=0.22 cut=4300 res=0.13 aA=0.004 aD=0.9 aS=0.38 aR=0.4 chr=0.8 rmx=0.27" },
        { "Rock & Alternative", "Power Synth", "o1w=1 o2w=1 o2f=16 omix=0.5 sub=0.22 cut=4900 res=0.13 fam=0.4 aA=0.005 aD=0.25 aS=0.8 aR=0.24 drv=0.52 chr=0.3" },

        // ------------------------------------------------------------ Funk & Disco
        { "Funk & Disco", "Disco Strings", "o1w=1 o2w=3 o2s=12 omix=0.4 cut=5200 res=0.12 fam=0.1 aA=0.12 aD=0.35 aS=0.83 aR=0.48 chr=0.38 rmx=0.22" },
        { "Funk & Disco", "Clav Groove", "o1w=2 o2w=3 o2s=12 omix=0.24 cut=4500 res=0.24 fam=0.5 aA=0.002 aD=0.21 aS=0.13 aR=0.13 drv=0.19 chr=0.17" },
        { "Funk & Disco", "Funk Synth Bass", "o1w=2 o2w=1 o2s=-12 omix=0.38 sub=0.22 cut=650 res=0.36 fam=0.76 aA=0.002 aD=0.18 aS=0.65 aR=0.13 fD=0.2 drv=0.29" },
        { "Funk & Disco", "Retro Brass Stab", "o1w=1 o2w=2 o2s=12 omix=0.3 cut=2100 res=0.14 fam=0.69 aA=0.02 aD=0.24 aS=0.48 aR=0.16 chr=0.22 rmx=0.19" },

        // ------------------------------------------------------------ Latin & Reggae
        { "Latin & Reggae", "Reggae Bubble Keys", "o1w=4 o2w=3 o2s=12 omix=0.22 cut=3500 res=0.1 aA=0.003 aD=0.16 aS=0.15 aR=0.1 chr=0.17 rmx=0.13" },
        { "Latin & Reggae", "Latin Bell", "o1w=10 o2w=0 o2s=12 omix=0.28 cut=7800 res=0.06 aA=0.002 aD=0.83 aS=0 aR=0.46 dlm=0.19 rmx=0.24" },
        { "Latin & Reggae", "Dub Bass", "o1w=0 o2w=3 o2s=-12 omix=0.27 sub=0.47 cut=610 res=0.12 aA=0.004 aD=0.43 aS=0.73 aR=0.2 dlm=0.13 rmx=0.04" },
        { "Latin & Reggae", "Caribbean Pluck", "o1w=3 o2w=0 o2s=12 omix=0.28 cut=5900 res=0.08 fam=0.4 aA=0.002 aD=0.31 aS=0 aR=0.22 rmx=0.22" }

    };
    return list;
}
