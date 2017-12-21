#include "best_endscreen.h"

#include <tari/animation.h>
#include <tari/input.h>
#include <tari/screeneffect.h>
#include <tari/tweening.h>

#include "../../miscgamemenu.h"

static struct {
	TextureData mFirstTexture;
	TextureData mSecondTexture;

	int mSecondAnimationID;
	double mSecondTextureAlpha;

	int mIsWinScreen;
	int mScreenStage;
} gData;

static void loadEndScreen() {
	if (gData.mIsWinScreen) {
		gData.mFirstTexture = loadTexture("assets/misc/best/WIN.pkg");
	}
	else {
		gData.mFirstTexture = loadTexture("assets/misc/best/LOSE.pkg");
	}
	playOneFrameAnimationLoop(makePosition(0, 0, 1), &gData.mFirstTexture);

	gData.mSecondTexture = loadTexture("assets/misc/best/CRASH.pkg");
	gData.mSecondAnimationID = playOneFrameAnimationLoop(makePosition(0,0,2), &gData.mSecondTexture);
	gData.mSecondTextureAlpha = 0;
	setAnimationTransparency(gData.mSecondAnimationID, gData.mSecondTextureAlpha);

	gData.mScreenStage = 0;
}

static void gotoMenuScreen(void* tCaller) {
	(void)tCaller;
	setNewScreen(&MiscGameMenu);
}

static void crashFadeInFinished(void* tCaller) {
	(void)tCaller;
	gData.mScreenStage++;
}

static void updateEndInput() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&MiscGameMenu);
	}

	if (hasPressedStartFlank()) {
		if (gData.mScreenStage == 0) {
			tweenDouble(&gData.mSecondTextureAlpha, 0, 1, quadraticTweeningFunction, 30, crashFadeInFinished, NULL);
			gData.mScreenStage++;
		}
		else if (gData.mScreenStage == 2) {
			addFadeOut(30, gotoMenuScreen, NULL);
			gData.mScreenStage++;
		}
	}
}

static void updateSecondTextureTransparency() {
	setAnimationTransparency(gData.mSecondAnimationID, gData.mSecondTextureAlpha);
}

static void updateEndScreen() {
	updateEndInput();
	updateSecondTextureTransparency();
}

Screen BestEndScreen = { 
	.mLoad = loadEndScreen,
	.mUpdate = updateEndScreen,
};

void setBestEndScreenWin()
{
	gData.mIsWinScreen = 1;
}

void setBestEndScreenLose()
{
	gData.mIsWinScreen = 0;
}
