#pragma once

#include <prism/actorhandler.h>
#include <prism/geometry.h>

extern ActorBlueprint BombxPlayer;


int isBombxPlayerAtTile(Vector3DI tTilePosition);
void explodeBombxPlayer();