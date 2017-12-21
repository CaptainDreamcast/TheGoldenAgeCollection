#include "luck_finalboss.h"

#include <tari/actorhandler.h>
#include <tari/animation.h>
#include <tari/collisionhandler.h>

#include "luck_collision.h"

static struct {
	TextureData mTexture;
	Collider mCollider;

	LuckCollisionData mCollisionData;
	int mAmount;
} gData;

void loadLuckFinalBosses() {
	gData.mTexture = loadTexture("assets/misc/luck/sprites/PIRATE0.pkg");
	gData.mCollider = makeColliderFromRect(makeCollisionRect(makePosition(54,0,0), makePosition(74,128,0)));
	gData.mCollisionData.mList = getLuckPirateCollisionList();
	gData.mAmount = 0;
}



typedef struct {
	Position mPosition;
	int mAnimation;
	int mCollision;

	int mActor;
} FinalBoss;

static void finalBossWasHit(void* tCaller, void* tCollisionData) {
	(void)tCollisionData;
	FinalBoss* e = tCaller;
	
	removeActor(e->mActor);
}

static void loadFinalBoss(void* tData) {
	FinalBoss* e = tData;
	e->mAnimation = playOneFrameAnimationLoop(e->mPosition, &gData.mTexture);
	e->mCollision = addColliderToCollisionHandler(getLuckPirateCollisionList(), &e->mPosition, gData.mCollider, finalBossWasHit, e, &gData.mCollisionData);

	gData.mAmount++;
}

static void unloadFinalBoss(void* tData) {
	FinalBoss* e = tData;
	removeHandledAnimation(e->mAnimation);
	removeFromCollisionHandler(getLuckPirateCollisionList(), e->mCollision);

	gData.mAmount--;
}

ActorBlueprint LuckFinalBossBP = {
	.mLoad = loadFinalBoss,
	.mUnload = unloadFinalBoss,
};

void addLuckFinalBoss(Position tPosition)
{
	double y = tPosition.y;
	tPosition.z = 2 + (y - (-64)) / (480 - (-64.0));

	FinalBoss* e = allocMemory(sizeof(FinalBoss));
	e->mPosition = tPosition;
	e->mActor = instantiateActorWithData(LuckFinalBossBP, e, 1);
}

int getLuckFinalBossAmount()
{
	return gData.mAmount;
}
