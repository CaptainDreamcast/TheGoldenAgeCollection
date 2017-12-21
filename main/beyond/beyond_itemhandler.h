#pragma once

#include <tari/actorhandler.h>
#include <tari/geometry.h>

extern ActorBlueprint BeyondItemHandler;

void addBeyondSmallPowerItems(Position tPosition, int tAmount);
void addBeyondLifeItems(Position tPosition, int tAmount);
void addBeyondBombItems(Position tPosition, int tAmount);