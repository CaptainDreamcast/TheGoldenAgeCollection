#pragma once

#include <tari/actorhandler.h>
#include <tari/geometry.h>

extern ActorBlueprint MirkPlayerBP;

void pauseMirkPlayerShooting();
void unpauseMirkPlayerShooting();

void setMirkPlayerShotRotation(double tAngle, Vector3D tCenter);
void addMirkPlayerShotManual(Position p, double r);