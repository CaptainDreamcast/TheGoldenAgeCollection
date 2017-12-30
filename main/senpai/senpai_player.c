#include "senpai_player.h"

#include <tari/animation.h>
#include <tari/input.h>
#include <tari/tweening.h>

#include "senpai_levelhandler.h"
#include "senpai_enemyhandler.h"
#include "senpai_finalboss.h"

static struct {
	TextureData mPlayerTextures[3];
	TextureData mVisionTexture;

	Animation mWalkingAnimation;

	Vector3DI mTilePosition;
	Position mPosition;

	int mIsWalking;
	int mAnimationID;
	int mVisionID;
} gData;

static void loadPlayer(void* tCaller) {
	(void)tCaller;

	loadConsecutiveTextures(gData.mPlayerTextures, "assets/main/senpai/player/PLAYER.pkg", 2);
	gData.mVisionTexture = loadTexture("assets/main/senpai/player/VISION.pkg");

	gData.mWalkingAnimation = createAnimation(2, 7.5);
	gData.mTilePosition = makeVector3DI(0, 0, 0);
	gData.mPosition = makePosition(0, 0, 0);

	gData.mIsWalking = 0;
	gData.mAnimationID = playOneFrameAnimationLoop(makePosition(320-32, 240-32, 10), gData.mPlayerTextures);
	gData.mVisionID = playOneFrameAnimationLoop(makePosition(0, 0, 30), &gData.mVisionTexture);
}

static void walkingFinished(void* tCaller) {
	changeAnimation(gData.mAnimationID, gData.mPlayerTextures, createOneFrameAnimation(), makeRectangleFromTexture(gData.mPlayerTextures[0]));
	gData.mIsWalking = 0;

	if (gData.mTilePosition.y == -10) {
		gotoNextSenpaiLevel();
	}

	if (getSenpaiCurrentLevel() == 11 && gData.mTilePosition.y == getSenpaiLevelSizeY() + 1) {
		gData.mIsWalking = 1;
		startSenpaiFinalBossScene();
	}
}

static void startWalking(double* tDst, int tDelta, Vector3DI tTileOffset) {

	Vector3DI target = gData.mTilePosition;
	target.x += tTileOffset.x;
	target.y += tTileOffset.y;

	if (!canSenpaiPlayerGoThere(target)) {
		return;
	}

	gData.mTilePosition = target;

	changeAnimation(gData.mAnimationID, gData.mPlayerTextures, gData.mWalkingAnimation, makeRectangleFromTexture(gData.mPlayerTextures[0]));

	tweenDouble(tDst, *tDst, *tDst + tDelta*64, linearTweeningFunction, 15, walkingFinished, NULL);
	gData.mIsWalking = 1;
}

static void updatePlayerMovement() {
	if (gData.mIsWalking) return;
	if (isSenpaiShowingHorror()) return;

	if (hasPressedLeft()) {
		startWalking(&gData.mPosition.x, -1, makeVector3DI(-1, 0, 0));
	}
	else if (hasPressedRight()) {
		startWalking(&gData.mPosition.x, 1, makeVector3DI(1, 0, 0));
	}
	else if (hasPressedUp()) {
		startWalking(&gData.mPosition.y, -1, makeVector3DI(0, -1, 0));
	}
	else if (hasPressedDown()) {
		startWalking(&gData.mPosition.y, 1, makeVector3DI(0, 1, 0));
	}
}

static void updatePlayer(void* tCaller) {
	(void)tCaller;

	updatePlayerMovement();
}

ActorBlueprint SenpaiPlayer = {
		.mLoad = loadPlayer,
		.mUpdate = updatePlayer,
};

void setSenpaiPlayerToStart(Vector3DI tTilePosition)
{
	gData.mTilePosition = tTilePosition;
	gData.mPosition = makePosition(tTilePosition.x * 64, tTilePosition.y * 64, 0);
}

Position getSenpaiPlayerPosition()
{
	return gData.mPosition;
}

Vector3DI getSenpaiPlayerTilePosition()
{
	return gData.mTilePosition;
}
