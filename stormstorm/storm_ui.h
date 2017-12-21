#pragma once

#include <tari/actorhandler.h>
#include <tari/geometry.h>

extern ActorBlueprint StormUIHandler;

void resetStormUI();
void increaseStormScore();
void increaseStormEnemiesMissed();
void setStormUIScorePosition(Position tPosition);