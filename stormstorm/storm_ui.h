#pragma once

#include <prism/actorhandler.h>
#include <prism/geometry.h>

extern ActorBlueprint StormUIHandler;

void resetStormUI();
void increaseStormScore();
void increaseStormEnemiesMissed();
void setStormUIScorePosition(Position tPosition);