<div align="center">

# 🥚 Tamagotchi ESP32-C3

### *Um bichinho-robô de olhos expressivos que, em breve, vai sair andando por aí sem cair da mesa*

```
   ╭──────────╮          ╭──────────╮
   │          │          │          │
   │    ██    │          │    ██    │
   │          │          │          │
   ╰──────────╯          ╰──────────╯
              ~  oi! 👋  ~
```

![ESP32-C3](https://img.shields.io/badge/MCU-ESP32--C3-E7352C?style=for-the-badge&logo=espressif&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Arduino-F5822A?style=for-the-badge&logo=platformio&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Status](https://img.shields.io/badge/status-chocando%20🐣-FFD23F?style=for-the-badge)

</div>

---

## 🐣 O que é isso?

Lembra daqueles chaveirinhos dos anos 90 com um bichinho que você precisava alimentar, brincar e colocar pra dormir? Este projeto pega essa ideia e **tira o bichinho do chaveiro**: ele vira um pequeno robô com um **ESP32-C3** e uma telinha **OLED** no lugar do rosto, e futuramente vai ganhar **motores para andar e explorar** o ambiente sozinho.

O coração do bichinho são os **olhos animados**: eles piscam sozinhos, olham ao redor com curiosidade e mudam de humor ao longo do tempo. Para andar sem se meter em encrenca, ele tem "sentidos":

- 📏 um **sensor de distância a laser (VL53L0X)** para enxergar o chão à frente e perceber a **borda da mesa antes de cair**;
- 🌀 um **acelerômetro/giroscópio (MPU6050)** para perceber quando está inclinado, tombou, foi levantado ou chacoalhado.

> 🥚 **Status:** o ovo ainda está chocando! Hoje os olhos já estão vivos e expressivos e os sensores já têm driver. Os próximos passos são os motores e a lógica de andar sem cair.

---

## 👀 Os olhos

Os olhos são desenhados como dois retângulos arredondados, e as expressões surgem de **"pálpebras" pretas** recortadas por cima deles. Tudo é interpolado suavemente a ~**50 FPS**.

| Humor | Expressão | Como é desenhado |
|:---:|:---:|---|
| 😐 `Normal` | `█  █` | Olhos abertos, sem pálpebra |
| 😊 `Happy` | `^  ^` | A metade de baixo é cortada, formando um arco |
| 😠 `Angry` | `\  /` | Canto **interno** da pálpebra abaixado |
| 😢 `Sad` | `/  \` | Canto **externo** da pálpebra abaixado |
| 😴 `Tired` | `▁  ▁` | Pálpebra reta e caída |

### ✨ Detalhes que dão vida

- **Piscada automática** a cada 5 segundos, mesmo fora do modo automático.
- **Olhar curioso:** no modo automático, ele escolhe uma das 9 direções a cada 0,8–2,5 s (o centro aparece com mais frequência, para o olhar não ficar agitado demais).
- **Mudança de humor:** a cada 4–9 s existe ~1/3 de chance de trocar para um humor diferente; no resto do tempo ele volta ao `Normal`.
- **Perspectiva:** o olho do lado para onde ele está olhando fica até 12% maior, dando sensação de profundidade.
- **Movimento suave:** posição e abertura usam interpolação exponencial, sem "teleportes".

---

## 🧰 Hardware

| Componente | Função | Endereço I²C |
|---|---|:---:|
| **ESP32-C3 DevKitM-1** | O cérebro 🧠 | — |
| **OLED SH1106 128×64** | O rosto 👀 | `0x3C` |
| **MPU6050** | O equilíbrio 🌀 | `0x68` |
| **VL53L0X** | O "detector de abismo" 📏 | `0x29` *(padrão)* |
| **Motores + driver** | As pernas 🦿 *(em breve)* | — |

### 🔌 Ligações

Todos os periféricos compartilham o mesmo barramento **I²C a 400 kHz**:

```
   ┌──────────────┐
   │   ESP32-C3   │
   │              │        ┌────────┐  ┌─────────┐  ┌─────────┐
   │       GPIO 8 ├─ SDA ──┤        ├──┤         ├──┤         │
   │       GPIO 9 ├─ SCL ──┤  OLED  ├──┤ MPU6050 ├──┤ VL53L0X │
   │          3V3 ├─ VCC ──┤ SH1106 ├──┤         ├──┤         │
   │          GND ├─ GND ──┤        ├──┤         ├──┤         │
   └──────────────┘        └────────┘  └─────────┘  └─────────┘
                    (todos em paralelo no mesmo barramento I²C)
```

| Sinal | GPIO |
|---|:---:|
| SDA | `8` |
| SCL | `9` |

> 💡 Os pinos ficam centralizados em [`src/PINNOUTS/pinnouts.cpp`](src/PINNOUTS/pinnouts.cpp). Para mudar a pinagem, basta editar ali.

---

## 🗂️ Estrutura do projeto

```
tamagotchi_esp_c3/
├── 📄 platformio.ini          → placa, flags de build e bibliotecas
├── 📁 include/
│   └── includes.h             → include central de libs e módulos
└── 📁 src/
    ├── main.cpp               → setup() e loop(): onde o bichinho nasce
    ├── 📁 PINNOUTS/           → definição dos pinos (I2C_Pins)
    ├── 📁 DISPLAY/            → inicialização do OLED SH1106
    ├── 📁 EYES/               → 👀 animação, humores e comportamento
    ├── 📁 MPU6050/            → acelerômetro + giroscópio
    └── 📁 VL53LX/             → sensor de distância a laser
```

Cada módulo expõe uma função `setupXxx()` e é incluído por meio de `includes.h`, então adicionar um "órgão" novo ao bichinho é só criar a pasta e registrar o header ali.

---

## 🚀 Como chocar o ovo

### Pré-requisitos

- [VS Code](https://code.visualstudio.com/) + extensão [PlatformIO IDE](https://platformio.org/install/ide?install=vscode)
  *ou* a [PlatformIO Core CLI](https://docs.platformio.org/en/latest/core/installation/index.html)

### Passo a passo

```bash
# 1. Clone o ninho
git clone https://github.com/<seu-usuario>/tamagotchi_esp_c3.git
cd tamagotchi_esp_c3

# 2. Compile (as bibliotecas são baixadas automaticamente)
pio run

# 3. Grave no ESP32-C3
pio run -t upload

# 4. Escute o bichinho pelo monitor serial
pio device monitor
```

> 🔧 O projeto já vem com `ARDUINO_USB_CDC_ON_BOOT=1`, então o monitor serial (115200 baud) funciona direto pela USB nativa do C3, sem adaptador.

### 📚 Bibliotecas usadas

| Biblioteca | Para quê |
|---|---|
| `adafruit/Adafruit SH110X` | Driver do OLED |
| `adafruit/Adafruit GFX Library` | Primitivas de desenho |
| `adafruit/Adafruit MPU6050` | Leitura do IMU |
| `pololu/VL53L0X` | Leitura do sensor de distância |

---

## 🎮 Controlando o bichinho

A API dos olhos ([`src/EYES/eyes.h`](src/EYES/eyes.h)) é simples e pode ser chamada de qualquer lugar:

```cpp
#include "includes.h"

void setup() {
  Serial.begin(115200);
  setupDisplay();
  setupMPU();
  setupEyes();

  eyesSetAuto(false);          // desliga o "piloto automático"
  eyesSetMood(Mood::Happy);    // ^ ^
  eyesLookAt(-0.5f, 0.3f);     // olha um pouco para a esquerda e para baixo
}

void loop() {
  updateEyes();                // chame sempre: anima e desenha os olhos
}
```

| Função | O que faz |
|---|---|
| `setupEyes()` | Inicializa os timers e desenha o primeiro quadro |
| `updateEyes()` | Deve ser chamada no `loop()`; cuida de piscada, comportamento e renderização |
| `eyesLookAt(x, y)` | Direção do olhar, com `x` e `y` entre `-1` e `1` |
| `eyesLookCenter/Left/Right/Up/Down()` | Atalhos de direção |
| `eyesBlink()` | Força uma piscada |
| `eyesSetMood(Mood)` / `eyesGetMood()` | Define ou consulta o humor |
| `eyesSetAuto(bool)` | Liga ou desliga o comportamento autônomo |

E para os sentidos:

| Função | O que faz |
|---|---|
| `setupMPU()` | Configura o MPU6050 (±4 g, ±500 °/s, filtro de 21 Hz) |
| `setupSensor()` | Inicia o VL53L0X em modo contínuo |
| `getDistancia()` | Distância em **mm**; retorna `0` em timeout ou acima de 2 m |

> ⚠️ Se algum periférico não responder no boot, o firmware avisa pelo Serial (`"Display não encontrado"`, `"MPU não encontrado!"`...) e trava ali, para facilitar o diagnóstico da fiação.

---

## 🗺️ Roadmap: a evolução do bichinho

- [x] 🥚 Setup básico do ESP32-C3 + barramento I²C
- [x] 📺 Display OLED funcionando
- [x] 👀 Olhos animados com piscada, olhar e 5 humores
- [x] 🌀 Inicialização do MPU6050
- [x] 📏 Driver do VL53L0X
- [ ] 🦿 Motores e driver: o bichinho aprende a andar
- [ ] 🕳️ Detectar a borda da mesa com o VL53L0X e recuar antes de cair
- [ ] ⚖️ Usar o MPU6050 para perceber inclinação, tombos e quando é levantado
- [ ] 🧭 Explorar sozinho, com os olhos acompanhando a direção do movimento
- [ ] 😱 Reações nos olhos: susto na borda, tontura ao ser chacoalhado
- [ ] 💤 Dormir quando ficar parado por muito tempo
- [ ] 🔋 Bateria para andar sem fio

---

<div align="center">

Feito com 💛 e muitos `delay()` evitados por **Matheus Piassi**

*Cuide bem do seu bichinho!* 🐾

</div>
