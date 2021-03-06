#pragma once

#include <prism/actorhandler.h>
#include <prism/animation.h>

extern ActorBlueprint MirkMirklingHandlerBP;

TextureData* getMirkMirklingWalkingTextures();
TextureData* getMirkMirklingRealTextures();
Animation getMirkMirklingWalkingAnimation();

void resetMirkMirklingAmount();
void increaseMirkMirklingAmount();
void decreaseMirkMirklingAmountOnScreen();
int getMirkMirklingAmount();
int getMirkMirklingAmountOnScreen();

void pauseMirkMirklingGeneration();
void unpauseMirkMirklingGeneration();

void setMirkMirklingSpeed(double tSpeedMin, double tSpeedMax);
void setMirkMirklingsGeneratedPerFrame(double tMirklingsPerFrame);