#pragma once

#include <prism/actorhandler.h>
#include <prism/geometry.h>
#include <prism/mugenanimationhandler.h>

extern ActorBlueprint SenpaiEnemyHandler;

void addSenpaiEnemy();
void unloadSenpaiEnemies();
int doesContainSenpaiEnemy(Vector3DI tTilePosition);

int isSenpaiShowingHorror();
void addSenpaiFinalBoss(Vector3DI tTilePosition);
MugenSpriteFile* getSenpaiEnemySprites();
MugenAnimations* getSenpaiEnemyAnimations();