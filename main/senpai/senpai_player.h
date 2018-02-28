#pragma once

#include <prism/actorhandler.h>
#include <prism/geometry.h>

extern ActorBlueprint SenpaiPlayer;

void setSenpaiPlayerToStart(Vector3DI tTilePosition);
Position getSenpaiPlayerPosition();
Vector3DI getSenpaiPlayerTilePosition();