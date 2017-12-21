#include "mirk_baftascreen.h"

#include <stdio.h>
#include <string.h>

#include <tari/input.h>
#include <tari/animation.h>
#include <tari/tweening.h>
#include <tari/timer.h>
#include <tari/screeneffect.h>

#include "mirk_titlescreen.h"

static struct {
	TextureData mMirklingsBGTexture;
	TextureData mHumanBGTexture;

	int mMirklingsBG;
	int mHumanBG;
	double mHumanTransparency;
} gData;

static void startFadeOver(void* tCaller) {
	(void)tCaller;
	tweenDouble(&gData.mHumanTransparency, 0, 1, quadraticTweeningFunction, 60, NULL, NULL);
}

static void loadBaftaScreen() {
	gData.mMirklingsBGTexture = loadTexture("assets/main/mirk/ending/ENDING_MIRKLINGS.pkg");
	gData.mHumanBGTexture = loadTexture("assets/main/mirk/ending/ENDING_HUMAN.pkg");

	gData.mMirklingsBG = playAnimationLoop(makePosition(0,0,1), &gData.mMirklingsBGTexture, createOneFrameAnimation(), makeRectangleFromTexture(gData.mMirklingsBGTexture));
	gData.mHumanBG = playAnimationLoop(makePosition(0, 0, 2), &gData.mHumanBGTexture, createOneFrameAnimation(), makeRectangleFromTexture(gData.mMirklingsBGTexture));
	gData.mHumanTransparency = 0;
	setAnimationTransparency(gData.mHumanBG, gData.mHumanTransparency);
	addTimerCB(180, startFadeOver, NULL);
	addFadeIn(20, NULL, NULL);
}

static void updateBaftaScreen() {
	setAnimationTransparency(gData.mHumanBG, gData.mHumanTransparency);
}

static void goToTitle(void* tCaller) {
	(void)tCaller;
	setNewScreen(&MirkTitleScreen);
}

static Screen* getNextBaftaScreenScreen() {
	if (hasPressedAbortFlank()) {
		return &MirkTitleScreen;
	}

	if (hasPressedStartFlank()) {
		addFadeOut(20, goToTitle, NULL);
	}

	return NULL;
}

Screen MirkBaftaScreen = {
	.mLoad = loadBaftaScreen,
	.mUpdate = updateBaftaScreen,
	.mGetNextScreen = getNextBaftaScreenScreen,
};
