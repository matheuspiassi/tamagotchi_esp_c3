#ifndef EYES_H
#define EYES_H

#include "includes.h"

enum class Mood { Normal, Happy, Angry, Sad, Tired };

void setupEyes();

void updateEyes();

// Direção do olhar: x e y de -1 a 1 (x: -1 esquerda, 1 direita | y: -1 cima, 1 baixo)
void eyesLookAt(float x, float y);
void eyesLookCenter();
void eyesLookLeft();
void eyesLookRight();
void eyesLookUp();
void eyesLookDown();

void eyesBlink();

void eyesSetMood(Mood m);
Mood eyesGetMood();

// Liga/desliga o comportamento automático (olhar em volta, piscar, mudar humor)
void eyesSetAuto(bool enabled);

#endif