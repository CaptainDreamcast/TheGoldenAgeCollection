#pragma once

#include <prism/actorhandler.h>
#include <prism/geometry.h>

extern ActorBlueprint BeyondItemHandler;

void addBeyondSmallPowerItems(Position tPosition, int tAmount);
void addBeyondLifeItems(Position tPosition, int tAmount);
void addBeyondBombItems(Position tPosition, int tAmount);