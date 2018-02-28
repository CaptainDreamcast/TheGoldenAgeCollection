#pragma once

#include <prism/actorhandler.h>
#include <prism/geometry.h>

extern ActorBlueprint MirkPlayerBP;

void pauseMirkPlayerShooting();
void unpauseMirkPlayerShooting();

void setMirkPlayerShotRotation(double tAngle, Vector3D tCenter);
void addMirkPlayerShotManual(Position p, double r);