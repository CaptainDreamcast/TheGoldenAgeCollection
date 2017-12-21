#include "medusa_badasshandler.h"

#include <tari/datastructures.h>
#include <tari/animation.h>
#include <tari/physicshandler.h>
#include <tari/collisionhandler.h>
#include <tari/math.h>

#include "medusa_collision.h"

typedef struct Badass_internal {
	void(*mUpdate)(struct Badass_internal*);

	Position mPosition;
	int mAnimationID;
	int mCollisionID;
	int mListID;

	Duration mTime;
} Badass;

static struct {
	IntMap mEnemies;

	TextureData mBadassTexture;

	Animation mDeathAnimation;
	TextureData mDeathTextures[3];

	double mSpeed;
} gData;

static void addEnemy(void(*tFunc)(Badass*), Position tPosition);

static void updateFirstBadass(Badass* e);
static void updateSecondBadass(Badass* e);
static void updateThirdBadass(Badass* e);


static void loadBadassHandler(void* tData) {
	(void)tData;
	gData.mBadassTexture = loadTexture("assets/misc/medusa/BADASS.pkg");
	gData.mDeathAnimation = createAnimation(2, 30);
	loadConsecutiveTextures(gData.mDeathTextures, "assets/misc/medusa/DYING.pkg", gData.mDeathAnimation.mFrameAmount);

	gData.mSpeed = 0.5;

	gData.mEnemies = new_int_map();

	addEnemy(updateFirstBadass, makePosition(500, 50, 0));
	addEnemy(updateSecondBadass, makePosition(500, 150, 0));
	addEnemy(updateThirdBadass, makePosition(500, 250, 0));	
}

static void unloadEnemy(Badass* e);

static void badassHitCB(void* tCaller, void* tCollisionData) {
	(void)tCollisionData;
	Badass* e = tCaller;
	playAnimation(e->mPosition, gData.mDeathTextures, gData.mDeathAnimation, makeRectangleFromTexture(gData.mDeathTextures[0]), NULL, NULL);

	unloadEnemy(e);
	int_map_remove(&gData.mEnemies, e->mListID);
}

void addEnemy(void(*tFunc)(Badass*), Position tPosition) {
	Badass* e = allocMemory(sizeof(Badass));
	e->mUpdate = tFunc;
	e->mPosition = tPosition;

	e->mAnimationID = playOneFrameAnimationLoop(makePosition(0,0,2), &gData.mBadassTexture);
	setAnimationBasePositionReference(e->mAnimationID, &e->mPosition);

	CollisionRect rect = makeCollisionRect(makePosition(57, 47, 0), makePosition(292, 154, 0));
	e->mCollisionID = addCollisionRectangleToCollisionHandler(getMedusaBadassCollisionList(), &e->mPosition, rect, badassHitCB, e, NULL);

	e->mTime = 0;
	e->mListID = int_map_push_back_owned(&gData.mEnemies, e);
}

void unloadEnemy(Badass* e) {
	removeFromCollisionHandler(getMedusaBadassCollisionList(), e->mCollisionID);
	removeHandledAnimation(e->mAnimationID);
}

static void updateFirstBadass(Badass* e) {
	e->mPosition.x-=gData.mSpeed;
}

static void updateSecondBadass(Badass* e) {
	e->mPosition.x-= gData.mSpeed;
	e->mPosition.y+= gData.mSpeed / 2;
}

static void updateThirdBadass(Badass* e) {
	handleDurationAndCheckIfOver(&e->mTime, INF);

	int loopBegin = 240;
	int loopDuration = 360;
	if (isDurationOver(e->mTime, loopBegin+loopDuration)) {
		e->mPosition.x-= gData.mSpeed;
	}
	else if (isDurationOver(e->mTime, loopBegin)) {
	
		double angle = 2*M_PI*((e->mTime - loopBegin) / (double)loopDuration);
		Velocity vel = vecRotateZ(makePosition(-gData.mSpeed, 0, 0), angle);
		e->mPosition = vecAdd(e->mPosition, vel);
	}
	else {
		e->mPosition.x-= gData.mSpeed;
	}
	
}

static void updateSingleBadass(void* tCaller, void* tData) {
	(void)tCaller;
	Badass* e = tData;

	e->mUpdate(e);
}

static void updateBadassHandler(void* tData) {
	(void)tData;
	int_map_map(&gData.mEnemies, updateSingleBadass, NULL);

}

ActorBlueprint MedusaBadassHandler = {
	.mLoad = loadBadassHandler,
	.mUpdate = updateBadassHandler,
};