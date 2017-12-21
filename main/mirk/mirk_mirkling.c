#include "mirk_mirkling.h"

#include <tari/actorhandler.h>
#include <tari/memoryhandler.h>
#include <tari/physicshandler.h>
#include <tari/animation.h>
#include <tari/math.h>
#include <tari/input.h>
#include <tari/collisionhandler.h>
#include <tari/stagehandler.h>
#include <tari/log.h>
#include <tari/system.h>

#include "mirk_mirklinghandler.h"
#include "mirk_stage.h"
#include "mirk_collision.h"
#include "mirk_particles.h"
#include "mirk_deathcount.h"
#include "mirk_explosion.h"
#include "mirk_pussymode.h"

// TODO: find a nicer way to write this
enum { MAXIMUM_MIRKLING_AMOUNT = (int)(10000*PERFORMANCE_FACTOR) };

typedef void(*RouteHitCB)(void* tCaller, void* tCollisionData);

typedef struct {
	int mActive;
	int mPhysics;
	int mAnimation;
	int mCollision;
	Collider mCollider;
	double mSpeed;
	Position mTarget;
	Color mColor;
	TextureData* mRealTextures;
	TextureData* mUnrealTextures;
	Position* mPosition;
	Velocity* mVelocity;
	Acceleration* mAcceleration;

	MirkCollisionData mCollisionData;

	RouteHitCB mRouteHitCB;
	void* mRouteHitCaller;
} Mirkling;

static struct {
	Mirkling mMirklings[MAXIMUM_MIRKLING_AMOUNT];
	int mFreePointer;
	
	double mSpawnY;

	int mAreMirklingsInvisible;

	int mAreReal;

	Position mImpactPosition;
	double mImpactPower;
} gData;

void initMirkMirklings() {
	gData.mFreePointer = 0;
	int i;	
	for(i = 0; i < MAXIMUM_MIRKLING_AMOUNT; i++) {
		gData.mMirklings[i].mActive = 0;
	}

	gData.mSpawnY = -20;
	gData.mAreMirklingsInvisible = 0;
	gData.mAreReal = 0;
	gData.mImpactPosition = makePosition(0, 0, 0);
	gData.mImpactPower = 0;
}

static void unloadMirkling(Mirkling* e);

static void addMirklingExplosion(Position p) {
	p = vecAdd(p, makePosition(8, 8, 0));
	p.z = 5;
	addMirkExplosion(p, 8);
}

static void mirklingHitByShot(void* mCaller, void* tColData) {
	
	Mirkling* e = mCaller;
	MirkCollisionData* colData = tColData;
	Position p = *getHandledPhysicsPositionReference(e->mPhysics);

	if (colData->mType == getMirkPreciousPeopleCollisionList()) {
		addMirklingExplosion(p);
	}

	addMirkBloodParticles(vecAdd(makePosition(8, 8, 0), p), e->mColor);
	drawMirkBloodOnStage(p, e->mColor);
	if (!isInMirkPussyMode()) {
		addStageHandlerScreenShake(1);
	}
	increaseMirkDeathCount();
	
	if (e->mRouteHitCB) {
		e->mRouteHitCB(e->mRouteHitCaller, tColData);
	}
	
	unloadMirkling(e);
}

static void chooseNewBottomScreenTarget(Mirkling* e) {
	Position p = *getHandledPhysicsPositionReference(e->mPhysics);
	e->mTarget = makePosition(randfrom(40, 600), 1000, 2);
	Vector3D delta = vecNormalize(vecSub(e->mTarget, p));
	stopHandledPhysics(e->mPhysics);
	addAccelerationToHandledPhysics(e->mPhysics, vecScale(delta, e->mSpeed));
}


static Color gPossibleBloodColors[] = { COLOR_RED }; 

