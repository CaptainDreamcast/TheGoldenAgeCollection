#include "norm_debris.h"

#include <tari/animation.h>
#include <tari/actorhandler.h>
#include <tari/math.h>
#include <tari/physicshandler.h>
#include <tari/soundeffect.h>


static struct {
	TextureData mDebrisTextures[10];
	TextureData mHeartTextures[10];

	int mExplosionSFX;
	int mHeartSFX;
} gData;

void loadNormDebris()
{
	loadConsecutiveTextures(gData.mDebrisTextures, "assets/main/norm/sprites/DEBRIS.pkg", 6);
	loadConsecutiveTextures(gData.mHeartTextures, "assets/main/norm/sprites/HEART.pkg", 3);

	gData.mExplosionSFX = loadSoundEffect("assets/main/norm/music/EXPLOSION.wav");
	gData.mHeartSFX = loadSoundEffect("assets/main/norm/music/HEART.wav");
}

typedef struct {
	int mAnimationID;
	int mPhysicsID;
	int mActor;

	double mRotation;
	double mRotationSpeed;
} Debris;

static void loadSingleDebris(void* tData) {
	Debris* e = tData;

	addAccelerationToHandledPhysics(e->mPhysicsID, makePosition(randfrom(-3, 3), randfrom(-10, -1), 0));
	setHandledPhysicsGravity(e->mPhysicsID, makePosition(0, 0.2, 0));

	setAnimationBasePositionReference(e->mAnimationID, getHandledPhysicsPositionReference(e->mPhysicsID));
	e->mRotation = randfrom(0, 1);
	e->mRotationSpeed = randfrom(-0.2, 0.2);
	setAnimationRotationZ(e->mAnimationID, e->mRotation, makePosition(16, 16, 0));
}

static void unloadDebris(void* tData) {
	Debris* e = tData;

	removeHandledAnimation(e->mAnimationID);
	removeFromPhysicsHandler(e->mPhysicsID);
}

static void updateSingleDebris(void* tData) {
	Debris* e = tData;

	e->mRotation += e->mRotationSpeed;
	setAnimationRotationZ(e->mAnimationID, e->mRotation, makePosition(16, 16, 0));

	Position p = *getHandledPhysicsPositionReference(e->mPhysicsID);

	if (p.y >= 500) {
		removeActor(e->mActor);
	}
}

ActorBlueprint DebrisBP = {
	.mLoad = loadSingleDebris,
	.mUnload = unloadDebris,
	.mUpdate = updateSingleDebris,
};


void addNormDebris(GeoRectangle tRect)
{
	playSoundEffect(gData.mExplosionSFX);

	int amount = randfromInteger(20, 30);
	int i;
	for (i = 0; i < amount; i++) {
		Debris* e = allocMemory(sizeof(Debris));
		Position p = makePosition(randfrom(tRect.mTopLeft.x, tRect.mBottomRight.x), randfrom(tRect.mTopLeft.y, tRect.mBottomRight.y), randfrom(7, 8));
		e->mPhysicsID = addToPhysicsHandler(p);
		e->mAnimationID = playOneFrameAnimationLoop(makePosition(0, 0, 0), &gData.mDebrisTextures[randfromInteger(0, 5)]);
		e->mActor = instantiateActorWithData(DebrisBP, e, 1);
	}
}


void addNormHeartDebris(GeoRectangle tRect)
{
	playSoundEffect(gData.mHeartSFX);

	int amount = randfromInteger(20, 30);
	int i;
	for (i = 0; i < amount; i++) {
		Debris* e = allocMemory(sizeof(Debris));
		Position p = makePosition(randfrom(tRect.mTopLeft.x, tRect.mBottomRight.x), randfrom(tRect.mTopLeft.y, tRect.mBottomRight.y), randfrom(7, 8));
		e->mPhysicsID = addToPhysicsHandler(p);
		e->mAnimationID = playOneFrameAnimationLoop(makePosition(0, 0, 0), &gData.mHeartTextures[randfromInteger(0, 2)]);
		e->mActor = instantiateActorWithData(DebrisBP, e, 1);
	}
}
