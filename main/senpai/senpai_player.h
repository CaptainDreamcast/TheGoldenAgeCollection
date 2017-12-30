#pragma once

#include <tari/actorhandler.h>
#include <tari/geometry.h>

extern ActorBlueprint SenpaiPlayer;

void setSenpaiPlayerToStart(Vector3DI tTilePosition);
Position getSenpaiPlayerPosition();
Vector3DI getSenpaiPlayerTilePosition();