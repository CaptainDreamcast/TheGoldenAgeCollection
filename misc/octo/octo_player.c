#include "octo_player.h"

#include <tari/physicshandler.h>
#include <tari/input.h>
#include <tari/mugenanimationhandler.h>
#include <tari/sound.h>

#include "octo_assethandler.h"
#include "octo_collision.h"

static struct {
	int mPhysicsID;
	int mAnimationID;

	double mSpeed;
} gData;

static void playerHitCB(void* tCaller, void* tCollisionData);

static void loadPlayer(void* tData) {
	(void)tData;

	gData.mPhysicsID = addToPhysicsHandler(makePosition(20, 200, 0));
	setHandledPhysicsMaxVelocity(gData.mPhysicsID, 10);

	gData.mSpeed = 3;
	setHandledPhysicsGravity(gData.mPhysicsID, makePosition(0,0,0));
	addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(gData.mSpeed, 0, 0));

	gData.mAnimationID = addMugenAnimation(getMugenAnimation(getOctoAnimations(), 1), getOctoSprites(), makePosition(0, 0, 2));
	setMugenAnimationBasePosition(gData.mAnimationID, getHandledPhysicsPositionReference(gData.mPhysicsID));
	setMugenAnimationCollisionActive(gData.mAnimationID, getOctoPlayerCollisionList(), playerHitCB, NULL, NULL);
}

static void playerHitCB(void* tCaller, void* tCollisionData) {
	(void)tCaller;
	(void)tCollisionData;
	changeMugenAnimation(gData.mAnimationID, getMugenAnimation(getOctoAnimations(), 2));
}

static void constraintPlayer() {
	Position* pos = getHandledPhysicsPositionReference(gData.mPhysicsID);
	Velocity* vel = getHandledPhysicsVelocityReference(gData.mPhysicsID);
	Velocity* acc = getHandledPhysicsAccelerationReference(gData.mPhysicsID);

	if (pos->y < 0 && vel->y < 0) pos->y = 0;
	if (pos->y >= 480 && vel->y > 0) {
		pos->y = 480;
		vel->y = 0;
		acc->y = 0;
		
	}
	
	if (pos->x < 0 && vel->x < 0) {
		pos->x = 0;
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(gData.mSpeed*2, 0, 0));
	}

	if (pos->x >= 640 && vel->x > 0) {
		pos->x = 640;
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(-gData.mSpeed*2, 0, 0));
	}
}

#ifdef DREAMCAST

static void updateUpInput() {
	double intensity = getFishingRodIntensity();

	double speed = -0.1;
	double factor = intensity - 0.3;

	addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(0, speed*factor, 0));
}
	
#else

static void updateUpInput() {
	double volume = getMicrophoneVolume();

	Velocity* vel = getHandledPhysicsVelocityReference(gData.mPhysicsID);
	double speed = 10;

	double factor = volume - 0.6;

	vel->y = -speed*factor;
}

#endif

static void updatePlayer(void* tData) {
	(void)tData;

	constraintPlayer();
	updateUpInput();
}

ActorBlueprint OctoPlayer = {
	.mLoad = loadPlayer,
	.mUpdate = updatePlayer,
};

Position getOctoPlayerPosition()
{
	return *getHandledPhysicsPositionReference(gData.mPhysicsID);
}