static void loadMirkling(Mirkling* e, Position p, double tSpeed) {
	e->mPhysics = addToPhysicsHandler(p);
	e->mPosition = getHandledPhysicsPositionReference(e->mPhysics);
	e->mVelocity = getHandledPhysicsVelocityReference(e->mPhysics);
	e->mAcceleration = getHandledPhysicsAccelerationReference(e->mPhysics);
	e->mSpeed = tSpeed;
	setHandledPhysicsMaxVelocity(e->mPhysics, e->mSpeed);
	e->mTarget = makePosition(0, 0, 0);
	e->mRealTextures = getMirkMirklingRealTextures();
	e->mUnrealTextures = getMirkMirklingWalkingTextures();
	TextureData* tex;
	if (gData.mAreReal) tex = e->mRealTextures;
	else tex = e->mUnrealTextures;
	e->mAnimation = playAnimationLoop(makePosition(0, 0, 0), tex, getMirkMirklingWalkingAnimation(), makeRectangleFromTexture(tex[0]));
	setAnimationBasePositionReference(e->mAnimation, getHandledPhysicsPositionReference(e->mPhysics));
	setAnimationScreenPositionReference(e->mAnimation, getMirkStagePositionReference());
	setAnimationCenter(e->mAnimation, makePosition(8,8,0));
	if (randfrom(0, 2) < 1) inverseAnimationVertical(e->mAnimation);
	if (gData.mAreMirklingsInvisible) {
		setAnimationScale(e->mAnimation, makePosition(0, 0, 0), makePosition(0, 0, 0));
	}
	e->mCollider = makeColliderFromCirc(makeCollisionCirc(makePosition(8,8,0), 8));
	e->mCollisionData = makeMirkCollisionData(getMirkMirklingsCollisionList());
	e->mCollision = addColliderToCollisionHandler(getMirkMirklingsCollisionList(), getHandledPhysicsPositionReference(e->mPhysics), e->mCollider, mirklingHitByShot, e, &e->mCollisionData);	
	e->mRouteHitCB = NULL;
	e->mRouteHitCaller = NULL;

	int colorAmount = (sizeof gPossibleBloodColors) / sizeof(Color);
	e->mColor = gPossibleBloodColors[randfromInteger(0, colorAmount - 1)];
	e->mActive = 1;

	increaseMirkMirklingAmount();
}



static void unloadMirkling(Mirkling* e) {
	removeFromCollisionHandler(getMirkMirklingsCollisionList(), e->mCollision);
	destroyCollider(&e->mCollider);
	removeHandledAnimation(e->mAnimation);
	removeFromPhysicsHandler(e->mPhysics);
	e->mActive = 0;
	decreaseMirkMirklingAmountOnScreen();
}

void shutdownMirklings() {
	int i;
	for(i = 0; i < MAXIMUM_MIRKLING_AMOUNT; i++) {
		if(!gData.mMirklings[i].mActive) continue;

		unloadMirkling(&gData.mMirklings[i]);
	}
}

static void updateMirklingDirection(Mirkling* e) {
	Vector3D targetDir = vecSub(e->mTarget, *e->mPosition);
	Vector3D impactDir = vecSub(*e->mPosition, gData.mImpactPosition);
	double impactScale = fmin(1, gData.mImpactPower*(1 / (vecLength(impactDir)+1e-6)));

	targetDir = normalizeVelocity(targetDir);
	impactDir = normalizeVelocity(impactDir);

	Vector3D dir = vecAdd(vecScale(targetDir, 1- impactScale), vecScale(impactDir, impactScale));
	dir = vecScale(normalizeVelocity(dir), e->mSpeed);
	*e->mVelocity = dir;
}

static void updateMirkling(Mirkling* e) {
	Position* p = getHandledPhysicsPositionReference(e->mPhysics);
	double downY = 480 + 20;

	double t = getLinearInterpolationFactor(-100, downY, p->y);
	p->z = interpolateLinear(2, 3, t);
	
	updateMirklingDirection(e);

	if (p->y > downY) {
		unloadMirkling(e);
		return;
	}
}

static void updateImpact() {
	gData.mImpactPower *= 0.975;
}

void updateMirkMirklings() {
	int i;

	for(i = 0; i < MAXIMUM_MIRKLING_AMOUNT; i++) {
		if(!gData.mMirklings[i].mActive) continue;

		updateMirkling(&gData.mMirklings[i]);
	}

	updateImpact();
}

