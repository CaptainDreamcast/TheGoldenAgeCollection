#include "norm_enemy.h"

#include <tari/actorhandler.h>
#include <tari/animation.h>
#include <tari/physicshandler.h>
#include <tari/math.h> 

#include "norm_button.h"
#include "norm_player.h"
#include "norm_debris.h"
#include "norm_bg.h"

static struct {
	Animation mIdleAnimation;
	TextureData mIdleTexture[4][10];
	
	int mStage;
} gData;

static double gSpeeds[] = { 8, 8, 8, 8 };

void loadNormEnemies() {
	gData.mIdleAnimation = createEmptyAnimation();
	gData.mIdleAnimation.mFrameAmount = 2;
	gData.mIdleAnimation.mDuration = 4;
	loadConsecutiveTextures(gData.mIdleTexture[0], "assets/main/norm/sprites/ENEMY0_IDLE.pkg", gData.mIdleAnimation.mFrameAmount);
	loadConsecutiveTextures(gData.mIdleTexture[1], "assets/main/norm/sprites/ENEMY1_IDLE.pkg", gData.mIdleAnimation.mFrameAmount);
	loadConsecutiveTextures(gData.mIdleTexture[2], "assets/main/norm/sprites/ENEMY2_IDLE.pkg", gData.mIdleAnimation.mFrameAmount);
	loadConsecutiveTextures(gData.mIdleTexture[3], "assets/main/norm/sprites/ENEMY3_IDLE.pkg", gData.mIdleAnimation.mFrameAmount);

	gData.mStage = 0;
	setNormButtonSpeed(gSpeeds[gData.mStage]);
}

typedef struct {
	int mAnimationID;
	int mPhysicsID;
	int mActor;
	int mButton;
	int mType;
} Enemy;

static void loadEnemy(void* tData) {
	Enemy* e = tData;

	e->mType = randfromInteger(0, 3);
	e->mPhysicsID = addToPhysicsHandler(makePosition(650, 200, 2));

	e->mAnimationID = playAnimationLoop(makePosition(0,0,0), gData.mIdleTexture[e->mType], gData.mIdleAnimation, makeRectangleFromTexture(gData.mIdleTexture[e->mType][0]));
	setAnimationBasePositionReference(e->mAnimationID, getHandledPhysicsPositionReference(e->mPhysicsID));
	setAnimationScreenPositionReference(e->mAnimationID, getNormGameScreenPositionReference());

	e->mButton = addNormButton();
}

static void unloadEnemy(void* tData) {
	Enemy* e = tData;

	removeNormButton(e->mButton);
	removeHandledAnimation(e->mAnimationID);
	removeFromPhysicsHandler(e->mPhysicsID);
}

static void updateEnemy(void* tData) {
	Enemy* e = tData;
	Position p = *getHandledPhysicsPositionReference(e->mPhysicsID);
	Velocity* vel = getHandledPhysicsVelocityReference(e->mPhysicsID);
	vel->x = -gSpeeds[gData.mStage];

	wobbleNormAnimation(e->mAnimationID, makePosition(128, 256, 0));

	if (hasPressedNormButton(e->mButton)) {
		punchNormEnemy();
		addNormDebris(makeGeoRectangle(p.x+64, p.y, 100, 200));
		unloadEnemy(e);
		removeActor(e->mActor);
		return;
	}

	
	if (isNormButtonBelowRange(e->mButton)) {
		healNormPlayer();
		addNormHeartDebris(makeGeoRectangle(p.x + 64, p.y, 100, 200));
		unloadEnemy(e);
		removeActor(e->mActor);
		return;
	}
}

ActorBlueprint EnemyBP = {
	.mLoad = loadEnemy,
	.mUpdate = updateEnemy,

};


void addNormEnemy()
{
	Enemy* e = allocMemory(sizeof(Enemy));

	e->mActor = instantiateActorWithData(EnemyBP, e, 1);
}

void increaseNormEnemySpeed()
{
	gData.mStage++;
	setNormButtonSpeed(gSpeeds[gData.mStage]);
}

void decreaseNormEnemySpeed()
{
	gData.mStage--;
	setNormButtonSpeed(gSpeeds[gData.mStage]);
}
