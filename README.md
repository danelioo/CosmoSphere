# Cosphere 1.0

**A free, lightweight synthesizer built for creativity, simplicity, and low-end PCs.**

Cosphere is a homemade synthesizer project developed with [JUCE](https://juce.com/), originally created as a personal experiment in audio plugin development. The goal was simple: build a fun, accessible synthesizer that doesn't demand powerful hardware or consume unnecessary CPU resources.

What started as a small learning project became a fully functional instrument, and I decided to share it with everyone — **completely free and open-source**.

Cosphere isn't meant to compete with major commercial synthesizers. It's a personal project made for experimentation, music production, and anyone who wants a simple instrument, especially on older or less powerful computers.

## Screenshots

### Default Dark Theme
![Cosphere - Default Dark Theme](screenshots/cosphere-dark.png)

### Light Theme
![Cosphere - Light Theme](screenshots/cosphere-light.png)

## Download

**[Download Cosphere 1.0 from GitHub Releases](https://github.com/danelioo/Cosphere/releases)**

Available for Windows 64-bit:

- **VST3:** Use Cosphere inside your favorite compatible DAW.
- **Standalone:** Run Cosphere directly without a DAW.

No installer, account, activation, or payment required.

## Features

- Lightweight synthesizer designed with CPU efficiency in mind
- 8-voice polyphony
- 89 factory presets organized into 12 categories
- Dual oscillators with waveform selection
- Filters with dedicated envelope
- ADSR amplitude envelope
- Modulation with BPM sync
- Built-in drive, bitcrusher, chorus, delay, and reverb
- Tempo-synced effects with straight and triplet subdivisions
- Multiple interface themes and adjustable UI scaling
- On-screen piano keyboard and QWERTY support in standalone mode
- VST3 and standalone formats

## Installation

### VST3 (Windows)

1. Download and extract the VST3 ZIP from Releases.
2. Copy the `Cosphere.vst3` folder to:

   `C:\Program Files\Common Files\VST3\`

3. Open your DAW and rescan your installed plugins.
4. Load Cosphere as an instrument.

Compatible VST3 hosts include FL Studio, Ableton Live, REAPER, Cubase, Studio One, and other VST3-compatible DAWs.

### Standalone (Windows)

Extract and launch `Cosphere.exe`. No installation required.

The standalone version supports computer-keyboard note input and an on-screen piano.

## Building from Source

Cosphere is built using JUCE 7 and CMake.

Requirements:
- CMake
- Git
- Visual Studio 2022 with C++ development tools
- JUCE 7.0.12 (downloaded during configuration)

```bash
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Windows builds are also configured through GitHub Actions.

## License

Cosphere is free and open-source software distributed under **GPL-3.0-or-later**.

Copyright © 2026 Danilo Pereira.

See [LICENSE](LICENSE) for details.

---

*Made as a personal experiment. Shared for free. Enjoy making music!*
