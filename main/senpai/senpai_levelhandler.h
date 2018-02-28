#pragma once

#include <prism/actorhandler.h>
#include <prism/geometry.h>

extern ActorBlueprint SenpaiLevelHandler;

int canSenpaiPlayerGoThere(Vector3DI tTilePosition);
int canSenpaiEnemyGoThere(Vector3DI tTilePosition);
int getSenpaiLevelSizeX();
int getSenpaiLevelSizeY();
int getSenpaiCurrentLevel();

Position* getSenpaiLevelBasePositionReference();

int isSenpaiPositionCrossroads(Vector3DI tTilePosition);
Vector3DI getSenpaiPositionCrossroadsContinueDirection(Vector3DI tTilePosition);

void resetSenpaiPlayerAfterGameOver();

void gotoNextSenpaiLevel();