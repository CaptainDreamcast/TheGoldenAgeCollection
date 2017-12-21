#pragma once 

#include <tari/geometry.h>
#include <tari/actorhandler.h>

extern ActorBlueprint BombxHandler;

void addBombx(Vector3DI tTilePosition, int tTimerValue);

int isTargetTileBombx(Vector3DI tTilePosition);
int getBombxAtTile(Vector3DI tTilePosition);

int isTargetTileInactiveBombx(Vector3DI tTilePosition);

int canBombxBeMoved(int tBombxID, int dx, int dy);

void startMovingBombx(int tBombxID, int dx, int dy);