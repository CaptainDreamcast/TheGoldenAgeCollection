#include "luck_pirate.h"

#include <prism/log.h>
#include <prism/animation.h>
#include <prism/memoryhandler.h>
#include <prism/physicshandler.h>
#include <prism/actorhandler.h>
#include <prism/collisionhandler.h>

#include "luck_collision.h"

static struct {
	Animation mAnimation;
	TextureData mTextures[10];
} gData;

typedef struct {
	int mAnimation;
	int mPhysics;
	int mActor;
	int mCollision;
	LuckCollisionData mCollisionData;

} Pirate;

static void hasBeenHit(void* tCaller, void* tCollisionData);

static void loadPirate(void* tData) {
	Pirate* e = tData;
	e->mPhysics = addToPhysicsHandler(makePosition(-30, 200, 2));
	addAccelerationToHandledPhysics(e->mPhysics, makePosition(3, 0, 0));

	e->mAnimation = playAnimationLoop(makePosition(0, 0, 0), gData.mTextures, gData.mAnimation, makeRectangleFromTexture(*gData.mTextures));
	setAnimationBasePositionReference(e->mAnimation, getHandledPhysicsPositionReference(e->mPhysics));

	Collider c = makeColliderFromRect(makeCollisionRect(makePosition(0, 0, 0), makePosition(128, 128, 0)));
	e->mCollisionData.mList = getLuckPirateCollisionList();
	e->mCollision = addColliderToCollisionHandler(getLuckPirateCollisionList(), getHandledPhysicsPositionReference(e->mPhysics), c, hasBeenHit, e, &e->mCollisionData);
}

static void unloadPirate(void* tData) {
	Pirate* e = tData;

	removeFromCollisionHandler(getLuckPirateCollisionList(), e->mCollision);
	removeHandledAnimation(e->mAnimation);
	removeFromPhysicsHandler(e->mPhysics);
}

static void hasBeenHit(void* tCaller, void* tCollisionData) {
	Pirate* e = tCaller;
	LuckCollisionData* colData = tCollisionData;

	(void)e;
	(void)colData;
}

ActorBlueprint LuckPirateBP = {
	.mLoad = loadPirate,
	.mUnload = unloadPirate,
};

void loadLuckPirates() {
	gData.mAnimation = createEmptyAnimation();
	gData.mAnimation.mDuration = 3;
	gData.mAnimation.mFrameAmount = 2;

	loadConsecutiveTextures(gData.mTextures, "assets/misc/luck/sprites/PIRATE.pkg", gData.mAnimation.mFrameAmount);
}

void addNewLuckPirate()
{
	Pirate* e = allocMemory(sizeof(Pirate));
	e->mActor = instantiateActorWithData(LuckPirateBP, e, 1);
}
