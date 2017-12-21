#pragma once

#include <tari/actorhandler.h>
#include <tari/mugenspritefilereader.h>

extern ActorBlueprint BeyondUserInterface;

void loadBeyondStageDisplay(MugenSpriteFile* mSprites);
void setBeyondPowerText(int tPower);
void setBeyondLifeText(int tLifeAmount);
void setBeyondBombText(int tBombAmount);