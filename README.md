# Cosphere

**Cosphere** é um sintetizador polifônico híbrido (VST3 + Standalone) que mistura timbres eletrônicos e orgânicos, com um globo de grades que gira conforme você toca.

> Um projeto caseiro, feito por **Danilo Pereira**, com a ideia de disponibilizar um plugin **gratuito** para quem quiser baixar e usar.
> Ele foi crescendo aos poucos, entre ideias, muita tentativa e erro, testes no FL Studio e a ajuda de alguns assistentes de IA que escrevem código — o que, convenhamos, ajuda bastante quando o hobby é grande e o tempo é curto.

---

## O que tem dentro

- **Motor de som:** 2 osciladores com 12 formas de onda (Sine, Saw, Square, Triangle, Organ, Noise, Pulse 25, Chip Tri, Vowel, Reed, Glass, Digital), sub, noise, FM, PWM
- **Filtro** ressonante (LP / BP / HP) com envelope próprio
- **Envelopes** de amplitude e de filtro, **envelope de pitch** (808, kicks, lasers, "coin" de videogame)
- **Modulação:** LFO com 4 formas e 5 destinos (pitch, cutoff, amp, FM, PWM), mod wheel e pitch bend
- **Vozes:** polifônico com até **32 vozes** (você escolhe 8, 12, 16, 24 ou 32), modo **Mono** com **glide** (ideal para 808 e leads)
- **Efeitos:** drive, crush (bit/sample rate), chorus, delay ping-pong, reverb
- **72 presets** por categoria: Trap & Rap, Plugg/Rage/Jersey, New Jazz/R&B/Lo-Fi, House/Techno/EDM, Synthwave & Vaporwave, Video Game, Ambient & Cinematic, Organic
- **Teclado** embaixo: clique para tocar e veja as notas que estão soando (no Standalone dá para tocar com o teclado do computador)
- **8 temas** de cor (padrão azul `#19e5f7`) — o tema e o tamanho da janela são lembrados
- Janela **redimensionável**
- Botão **i** com informações sobre o plugin

## Instalação (Windows)

1. Baixe o artifact **Cosphere-VST3-Windows** na aba *Actions* do GitHub (ou a versão publicada em *Releases*).
2. Copie a pasta **`Cosphere.vst3` inteira** (com a pasta `Contents` dentro) para `C:\Program Files\Common Files\VST3`.
3. No FL Studio: *Options → Manage plugins → Find plugins*.

> Importante: no Windows o VST3 é uma **pasta**. Copiar só o arquivo `.vst3` de dentro dela não funciona.
> Se você tinha uma versão de teste com outro nome, apague a pasta/arquivo dela para não aparecer duplicado na lista.

O **Standalone** (`Cosphere.exe`) abre o sintetizador sem DAW, só para testar.

## Como compilar

O projeto usa **JUCE 7.0.12** + CMake, e o build é feito automaticamente pelo GitHub Actions (`.github/workflows/build.yml`). Para compilar localmente:

```
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

O resultado fica em `build/Cosphere_artefacts/Release/`.

### Fonte do título

O nome usa a fonte **Redaction 50** (licença SIL Open Font License). O workflow baixa o arquivo automaticamente para `Assets/Redaction50.ttf`. Se o download falhar, o plugin usa uma fonte padrão e funciona normalmente. Você também pode colocar o arquivo lá manualmente (veja `Assets/LEIAME.txt`).

## Créditos e licença

- Feito com [JUCE](https://juce.com). O JUCE é usado sob a licença **GPLv3**, então este projeto deve ser distribuído com o código-fonte disponível e sob GPLv3. Se for publicar, adicione um arquivo `LICENSE` com o texto da GPLv3.
- Fonte Redaction 50 por Jeremy Mickel / MCKL — [SIL OFL](https://scripts.sil.org/OFL).
- Ideia, direção, testes e curadoria de sons: **Danilo Pereira**.

Feito em casa, com carinho. Se o plugin te ajudou a fazer uma música, já valeu.
