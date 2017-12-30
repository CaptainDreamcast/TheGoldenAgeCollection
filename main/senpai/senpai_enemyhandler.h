#pragma once

#include <tari/actorhandler.h>
#include <tari/geometry.h>
#include <tari/mugenanimationhandler.h>

extern ActorBlueprint SenpaiEnemyHandler;

void addSenpaiEnemy();
void unloadSenpaiEnemies();
int doesContainSenpaiEnemy(Vector3DI tTilePosition);

int isSenpaiShowingHorror();
void addSenpaiFinalBoss(Vector3DI tTilePosition);
MugenSpriteFile* getSenpaiEnemySprites();
MugenAnimations* getSenpaiEnemyAnimations();