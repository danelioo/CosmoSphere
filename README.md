# Cosphere 1.0

Free JUCE synthesizer (GPL-3.0-or-later). Windows 64-bit builds are distributed as **Cosphere.vst3** (portable ZIP) and **Cosphere.exe** (standalone) — **no installer**.

## Manual VST3 setup
Extract `Cosphere.vst3` into `C:\Program Files\Common Files\VST3\` and rescan plugins in FL Studio. Standalone `Cosphere.exe` can be launched directly, without installation.

## Controls
Choose a preset through the genre submenus. `Options` controls interface scaling and themes; the chosen scale is saved locally. Master volume is at top right. Delay `SYNC` is enabled by default and uses the host's BPM (standalone fallback: 120 BPM); choose `BEAT` subdivision, from 1/1 to 1/32. When Sync is off, use `TIME` in seconds.

In standalone, click the virtual keyboard to give it focus and use the mapped computer keys `Z S X D C V G B H N J M , L . ; /` to play notes. **FL Studio handles its own typing keyboard to piano** and forwards MIDI to the VST3; you may need to enable FL Studio's `Typing keyboard to piano keyboard` option.

## Build
GitHub Actions (`.github/workflows/build.yml`) compiles on Windows using Visual Studio 2022. Local requirements: Git, CMake, Visual Studio 2022 with C++ desktop workload and internet access to download JUCE 7.0.12. `cmake -B build -G "Visual Studio 17 2022" -A x64` then `cmake --build build --config Release`. Git tag `v1.0.0` triggers release upload only if build succeeds.

## License
Copyright (C) 2026 Danilo Pereira. GPL-3.0-or-later; see `LICENSE` and `NOTICE.md`.
