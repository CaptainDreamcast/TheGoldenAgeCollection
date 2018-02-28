#pragma once

#include <prism/actorhandler.h>
#include <prism/geometry.h>

extern ActorBlueprint BombxLevelHandler;

void resetBombxLevels();

Vector3DI getPlayerTileStartPosition();
int isBombxLevelTileFree(Vector3DI tTilePosition);
int isBombxLevelTileHole(Vector3DI tTilePosition);

void fillBombxHole();
int areAllBombxHolesFilled();
void setBombxLevelWon();
void resetBombxLevel();

int isFinalBombxLevel();