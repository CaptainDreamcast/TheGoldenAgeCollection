#include "medusa_shothandler.h"

#include <prism/animation.h>
#include <prism/datastructures.h>
#include <prism/physicshandler.h>
#include <prism/collisionhandler.h>

#include "medusa_collision.h"
#include "medusa_resources.h"

typedef struct {
	int mPhysicsID;
	int mAnimationID;
	int mCollisionID;
	int mListID;
} Shot;

static struct {
	IntMap mShots;
} gData;

static void loadShotHandler(void* tData) {
	(void)tData;
	gData.mShots = new_int_map();
}

static void unloadShot(Shot* e) {
	removeFromCollisionHandler(getMedusaShotCollisionList(), e->mCollisionID);
	removeMugenAnimation(e->mAnimationID);
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

	e->mAnimationID = addMugenAnimation(getMugenAnimation(getMedusaAnimations(), 30), getMedusaSprites(), makePosition(0, 0, 4));
	setMugenAnimationBasePosition(e->mAnimationID, getHandledPhysicsPositionReference(e->mPhysicsID));
	
	CollisionRect rect = makeCollisionRect(makePosition(0, 0, 0), makePosition(18, 29, 1));
	e->mCollisionID = addCollisionRectangleToCollisionHandler(getMedusaShotCollisionList(), getHandledPhysicsPositionReference(e->mPhysicsID), rect, shotHitCB, e, NULL);

	e->mListID = int_map_push_back_owned(&gData.mShots, e);
}
