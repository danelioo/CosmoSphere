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
        { "Trap & Rap", "808 Sub",        "o1w=0 o2w=0 omix=0 o2f=0 cut=1200 res=0 fam=0 aA=0.002 aD=1.8 aS=0 aR=0.3 penv=10 pdec=0.05 mode=1 gld=0.08 drv=0.3 rmx=0" },
        { "Trap & Rap", "808 Distorted",  "o1w=0 o2w=3 omix=0.2 o2f=0 cut=2500 res=0.1 fam=0 aA=0.002 aD=1.4 aS=0 aR=0.25 penv=14 pdec=0.04 mode=1 gld=0.06 drv=0.8 crsh=0.1 rmx=0" },
        { "Trap & Rap", "Drill Slide",    "o1w=0 o2w=2 omix=0.15 o2f=0 cut=1500 res=0.2 fam=0.3 aA=0.003 aD=1.2 aS=0 aR=0.3 penv=6 pdec=0.05 mode=1 gld=0.2 drv=0.5 rmx=0.05" },
        { "Trap & Rap", "Trap Bell",      "o1w=10 o2w=0 o2s=12 omix=0.25 o2f=0 cut=9000 res=0 fam=0 aA=0.001 aD=1.1 aS=0 aR=0.8 dlm=0.25 dlt=0.375 dlf=0.4 rsz=0.7 rmx=0.35 chr=0.2" },
        { "Trap & Rap", "Dark Pluck",     "o1w=2 o2w=1 o2s=-12 omix=0.4 cut=1200 res=0.3 fam=0.6 aA=0.002 aD=0.4 aS=0 aR=0.2 fD=0.25 fS=0 dlm=0.2 dlt=0.375 rsz=0.6 rmx=0.25" },
        { "Trap & Rap", "Trap Flute",     "o1w=0 o2w=9 o2s=12 omix=0.3 nse=0.08 cut=4000 res=0.15 fam=0.2 aA=0.05 aD=0.4 aS=0.7 aR=0.4 ldst=0 lrt=5 ldp=0.1 dlm=0.25 dlt=0.375 rmx=0.35" },
        { "Trap & Rap", "Trap Choir",     "o1w=8 o2w=8 o2s=12 omix=0.3 cut=3000 res=0.2 fam=0.2 aA=0.4 aD=0.8 aS=0.7 aR=1.5 chr=0.5 dlm=0.2 rsz=0.9 rmx=0.55" },
        { "Trap & Rap", "Hi-Hat Noise",   "o1w=5 o2w=5 omix=0 ftp=2 cut=7500 res=0.2 fam=0.2 aA=0.001 aD=0.06 aS=0 aR=0.04 fA=0.001 fD=0.05 fS=0 rmx=0.1" },

        // ------------------------------------------------------------ Plugg, Rage & Jersey
        { "Plugg, Rage & Jersey", "Plugg Dream",    "o1w=6 o2w=3 o2s=12 omix=0.35 o2f=6 cut=3500 res=0.2 fam=0.5 aA=0.003 aD=0.7 aS=0 aR=0.9 fD=0.35 fS=0.1 chr=0.7 dlm=0.3 dlt=0.375 dlf=0.45 rsz=0.9 rmx=0.55" },
        { "Plugg, Rage & Jersey", "Pluggnb Keys",   "o1w=0 o2w=10 o2s=12 omix=0.3 fm=0.2 cut=5000 res=0.1 fam=0.2 aA=0.003 aD=1.5 aS=0.1 aR=1.2 chr=0.6 dlm=0.25 rsz=0.9 rmx=0.6" },
        { "Plugg, Rage & Jersey", "Jugg Pluck",     "o1w=2 o2w=6 o2s=12 omix=0.4 o2f=5 cut=2800 res=0.35 fam=0.7 aA=0.001 aD=0.22 aS=0 aR=0.12 fD=0.15 fS=0 dlm=0.2 dlt=0.19 dlf=0.3 rmx=0.15" },
        { "Plugg, Rage & Jersey", "Rage Lead",      "o1w=1 o2w=2 o2f=15 omix=0.5 cut=6000 res=0.15 fam=0.3 aA=0.002 aD=0.3 aS=0.8 aR=0.15 drv=0.85 crsh=0.15 chr=0.3 rmx=0.1" },
        { "Plugg, Rage & Jersey", "Rage Stab",      "o1w=1 o2w=1 o2s=7 o2f=20 omix=0.5 cut=3500 res=0.2 fam=0.5 aA=0.001 aD=0.25 aS=0.1 aR=0.1 fD=0.18 fS=0.1 drv=0.8 dlm=0.2 dlt=0.25 rmx=0.1" },
        { "Plugg, Rage & Jersey", "Jersey Stab",    "o1w=1 o2w=2 o2s=12 omix=0.45 cut=4500 res=0.25 fam=0.5 aA=0.001 aD=0.18 aS=0 aR=0.1 fD=0.12 fS=0 dlm=0.25 dlt=0.19 dlf=0.3 rmx=0.12" },
        { "Plugg, Rage & Jersey", "Jersey Bell",    "o1w=0 o2w=10 o2s=19 omix=0.3 fm=0.4 cut=10000 res=0 fam=0 aA=0.001 aD=0.9 aS=0 aR=0.5 dlm=0.2 dlt=0.19 rmx=0.25" },
        { "Plugg, Rage & Jersey", "Hyperpop Saw",   "o1w=1 o2w=1 o2f=35 omix=0.5 sub=0.1 cut=9000 res=0.15 fam=0.2 aA=0.005 aD=0.3 aS=0.8 aR=0.3 drv=0.5 crsh=0.1 chr=0.5 rmx=0.25" },

        // ------------------------------------------------------------ New Jazz, R&B & Lo-Fi
        { "New Jazz, R&B & Lo-Fi", "Rhodes Keys",     "o1w=0 o2w=0 o2s=12 o2f=0 omix=0.1 fm=0.3 cut=6000 res=0 fam=0 aA=0.002 aD=1.6 aS=0.2 aR=0.5 chr=0.5 ldst=2 lrt=4.5 ldp=0.1 rmx=0.25" },
        { "New Jazz, R&B & Lo-Fi", "Neo Soul Wurli",  "o1w=3 o2w=0 o2s=12 omix=0.2 fm=0.15 cut=3500 res=0.15 fam=0.25 aA=0.002 aD=1 aS=0.3 aR=0.35 drv=0.15 chr=0.35 rmx=0.2" },
        { "New Jazz, R&B & Lo-Fi", "Smooth Sax",      "o1w=9 o2w=1 omix=0.25 nse=0.05 cut=2600 res=0.2 fam=0.35 aA=0.04 aD=0.3 aS=0.8 aR=0.2 ldst=0 lrt=5 ldp=0.08 rmx=0.25" },
        { "New Jazz, R&B & Lo-Fi", "Muted Trumpet",   "o1w=1 o2w=2 omix=0.3 ftp=1 cut=1800 res=0.45 fam=0.3 aA=0.03 aD=0.2 aS=0.8 aR=0.15 ldst=0 lrt=5.5 ldp=0.06 rmx=0.2" },
        { "New Jazz, R&B & Lo-Fi", "Upright Bass",    "o1w=0 o2w=3 omix=0.3 o2f=0 sub=0.3 cut=700 res=0.1 fam=0.3 aA=0.004 aD=0.5 aS=0.2 aR=0.25 fD=0.2 fS=0.1 rmx=0.05" },
        { "New Jazz, R&B & Lo-Fi", "Vibraphone",      "o1w=0 o2w=0 o2s=24 omix=0.12 o2f=0 cut=7000 fam=0 aA=0.002 aD=2 aS=0 aR=1.2 ldst=2 lrt=4.2 ldp=0.35 rmx=0.3" },
        { "New Jazz, R&B & Lo-Fi", "Lo-Fi Keys",      "o1w=3 o2w=0 o2s=12 omix=0.2 fm=0.2 cut=2200 res=0.1 fam=0.1 aA=0.003 aD=1 aS=0.2 aR=0.4 crsh=0.35 chr=0.6 ldst=0 lrt=0.5 ldp=0.08 rmx=0.2" },
        { "New Jazz, R&B & Lo-Fi", "Nu Jazz Pad",     "o1w=3 o2w=1 o2s=7 omix=0.4 cut=2200 res=0.1 fam=0.15 aA=0.8 aD=1 aS=0.8 aR=1.5 chr=0.5 rmx=0.4 dlm=0.15" },

        // ------------------------------------------------------------ House, Techno & EDM
        { "House, Techno & EDM", "Deep House Chord", "o1w=1 o2w=2 o2f=10 omix=0.4 cut=1500 res=0.2 fam=0.5 aA=0.005 aD=0.35 aS=0.3 aR=0.3 fD=0.3 fS=0.15 dlm=0.2 dlt=0.375 rmx=0.25 chr=0.3" },
        { "House, Techno & EDM", "Techno Stab",      "o1w=1 o2w=2 o2s=7 omix=0.5 cut=900 res=0.45 fam=0.7 aA=0.001 aD=0.18 aS=0 aR=0.12 fD=0.14 fS=0 dlm=0.3 dlt=0.375 dlf=0.45 rmx=0.2" },
        { "House, Techno & EDM", "Sine Kick",        "o1w=0 o2w=0 omix=0 o2f=0 cut=3000 res=0 fam=0 aA=0.001 aD=0.35 aS=0 aR=0.05 penv=24 pdec=0.03 drv=0.3 rmx=0" },
        { "House, Techno & EDM", "Trance Pluck",     "o1w=1 o2w=2 o2s=12 omix=0.4 cut=2500 res=0.3 fam=0.6 aA=0.002 aD=0.3 aS=0 aR=0.2 fD=0.2 fS=0 dlm=0.3 dlt=0.375 dlf=0.5 rmx=0.35" },
        { "House, Techno & EDM", "Super Saw Lead",   "o1w=1 o2w=1 o2f=28 omix=0.6 sub=0.1 cut=7000 res=0.15 fam=0.2 aA=0.01 aD=0.3 aS=0.8 aR=0.3 ldst=0 lrt=5.5 ldp=0.06 chr=0.6 dlm=0.25 dlt=0.375 rmx=0.3" },
        { "House, Techno & EDM", "Acid Lead",        "o1w=1 o2w=2 o2f=10 omix=0.3 cut=700 res=0.75 fam=0.8 aA=0.002 aD=0.2 aS=0.7 aR=0.15 fA=0.002 fD=0.25 fS=0.15 fR=0.15 drv=0.5 dlm=0.3 dlt=0.3 dlf=0.45 rmx=0.1" },
        { "House, Techno & EDM", "Reese Bass",       "o1w=1 o2w=1 o2f=45 omix=0.5 sub=0.4 cut=700 res=0.25 fam=0.25 aA=0.005 aD=0.5 aS=0.9 aR=0.2 drv=0.3 chr=0.3 rmx=0.05" },
        { "House, Techno & EDM", "Wobble Bass",      "o1w=1 o2w=1 o2s=-12 o2f=10 omix=0.5 sub=0.4 cut=450 res=0.5 fam=0.2 aA=0.005 aD=0.3 aS=0.9 aR=0.12 ldst=1 lrt=3 ldp=0.75 lsh=1 drv=0.4 chr=0.15 rmx=0.05" },
        { "House, Techno & EDM", "Neon Bass",        "o1w=1 o2w=2 o2f=6 omix=0.4 sub=0.6 cut=500 res=0.35 fam=0.6 aA=0.003 aD=0.25 aS=0.8 aR=0.1 fA=0.003 fD=0.22 fS=0.1 fR=0.1 drv=0.35 rmx=0.05" },
        { "House, Techno & EDM", "Pluck Pop",        "o1w=1 o2w=2 o2s=12 omix=0.3 cut=900 res=0.25 fam=0.7 aA=0.002 aD=0.35 aS=0 aR=0.25 fA=0.002 fD=0.18 fS=0 fR=0.2 dlm=0.25 dlt=0.28 dlf=0.35 rmx=0.2" },
        { "House, Techno & EDM", "Digital Stab",     "o1w=11 o2w=1 o2s=12 omix=0.3 cut=5000 res=0.25 fam=0.4 aA=0.001 aD=0.25 aS=0.1 aR=0.15 dlm=0.2 dlt=0.25 rmx=0.2" },

        // ------------------------------------------------------------ Synthwave & Vaporwave
        { "Synthwave & Vaporwave", "Vapor Pad",         "o1w=1 o2w=3 o2f=14 omix=0.5 cut=1400 res=0.15 fam=0.2 aA=1.4 aD=1.5 aS=0.8 aR=2.5 ldst=0 lrt=0.5 ldp=0.05 chr=0.9 dlm=0.3 dlt=0.45 dlf=0.5 rsz=0.95 rmx=0.6 crsh=0.1" },
        { "Synthwave & Vaporwave", "Neon Drive Bass",   "o1w=1 o2w=2 o2s=-12 o2f=8 omix=0.4 sub=0.3 cut=900 res=0.3 fam=0.4 aA=0.004 aD=0.25 aS=0.7 aR=0.15 drv=0.2 rmx=0.05" },
        { "Synthwave & Vaporwave", "Outrun Lead",       "o1w=1 o2w=2 o2f=12 omix=0.4 cut=4500 res=0.2 fam=0.3 aA=0.02 aD=0.3 aS=0.8 aR=0.3 ldst=0 lrt=5.5 ldp=0.08 chr=0.5 dlm=0.3 dlt=0.375 dlf=0.45 rmx=0.35" },
        { "Synthwave & Vaporwave", "Miami Keys",        "o1w=0 o2w=0 o2s=12 omix=0.12 fm=0.5 cut=8000 res=0 fam=0 aA=0.002 aD=1.2 aS=0.1 aR=0.5 chr=0.5 rmx=0.3" },
        { "Synthwave & Vaporwave", "Mall Soft Bells",   "o1w=10 o2w=0 o2s=12 omix=0.3 cut=5000 res=0 fam=0 aA=0.005 aD=2 aS=0.1 aR=2 chr=0.7 dlm=0.3 dlt=0.5 rsz=0.95 rmx=0.65 crsh=0.1" },
        { "Synthwave & Vaporwave", "Cassette Strings",  "o1w=1 o2w=1 o2f=18 omix=0.5 cut=2200 res=0.1 fam=0.15 aA=0.6 aD=0.8 aS=0.85 aR=1.2 crsh=0.25 chr=0.8 ldst=0 lrt=0.6 ldp=0.08 rmx=0.4" },
        { "Synthwave & Vaporwave", "Sunset Brass",      "o1w=1 o2w=1 o2f=12 omix=0.5 cut=1500 res=0.15 fam=0.6 aA=0.06 aD=0.35 aS=0.7 aR=0.25 fA=0.06 fD=0.4 fS=0.4 chr=0.3 rmx=0.25" },

        // ------------------------------------------------------------ Video Game
        { "Video Game", "8-Bit Lead",        "o1w=6 o2w=6 omix=0 o2f=0 cut=12000 res=0 fam=0 aA=0.001 aD=0.1 aS=0.8 aR=0.05 ldst=0 lrt=6 ldp=0.06 crsh=0.2 rmx=0" },
        { "Video Game", "Chip Square PWM",   "o1w=2 o2w=2 o2f=0 omix=0 pw=0.5 cut=14000 res=0 fam=0 aA=0.001 aD=0.15 aS=0.7 aR=0.06 ldst=4 lrt=4 ldp=0.5 rmx=0" },
        { "Video Game", "Chip Bass",         "o1w=7 o2w=7 omix=0 o2f=0 cut=2500 res=0 fam=0 aA=0.001 aD=0.2 aS=0.8 aR=0.05 crsh=0.1 rmx=0" },
        { "Video Game", "Coin Pickup",       "o1w=6 omix=0 o2f=0 cut=14000 res=0 fam=0 aA=0.001 aD=0.25 aS=0 aR=0.1 penv=-12 pdec=0.04 rmx=0.1" },
        { "Video Game", "Laser Zap",         "o1w=1 o2w=2 omix=0.3 cut=9000 res=0.3 fam=0 aA=0.001 aD=0.22 aS=0 aR=0.05 penv=30 pdec=0.12 rmx=0.05" },
        { "Video Game", "Power Up",          "o1w=2 omix=0 cut=12000 res=0 fam=0 aA=0.001 aD=0.5 aS=0 aR=0.1 penv=-24 pdec=0.35 ldst=0 lrt=14 ldp=0.04 crsh=0.1 rmx=0" },
        { "Video Game", "Game Over Slide",   "o1w=7 omix=0 cut=6000 fam=0 aA=0.001 aD=1.2 aS=0 aR=0.2 penv=14 pdec=0.9 crsh=0.2 rmx=0.1" },
        { "Video Game", "Boss Battle Bass",  "o1w=2 o2w=7 o2s=-12 omix=0.4 o2f=0 cut=1800 res=0.35 fam=0.4 aA=0.001 aD=0.2 aS=0.7 aR=0.08 fD=0.15 fS=0.3 drv=0.3 crsh=0.15 rmx=0.05" },
        { "Video Game", "Arcade FM Bell",    "o1w=0 o2w=0 o2s=7 omix=0 fm=0.7 cut=12000 res=0 fam=0 aA=0.001 aD=0.6 aS=0 aR=0.4 chr=0.2 crsh=0.1 rmx=0.1" },
        { "Video Game", "Overworld Pluck",   "o1w=6 o2w=3 o2s=12 omix=0.3 cut=6000 res=0.1 fam=0.3 aA=0.001 aD=0.3 aS=0 aR=0.15 dlm=0.2 dlt=0.19 rmx=0.15 crsh=0.1" },
        { "Video Game", "Warp Whistle",      "o1w=0 o2w=3 o2s=12 omix=0.3 cut=8000 aA=0.01 aD=0.3 aS=0.7 aR=0.2 ldst=0 lrt=7 ldp=0.2 mode=1 gld=0.1 rmx=0.2" },
        { "Video Game", "Dungeon Drone",     "o1w=1 o2w=5 omix=0.2 nse=0.2 sub=0.4 cut=500 res=0.5 fam=0.2 aA=1.5 aD=2 aS=1 aR=3 ldst=1 lrt=0.2 ldp=0.7 lsh=1 dlm=0.3 dlt=0.6 rsz=0.9 rmx=0.6" },

        // ------------------------------------------------------------ Ambient & Cinematic
        { "Ambient & Cinematic", "Cosmic Pad",       "o1w=1 o2w=1 o2f=12 omix=0.5 sub=0.2 cut=1800 res=0.15 fam=0.25 aA=1.2 aD=1.5 aS=0.8 aR=2.5 fA=1.5 fD=2 fS=0.5 fR=2 lrt=0.3 ldp=0.25 ldst=1 chr=0.5 dlm=0.2 dlt=0.45 dlf=0.4 rsz=0.8 rmx=0.45" },
        { "Ambient & Cinematic", "Deep Space Drone", "o1w=1 o2w=0 o2s=-12 o2f=3 omix=0.5 sub=0.5 fm=0.15 cut=700 res=0.4 fam=0.2 aA=3 aD=3 aS=1 aR=5 ldst=1 lrt=0.12 ldp=0.6 lsh=1 chr=0.3 dlm=0.35 dlt=0.7 dlf=0.6 rsz=0.95 rmx=0.8" },
        { "Ambient & Cinematic", "Wind Texture",     "o1w=5 o2w=5 omix=0.5 nse=0.6 ftp=1 cut=1200 res=0.6 fam=0.3 aA=1.5 aD=1 aS=1 aR=2 ldst=1 lrt=0.25 ldp=0.7 lsh=1 rsz=0.9 rmx=0.5" },
        { "Ambient & Cinematic", "Glass Choir",      "o1w=10 o2w=8 o2s=12 omix=0.4 cut=6000 res=0.1 fam=0.1 aA=1.2 aD=1.5 aS=0.8 aR=3 chr=0.6 dlm=0.3 dlt=0.55 rsz=0.95 rmx=0.65" },
        { "Ambient & Cinematic", "Dark Atmosphere",  "o1w=1 o2w=9 o2s=-12 omix=0.4 nse=0.1 cut=600 res=0.35 fam=0.3 aA=2.5 aD=3 aS=0.9 aR=4 ldst=1 lrt=0.15 ldp=0.5 lsh=1 chr=0.4 rsz=1 rmx=0.7" },
        { "Ambient & Cinematic", "Cinematic Swell",  "o1w=1 o2w=1 o2f=25 omix=0.5 cut=500 res=0.2 fam=0.8 aA=3 aD=2 aS=0.9 aR=3 fA=3 fD=2 fS=0.8 chr=0.5 rsz=0.9 rmx=0.55" },
        { "Ambient & Cinematic", "Shimmer Pad",      "o1w=0 o2w=10 o2s=12 omix=0.5 cut=9000 res=0 fam=0 aA=1.5 aD=1.5 aS=0.8 aR=3 chr=0.8 dlm=0.35 dlt=0.5 dlf=0.55 rsz=0.95 rmx=0.7" },

        // ------------------------------------------------------------ Organic
        { "Organic", "Warm Strings",    "o1w=1 o2w=1 o2f=14 omix=0.5 cut=3500 res=0.1 fam=0.15 aA=0.5 aD=0.6 aS=0.85 aR=1 ldst=0 lrt=5.5 ldp=0.1 chr=0.6 rmx=0.35" },
        { "Organic", "Breathy Flute",   "o1w=0 o2w=3 o2s=12 omix=0.25 nse=0.2 cut=4500 res=0.25 fam=0.2 aA=0.12 aD=0.3 aS=0.8 aR=0.3 ldst=0 lrt=5 ldp=0.12 rmx=0.35" },
        { "Organic", "Pan Flute",       "o1w=0 o2w=3 o2s=12 omix=0.2 nse=0.25 cut=3000 res=0.3 aA=0.08 aD=0.2 aS=0.8 aR=0.2 ldst=0 lrt=5 ldp=0.07 rmx=0.35" },
        { "Organic", "Clarinet",        "o1w=9 o2w=9 o2s=12 o2f=0 omix=0.2 nse=0.04 cut=3500 res=0.1 fam=0.2 aA=0.06 aD=0.3 aS=0.85 aR=0.2 ldst=0 lrt=5 ldp=0.06 rmx=0.25" },
        { "Organic", "Wood Marimba",    "o1w=0 o2w=0 o2s=24 omix=0.18 fm=0.1 cut=6000 res=0 fam=0 aA=0.001 aD=0.5 aS=0 aR=0.3 rmx=0.2" },
        { "Organic", "Tonewheel Organ", "o1w=4 o2w=4 o2s=12 omix=0.4 sub=0.3 cut=9000 res=0.05 fam=0 aA=0.005 aD=0.1 aS=1 aR=0.08 ldst=2 lrt=6 ldp=0.15 chr=0.5 drv=0.2 rmx=0.15" },
        { "Organic", "Kalimba Pluck",   "o1w=3 o2w=0 o2s=19 omix=0.15 fm=0.2 cut=5000 res=0 fam=0.3 aA=0.001 aD=0.8 aS=0 aR=0.5 fA=0.001 fD=0.15 fS=0 dlm=0.3 dlt=0.3 dlf=0.35 rmx=0.3" },
        { "Organic", "Choir Aah",       "o1w=8 o2w=8 o2s=12 omix=0.4 nse=0.04 cut=4000 res=0.2 fam=0.1 aA=0.7 aD=0.5 aS=0.9 aR=1.2 ldst=0 lrt=5 ldp=0.08 chr=0.5 rsz=0.8 rmx=0.5" },
        { "Organic", "Soft Keys",       "o1w=0 o2w=0 o2s=12 omix=0.1 fm=0.3 cut=7000 fam=0 aA=0.002 aD=1.4 aS=0.15 aR=0.5 chr=0.5 rmx=0.25" }
    };
    return list;
}
