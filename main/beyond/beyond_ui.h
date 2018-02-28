#pragma once

#include <prism/actorhandler.h>
#include <prism/mugenspritefilereader.h>

extern ActorBlueprint BeyondUserInterface;

void loadBeyondStageDisplay(MugenSpriteFile* mSprites);
void setBeyondPowerText(int tPower);
void setBeyondLifeText(int tLifeAmount);
void setBeyondBombText(int tBombAmount);