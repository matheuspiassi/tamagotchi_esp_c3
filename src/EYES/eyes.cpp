#include "eyes.h"

// ---------- Geometria dos olhos ----------
static constexpr int SCREEN_W = 128;
static constexpr int SCREEN_H = 64;
static constexpr int EYE_W    = 36;   // largura
static constexpr int EYE_H    = 36;   // altura
static constexpr int EYE_R    = 8;    // raio dos cantos
static constexpr int EYE_GAP  = 20;   // espaço entre os olhos
static constexpr int MAX_DX   = 18;   // deslocamento máximo horizontal
static constexpr int MAX_DY   = 12;   // deslocamento máximo vertical
static constexpr unsigned long FRAME_MS = 20;  // ~50 quadros por segundo
static constexpr unsigned long BLINK_MS = 5000; // intervalo entre piscadas

// ---------- Estado ----------
static Mood  mood = Mood::Normal;
static float curX = 0, curY = 0, tgtX = 0, tgtY = 0;
static float openness = 1.0f, tgtOpen = 1.0f;   // 1 = aberto, 0 = fechado
static bool  autoMode = true;

static unsigned long lastFrame = 0, nextBlink = 0, nextLook = 0, nextMood = 0;

// ---------- Comandos ----------
void eyesLookAt(float x, float y) {
  x = constrain(x, -1.0f, 1.0f);
  y = constrain(y, -1.0f, 1.0f);
  tgtX = x * MAX_DX;
  tgtY = y * MAX_DY;
}
void eyesLookCenter() { eyesLookAt( 0,  0); }
void eyesLookLeft()   { eyesLookAt(-1,  0); }
void eyesLookRight()  { eyesLookAt( 1,  0); }
void eyesLookUp()     { eyesLookAt( 0, -1); }
void eyesLookDown()   { eyesLookAt( 0,  1); }

void eyesBlink()            { tgtOpen = 0; }
void eyesSetMood(Mood m)    { mood = m; }
Mood eyesGetMood()          { return mood; }
void eyesSetAuto(bool on)   { autoMode = on; }

// ---------- Desenho ----------
static void drawEye(int cx, int cy, float scaleH, bool isLeft) {
  int h = max(2, (int)(EYE_H * openness * scaleH));
  int w = EYE_W;
  int x = cx - w / 2;
  int y = cy - h / 2;
  int r = min(EYE_R, h / 2);

  display.fillRoundRect(x, y, w, h, r, SH110X_WHITE);

  if (h < 8) return;  // durante a piscada não desenha as pálpebras

  // "Pálpebras" em preto para criar as expressões
  switch (mood) {
    case Mood::Happy:   // corta a parte de baixo -> olho em arco ^ ^
      display.fillRoundRect(x - 1, y + h / 2, w + 2, h, EYE_R, SH110X_BLACK);
      break;
    case Mood::Angry:   // canto interno abaixado
      if (isLeft) display.fillTriangle(x - 1, y - 1, x + w + 1, y - 1, x + w + 1, y + h * 2 / 5, SH110X_BLACK);
      else        display.fillTriangle(x - 1, y - 1, x + w + 1, y - 1, x - 1,     y + h * 2 / 5, SH110X_BLACK);
      break;
    case Mood::Sad:     // canto externo abaixado
      if (isLeft) display.fillTriangle(x - 1, y - 1, x + w + 1, y - 1, x - 1,     y + h * 2 / 5, SH110X_BLACK);
      else        display.fillTriangle(x - 1, y - 1, x + w + 1, y - 1, x + w + 1, y + h * 2 / 5, SH110X_BLACK);
      break;
    case Mood::Tired:   // pálpebra reta caída
      display.fillRect(x - 1, y - 1, w + 2, h * 2 / 5, SH110X_BLACK);
      break;
    default:
      break;
  }
}

static void renderEyes() {
  // Interpolação suave
  curX     += (tgtX - curX) * 0.3f;
  curY     += (tgtY - curY) * 0.3f;
  openness += (tgtOpen - openness) * 0.5f;
  if (tgtOpen == 0 && openness < 0.05f) tgtOpen = 1;  // terminou de fechar -> abre

  // Leve perspectiva: o olho do lado para onde olha fica um pouco maior
  float k = curX / MAX_DX;
  float scaleL = 1.0f - 0.12f * k;
  float scaleR = 1.0f + 0.12f * k;

  int baseL = SCREEN_W / 2 - EYE_GAP / 2 - EYE_W / 2;
  int baseR = SCREEN_W / 2 + EYE_GAP / 2 + EYE_W / 2;
  int cy    = SCREEN_H / 2 + (int)curY;

  display.clearDisplay();
  drawEye(baseL + (int)curX, cy, scaleL, true);
  drawEye(baseR + (int)curX, cy, scaleR, false);
  display.display();
}

static void autoBehavior(unsigned long now) {
  // Olha para uma direção aleatória (centro aparece mais vezes)
  if (now >= nextLook) {
    static const float pos[][2] = {
      {0, 0}, {0, 0}, {-1, 0}, {1, 0}, {0, -1}, {0, 1},
      {-1, -1}, {1, -1}, {-1, 1}, {1, 1}
    };
    int i = random(0, 10);
    eyesLookAt(pos[i][0], pos[i][1]);
    nextLook = now + random(800, 2500);
  }

  // Troca de humor de vez em quando (na maior parte do tempo fica Normal)
  if (now >= nextMood) {
    mood = (random(0, 3) == 0) ? (Mood)random(1, 5) : Mood::Normal;
    nextMood = now + random(4000, 9000);
  }
}

// ---------- API principal ----------
void setupEyes() {
  randomSeed(esp_random());
  unsigned long now = millis();
  nextBlink = now + BLINK_MS;
  nextLook  = now + random(500, 1500);
  nextMood  = now + random(4000, 9000);
  renderEyes();
}

void updateEyes() {
  unsigned long now = millis();

  // Pisca a cada BLINK_MS, com ou sem o modo automático
  if (now >= nextBlink) {
    eyesBlink();
    nextBlink = now + BLINK_MS;
  }

  if (autoMode) autoBehavior(now);

  if (now - lastFrame >= FRAME_MS) {
    lastFrame = now;
    renderEyes();
  }
}