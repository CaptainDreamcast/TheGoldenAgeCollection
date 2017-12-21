#include "medusa_shothandler.h"

#include <tari/animation.h>
#include <tari/datastructures.h>
#include <tari/physicshandler.h>
#include <tari/collisionhandler.h>

#include "medusa_collision.h"

typedef struct {
	int mPhysicsID;
	int mAnimationID;
	int mCollisionID;
	int mListID;
} Shot;

static struct {
	IntMap mShots;
	TextureData mTexture;
} gData;

static void loadShotHandler(void* tData) {
	(void)tData;
	gData.mTexture = loadTexture("assets/misc/medusa/SHOT.pkg");
	gData.mShots = new_int_map();
}

static void unloadShot(Shot* e) {
	removeFromCollisionHandler(getMedusaShotCollisionList(), e->mCollisionID);
	removeHandledAnimation(e->mAnimationID);
	removeFromPhysicsHandler(e->mPhysicsID);
}

static int updateSingleShot(void* tCaller, void* tData) {
	(void)tCaller;
	Shot* e = tData;

	Position p = *getHandledPhysicsPositionReference(e->mPhysicsID);

	if (p.x > 700) {
		unloadShot(e);
		return 1;
	}

	return 0;
}

static void updateShotHandler(void* tData) {
	(void)tData;
	int_map_remove_predicate(&gData.mShots, updateSingleShot, NULL);
}

ActorBlueprint MedusaShotHandler = {
	.mLoad = loadShotHandler,
	.mUpdate = updateShotHandler,
};



static void shotHitCB(void* tCaller, void* tCollisionData) {
	(void)tCollisionData;
	Shot* e = tCaller;
	unloadShot(e);
	int_map_remove(&gData.mShots, e->mListID);
}

void addMedusaShot(Position p)
{
	Shot* e = allocMemory(sizeof(Shot));
	e->mPhysicsID = addToPhysicsHandler(p);
	addAccelerationToHandledPhysics(e->mPhysicsID, makePosition(1, 0, 0));
	e->mAnimationID = playOneFrameAnimationLoop(makePosition(0,0,4), &gData.mTexture);
	setAnimationBasePositionReference(e->mAnimationID, getHandledPhysicsPositionReference(e->mPhysicsID));
	
	CollisionRect rect = makeCollisionRect(makePosition(0, 0, 0), makePosition(18, 29, 1));
	e->mCollisionID = addCollisionRectangleToCollisionHandler(getMedusaShotCollisionList(), getHandledPhysicsPositionReference(e->mPhysicsID), rect, shotHitCB, e, NULL);

	e->mListID = int_map_push_back_owned(&gData.mShots, e);
}
