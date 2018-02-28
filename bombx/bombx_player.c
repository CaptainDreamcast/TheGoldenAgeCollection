#include "bombx_player.h"

#include <prism/animation.h>
#include <prism/physicshandler.h>
#include <prism/input.h>
#include <prism/tweening.h>
#include <prism/screeneffect.h>


#include "bombx_levelhandler.h"
#include "bombx_bombxhandler.h"
#include "bombx_gamescreen.h"
#include "bombx_congratsscreen.h"

static struct {
	Animation mWalkingAnimation;
	TextureData mUpTextures[3];
	TextureData mLeftTextures[3];

	TextureData mExplodeUpTexture;
	TextureData mExplodeLeftTexture;

	TextureData* mCurrentTextures;
	
	Vector3DI mTilePosition;
	int mPhysicsID;
	int mAnimationID;

	int mIsWalking;
	int mIsAlive;

} gData;

static void loadBombxPlayer(void* tData) {
	(void)tData;

	gData.mWalkingAnimation = createAnimation(2, 5);
	loadConsecutiveTextures(gData.mUpTextures, "assets/bombx/player/PLAYER_UP.pkg", gData.mWalkingAnimation.mFrameAmount);
	loadConsecutiveTextures(gData.mLeftTextures, "assets/bombx/player/PLAYER_LEFT.pkg", gData.mWalkingAnimation.mFrameAmount);
	gData.mExplodeUpTexture = loadTexture("assets/bombx/player/PLAYER_EXPLODE_UP.pkg");
	gData.mExplodeLeftTexture = loadTexture("assets/bombx/player/PLAYER_EXPLODE_LEFT.pkg");
	
	gData.mCurrentTextures = gData.mUpTextures;
	
	gData.mTilePosition = getPlayerTileStartPosition();
	gData.mPhysicsID = addToPhysicsHandler(makePosition(gData.mTilePosition.x * 32, gData.mTilePosition.y * 32, 0));

	gData.mAnimationID = playOneFrameAnimationLoop(makePosition(0, 0, 3), gData.mCurrentTextures);
	setAnimationBasePositionReference(gData.mAnimationID, getHandledPhysicsPositionReference(gData.mPhysicsID));
	setAnimationCenter(gData.mAnimationID, makePosition(16,16,0));

	inverseAnimationHorizontal(gData.mAnimationID);

	gData.mIsWalking = 0;
	gData.mIsAlive = 1;
}

static void walkingFinishedCB(void* tCaller) {
	(void)tCaller;
	if (!gData.mIsAlive) return;

	changeAnimation(gData.mAnimationID, gData.mCurrentTextures, createOneFrameAnimation(), makeRectangleFromTexture(gData.mCurrentTextures[0]));
	gData.mIsWalking = 0;
}

static int canWalkThere(Vector3DI tTilePosition, int dx, int dy) {
	Vector3DI target = tTilePosition;
	target.x += dx;
	target.y += dy;

	if (isTargetTileBombx(target)) return canBombxBeMoved(getBombxAtTile(target), dx, dy);
	if (isTargetTileInactiveBombx(target)) return 1;

	return isBombxLevelTileFree(target);

}

static void startWalking(TextureData* newTextures, int dx, int dy) {
	if (!canWalkThere(gData.mTilePosition, dx, dy)) return;

	Position* pos = getHandledPhysicsPositionReference(gData.mPhysicsID);

	changeAnimation(gData.mAnimationID, newTextures, gData.mWalkingAnimation, makeRectangleFromTexture(newTextures[0]));
	setAnimationHorizontalInversion(gData.mAnimationID, dy == 1);
	setAnimationVerticalInversion(gData.mAnimationID, dx == 1);
	gData.mCurrentTextures = newTextures;

	

	gData.mTilePosition.x += dx;
	gData.mTilePosition.y += dy;

	if (isTargetTileBombx(gData.mTilePosition)) {
		startMovingBombx(getBombxAtTile(gData.mTilePosition), dx, dy);
	}

	Duration walkDuration = 10;
	tweenDouble(&pos->x, pos->x, pos->x + dx* 32, linearTweeningFunction, walkDuration, NULL, NULL);
	tweenDouble(&pos->y, pos->y, pos->y + dy * 32, linearTweeningFunction, walkDuration, walkingFinishedCB, NULL);

	gData.mIsWalking = 1;
}

static void updateWalkingInput() {
	if (gData.mIsWalking) return;
	if (!gData.mIsAlive) return;

	if (hasPressedLeft()) {
		startWalking(gData.mLeftTextures, -1, 0);
	}
	else if (hasPressedRight()) {
		startWalking(gData.mLeftTextures, 1, 0);
	}
	else if (hasPressedUp()) {
		startWalking(gData.mUpTextures, 0, -1);
	}
	else if (hasPressedDown()) {
		startWalking(gData.mUpTextures, 0, 1);
	}
}

static void updateBombxPlayer(void* tData) {
	(void)tData;

	updateWalkingInput();

}

ActorBlueprint BombxPlayer = {
	.mLoad = loadBombxPlayer,
	.mUpdate = updateBombxPlayer,
};

int isBombxPlayerAtTile(Vector3DI tTilePosition)
{
	return gData.mTilePosition.x == tTilePosition.x && gData.mTilePosition.y == tTilePosition.y;
}

static void resetLevelCB(void* tCaller) {
	(void)tCaller;
	setAnimationCB(gData.mAnimationID, NULL, NULL);
	
	resetBombxLevel();
}

static void gotoCongratsScreen(void* tCaller) {
	(void)tCaller;
	setNewScreen(&BombxCongratsScreen);
}

void explodeBombxPlayer()
{
	gData.mIsAlive = 0;

	if (isFinalBombxLevel()) {
		changeAnimation(gData.mAnimationID, gData.mCurrentTextures, createOneFrameAnimation(), makeRectangleFromTexture(gData.mCurrentTextures[0]));
		addFadeOut(10, gotoCongratsScreen, NULL);
		return;
	}

	TextureData* textures;

	if (gData.mCurrentTextures == gData.mUpTextures) {
		textures = &gData.mExplodeUpTexture;
	}
	else {
		textures = &gData.mExplodeLeftTexture;
	}

	changeAnimation(gData.mAnimationID, textures, createAnimation(1, 60), makeRectangleFromTexture(*textures));
	setAnimationCB(gData.mAnimationID, resetLevelCB, NULL);
}
