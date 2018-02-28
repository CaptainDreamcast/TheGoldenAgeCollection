#include "luck_door.h"

#include <prism/animation.h>
#include <prism/math.h>
#include <prism/tweening.h>

static struct {

	TextureData mTexture;
	int mAnimation;
	Duration mKnockNow;
	Duration mKnockDuration;
	double mScale;
} gData;

static void loadDoor(void* tData) {
	(void)tData;
	gData.mTexture = loadTexture("assets/misc/luck/sprites/DOOR.pkg");
	gData.mAnimation = playOneFrameAnimationLoop(makePosition(20, 105, 5), &gData.mTexture);
	gData.mKnockNow = 0;
	gData.mKnockDuration = 10;
	gData.mScale = 1;
}

static void unloadDoor(void* tData) {
	(void)tData;
	removeHandledAnimation(gData.mAnimation);
}



static void updateDoor(void* tData) {
	(void)tData;

	setAnimationScale(gData.mAnimation, makePosition(gData.mScale, gData.mScale, 1), makePosition(32, 128, 0));

	if (handleDurationAndCheckIfOver(&gData.mKnockNow, gData.mKnockDuration)) {
		gData.mKnockNow = 0;
		gData.mKnockDuration = randfromInteger(15, 60);
		tweenDouble(&gData.mScale, 1.5, 1, transformAtEndTweeningFunction, 10, NULL, NULL);
	}
}



ActorBlueprint LuckDoorBP = {
	.mLoad = loadDoor,
	.mUnload = unloadDoor,
	.mUpdate = updateDoor,
};