#include "norm_timer.h"

#include <tari/animation.h>
#include <tari/framerate.h>
#include <tari/timer.h>
#include <tari/screeneffect.h>

#include "norm_gameoverscreen.h"

static struct {
	TextureData mNumbers[10];
	TextureData mDot;

	int mTimerValue;

	int mAnimationIDs[6];
} gData;

static void gotoGameOverScreen(void* tData) {
	(void)tData;
	setNewScreen(&NormGameOverScreen);
}

static void updateTimerValue() {
	int value = gData.mTimerValue;
	int thirdValue = value % 10;
	value /= 10;
	int secondValue = value % 10;
	value /= 10;
	int firstValue = value;

	changeAnimation(gData.mAnimationIDs[0], &gData.mNumbers[firstValue], createOneFrameAnimation(), makeRectangleFromTexture(gData.mNumbers[0]));
	changeAnimation(gData.mAnimationIDs[1], &gData.mDot, createOneFrameAnimation(), makeRectangleFromTexture(gData.mNumbers[0]));
	changeAnimation(gData.mAnimationIDs[2], &gData.mNumbers[secondValue], createOneFrameAnimation(), makeRectangleFromTexture(gData.mNumbers[0]));
	changeAnimation(gData.mAnimationIDs[3], &gData.mNumbers[thirdValue], createOneFrameAnimation(), makeRectangleFromTexture(gData.mNumbers[0]));

}

static void timerCB(void* tData) {
	(void)tData;

	if (gData.mTimerValue == 0) {
		addFadeOut(30, gotoGameOverScreen, NULL);
		return;
	}

	gData.mTimerValue--;
	if (gData.mTimerValue % 100 == 99) {
		gData.mTimerValue -= 40;
	}

	updateTimerValue();

	addTimerCB(getFramerate(), timerCB, NULL);
}

static void loadNormTimer(void* tData) {
	loadConsecutiveTextures(gData.mNumbers, "assets/main/norm/sprites/NUMBER.pkg", 10);
	gData.mDot = loadTexture("assets/main/norm/sprites/DOT.pkg");

	Position pos = makePosition(400, 405, 10);
	int i;
	for (i = 0; i < 4; i++) {
		pos.x += 40;
		gData.mAnimationIDs[i] = playOneFrameAnimationLoop(pos, &gData.mNumbers[0]);
	}

	gData.mTimerValue = 245;

	updateTimerValue();
	addTimerCB(getFramerate(), timerCB, NULL);

}

ActorBlueprint NormTimer = {
	.mLoad = loadNormTimer,
};