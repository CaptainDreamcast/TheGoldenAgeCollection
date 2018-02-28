#include "bombx_bombxhandler.h"

#include <prism/datastructures.h>
#include <prism/animation.h>
#include <prism/physicshandler.h>
#include <prism/log.h>
#include <prism/system.h>
#include <prism/tweening.h>

#include "bombx_levelhandler.h"
#include "bombx_player.h"

typedef struct {
	Vector3DI mTilePosition;
	int mPhysicsID;
	int mAnimationID;
	int mListID;

	int mFirstNumberAnimationID;
	int mSecondNumberAnimationID;

	int mTimer;	
	int mIsActive;

} ActiveBombx;

static struct {
	List mActiveBombx;

	TextureData mBombxTexture;
	TextureData mInactiveBombxTexture;
	Animation mExplodeAnimation;
	TextureData mExplodeTextures[4];

	TextureData mNumberTextures[11];

	int mActiveBombxAmount;
} gData;

static void loadBombxHandler(void* tData) {
	(void)tData;

	gData.mActiveBombx = new_list();

	gData.mBombxTexture = loadTexture("assets/bombx/bombx/BOMBX.pkg");
	gData.mInactiveBombxTexture = loadTexture("assets/bombx/bombx/BOMBX_INACTIVE.pkg");

	gData.mExplodeAnimation = createAnimation(2, 5);
	loadConsecutiveTextures(gData.mExplodeTextures, "assets/bombx/bombx/EXPLODE.pkg", gData.mExplodeAnimation.mFrameAmount);

	loadConsecutiveTextures(gData.mNumberTextures, "assets/bombx/bombx/numbers/NUM.pkg", 10);

	gData.mActiveBombxAmount = 0;
}

ActorBlueprint BombxHandler = {
	.mLoad = loadBombxHandler,
};

void addBombx(Vector3DI tTilePosition, int tTimerValue)
{
	ActiveBombx* e = allocMemory(sizeof(ActiveBombx));
	e->mTilePosition = tTilePosition;
	e->mPhysicsID = addToPhysicsHandler(makePosition(tTilePosition.x*32, tTilePosition.y*32, 0));
	e->mAnimationID = playOneFrameAnimationLoop(makePosition(0, 0, 3), &gData.mBombxTexture);
	setAnimationBasePositionReference(e->mAnimationID, getHandledPhysicsPositionReference(e->mPhysicsID));

	e->mTimer = tTimerValue;

	int val1 = e->mTimer / 10;
	e->mFirstNumberAnimationID = playOneFrameAnimationLoop(makePosition(0, 16, 4), &gData.mNumberTextures[val1]);
	setAnimationBasePositionReference(e->mFirstNumberAnimationID, getHandledPhysicsPositionReference(e->mPhysicsID));

	int val2 = e->mTimer % 10;
	e->mSecondNumberAnimationID = playOneFrameAnimationLoop(makePosition(16, 16, 4), &gData.mNumberTextures[val2]);
	setAnimationBasePositionReference(e->mSecondNumberAnimationID, getHandledPhysicsPositionReference(e->mPhysicsID));

	e->mIsActive = 1;

	e->mListID = list_push_back_owned(&gData.mActiveBombx, e);

	gData.mActiveBombxAmount++;
}

typedef struct {
	Vector3DI mTilePosition;
	int mWasFound;
	int mBombxID;

} FindBombxCaller;

static void findBombxAtTarget(void* tCaller, void* tData) {
	FindBombxCaller* caller = tCaller;
	ActiveBombx* e = tData;

	if (!e->mIsActive) return;

	int isSamePosition = e->mTilePosition.x == caller->mTilePosition.x && e->mTilePosition.y == caller->mTilePosition.y;
	if (isSamePosition) {
		caller->mWasFound = 1;
		caller->mBombxID = e->mListID;
	}
}

int isTargetTileBombx(Vector3DI tTilePosition)
{
	FindBombxCaller caller;
	caller.mTilePosition = tTilePosition;
	caller.mWasFound = 0;

	list_map(&gData.mActiveBombx, findBombxAtTarget, &caller);

	return caller.mWasFound;
}

int getBombxAtTile(Vector3DI tTilePosition)
{
	FindBombxCaller caller;
	caller.mTilePosition = tTilePosition;
	caller.mWasFound = 0;

	list_map(&gData.mActiveBombx, findBombxAtTarget, &caller);
	if (!caller.mWasFound) {
		logError("Unable to find bombx at tile position.");
		logErrorInteger(caller.mTilePosition.x);
		logErrorInteger(caller.mTilePosition.y);
		abortSystem();
	}

	return caller.mBombxID;
}

static void findInactiveBombxAtTarget(void* tCaller, void* tData) {
	FindBombxCaller* caller = tCaller;
	ActiveBombx* e = tData;
	if (e->mIsActive) return;

	int isSamePosition = e->mTilePosition.x == caller->mTilePosition.x && e->mTilePosition.y == caller->mTilePosition.y;
	if (isSamePosition) {
		caller->mWasFound = 1;
		caller->mBombxID = e->mListID;
	}
}

