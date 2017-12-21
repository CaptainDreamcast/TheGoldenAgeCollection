#pragma once

#include <tari/mugendefreader.h>
#include <tari/animation.h>
#include <tari/actorhandler.h>
#include <tari/mugenanimationreader.h>
#include <tari/mugenassignment.h>

typedef enum {
	ENEMY_MOVEMENT_TYPE_WAIT,
	ENEMY_MOVEMENT_TYPE_RUSH,
} EnemyMovementType;

typedef struct {
	int mType;

	MugenAssignment* mAmount;
	MugenAssignment* mStartPosition;
	MugenAssignment* mWaitPosition;
	MugenAssignment* mFinalPosition;
	EnemyMovementType mMovementType;
	MugenAssignment* mSpeed;
	MugenAssignment* mSmallPowerAmount;
	MugenAssignment* mLifeDropAmount;
	MugenAssignment* mBombDropAmount;

	MugenAssignment* mShotFrequency;
	MugenAssignment* mShotType;

	MugenAssignment* mHealth;
	MugenAssignment* mWaitDuration;
} StageEnemy;

extern ActorBlueprint BeyondEnemyHandler;

void loadBeyondEnemyTypesFromScript(MugenDefScript* tScript, MugenAnimations* tAnimations, MugenSpriteFile* tSprites);
void getBeyondCurrentEnemyIndex(char* tDst, void* tCaller);
void addBeyondEnemy(StageEnemy* tEnemy);
int getBeyondEnemyAmount();
Position getClosestBeyondEnemyPosition(Position tPosition);
Position getRandomBeyondEnemyPosition();