void setMirkMirklingRouteHitCB(int tID, void(*tCB)(void *tCaller, void *tCollisionData), void * tCaller)
{
	Mirkling* e = &gData.mMirklings[tID];
	e->mRouteHitCB = tCB;
	e->mRouteHitCaller = tCaller;
}

void setMirkMirklingSpawnY(double tY)
{
	gData.mSpawnY = tY;
}

void setMirkMirklingsInvisible()
{
	gData.mAreMirklingsInvisible = 1;
}

void setMirkMirklingsVisible()
{
	gData.mAreMirklingsInvisible = 0;
}

static void setSingleMirklingReal(Mirkling* e) {
	changeAnimation(e->mAnimation, e->mRealTextures, getMirkMirklingWalkingAnimation(), makeRectangleFromTexture(e->mRealTextures[0]));
}

static void setSingleMirklingUnreal(Mirkling* e) {
	changeAnimation(e->mAnimation, e->mUnrealTextures, getMirkMirklingWalkingAnimation(), makeRectangleFromTexture(e->mUnrealTextures[0]));
}

void setMirkMirklingsReal() {
	int i;
	for (i = 0; i < MAXIMUM_MIRKLING_AMOUNT; i++) {
		if (!gData.mMirklings[i].mActive) continue;

		setSingleMirklingReal(&gData.mMirklings[i]);
	}

	gData.mAreReal = 1;
}

void setMirkMirklingsUnreal() {
	int i;
	for (i = 0; i < MAXIMUM_MIRKLING_AMOUNT; i++) {
		if (!gData.mMirklings[i].mActive) continue;

		setSingleMirklingUnreal(&gData.mMirklings[i]);
	}

	gData.mAreReal = 0;
}

void addMirkMirklingDetraction(Position p, double tPower)
{
	gData.mImpactPosition = p;
	gData.mImpactPower = tPower;
}


void invertMirkMirklingsReality()
{
	if (gData.mAreReal) setMirkMirklingsUnreal();
	else setMirkMirklingsReal();
}

Position getLowestMirkMirklingPosition()
{
	Position lowest = makePosition(0, 0, 0);
	int i;
	for (i = 0; i < MAXIMUM_MIRKLING_AMOUNT; i++) {
		Mirkling* e = &gData.mMirklings[i];
		if (!e->mActive) continue;

		Position p = *getHandledPhysicsPositionReference(e->mPhysics);
		if (p.y > lowest.y) {
			lowest = p;
		}
	}

	return lowest;
}


static int findFreeMirklingSpotIndex() {
	int i;

	for(i = 0; i < MAXIMUM_MIRKLING_AMOUNT; i++) {
		if(!gData.mMirklings[gData.mFreePointer].mActive) return gData.mFreePointer;
		gData.mFreePointer = (gData.mFreePointer + 1) % MAXIMUM_MIRKLING_AMOUNT;
	}

	logError("Unable to find new free Mirkling spot.");
	abortSystem();

	return -1;
}

void addMirkMirkling(double tSpeed)
{
	int id = findFreeMirklingSpotIndex();
	Mirkling* e = &gData.mMirklings[id];
	loadMirkling(e, makePosition(randfrom(-8, 632), gData.mSpawnY, 2), tSpeed);
	chooseNewBottomScreenTarget(e);
}

static Vector3D calculateManualMirklingTarget(Position tPos, Vector3D tDir) {
	double dy = 1000 - tPos.y;
	double dx = (dy / tDir.y) * tDir.x;

	return makePosition(tPos.x+dx, 1000, 0);
}

int addMirkMirklingManual(Position tPos, Vector3D tDir, double tSpeed)
{
	int id = findFreeMirklingSpotIndex();
	Mirkling* e = &gData.mMirklings[id];
	loadMirkling(e, tPos, tSpeed);
	addAccelerationToHandledPhysics(e->mPhysics, vecScale(tDir, tSpeed));
	e->mTarget = calculateManualMirklingTarget(tPos, tDir);
	return id;
}