int isTargetTileInactiveBombx(Vector3DI tTilePosition)
{
	FindBombxCaller caller;
	caller.mTilePosition = tTilePosition;
	caller.mWasFound = 0;

	list_map(&gData.mActiveBombx, findInactiveBombxAtTarget, &caller);

	return caller.mWasFound;
}

int canBombxBeMoved(int tBombxID, int dx, int dy)
{
	ActiveBombx* e = list_get(&gData.mActiveBombx, tBombxID);

	Vector3DI target = e->mTilePosition;
	target.x += dx;
	target.y += dy;

	if (isTargetTileBombx(target)) return 0;
	if (isBombxLevelTileHole(target)) return 1;

	return isBombxLevelTileFree(target);
}

static void setBombxInactive(ActiveBombx* e) {
	changeAnimation(e->mAnimationID, &gData.mInactiveBombxTexture, createOneFrameAnimation(), makeRectangleFromTexture(gData.mInactiveBombxTexture));
	setAnimationPosition(e->mAnimationID, makePosition(0, 0, 2));

	removeHandledAnimation(e->mFirstNumberAnimationID);
	removeHandledAnimation(e->mSecondNumberAnimationID);
	
	gData.mActiveBombxAmount--;

	e->mIsActive = 0;

	fillBombxHole();
}

static void explodeBombx(ActiveBombx* e);

static void explodeBombxByID(int tID) {
	ActiveBombx* e = list_get(&gData.mActiveBombx, tID);
	explodeBombx(e);
}

static void checkExplodeNeighborBombx(Vector3DI tTilePosition, int dx, int dy) {
	tTilePosition.x += dx;
	tTilePosition.y += dy;

	if (isTargetTileBombx(tTilePosition)) {
		explodeBombxByID(getBombxAtTile(tTilePosition));
	}

	if (isBombxPlayerAtTile(tTilePosition)) {
		explodeBombxPlayer();
	}
}

static void explodeBombx(ActiveBombx* e) {
	changeAnimation(e->mAnimationID, gData.mExplodeTextures, gData.mExplodeAnimation, makeRectangleFromTexture(gData.mExplodeTextures[0]));
	setAnimationLoop(e->mAnimationID, 0);

	removeHandledAnimation(e->mFirstNumberAnimationID);
	removeHandledAnimation(e->mSecondNumberAnimationID);

	gData.mActiveBombxAmount--;

	e->mIsActive = 0;

	checkExplodeNeighborBombx(e->mTilePosition, 1, 0);
	checkExplodeNeighborBombx(e->mTilePosition, -1, 0);
	checkExplodeNeighborBombx(e->mTilePosition, 0, 1);
	checkExplodeNeighborBombx(e->mTilePosition, 0, -1);

}

static void updateSingleBombxTimer(void* tCaller, void* tData) {
	(void)tCaller;
	ActiveBombx* e = tData;

	if (!e->mIsActive) return;

	e->mTimer--;

	if (!e->mTimer) {
		explodeBombx(e);
		return;
	}

	int v1 = e->mTimer / 10;
	changeAnimation(e->mFirstNumberAnimationID, &gData.mNumberTextures[v1], createOneFrameAnimation(), makeRectangleFromTexture(gData.mNumberTextures[0]));

	int v2 = e->mTimer % 10;
	changeAnimation(e->mSecondNumberAnimationID, &gData.mNumberTextures[v2], createOneFrameAnimation(), makeRectangleFromTexture(gData.mNumberTextures[0]));
}

static void updateBombxTimers() {
	list_map(&gData.mActiveBombx, updateSingleBombxTimer, NULL);
}

static void stopMovingBombxCB(void* tCaller) {
	ActiveBombx* e = tCaller;

	if (isBombxLevelTileHole(e->mTilePosition) && !isTargetTileInactiveBombx(e->mTilePosition)) {
		setBombxInactive(e);
	}

	updateBombxTimers();

	if (areAllBombxHolesFilled() && !gData.mActiveBombxAmount) {
		setBombxLevelWon();
	}
}

void startMovingBombx(int tBombxID, int dx, int dy)
{
	ActiveBombx* e = list_get(&gData.mActiveBombx, tBombxID);
	Position* pos = getHandledPhysicsPositionReference(e->mPhysicsID);

	Duration walkDuration = 10;
	tweenDouble(&pos->x, pos->x, pos->x + dx * 32, linearTweeningFunction, walkDuration, NULL, NULL);
	tweenDouble(&pos->y, pos->y, pos->y + dy * 32, linearTweeningFunction, walkDuration, stopMovingBombxCB, e);

	e->mTilePosition.x += dx;
	e->mTilePosition.y += dy;
}
