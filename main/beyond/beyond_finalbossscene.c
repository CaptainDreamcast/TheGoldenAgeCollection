#include "beyond_finalbossscene.h"

#include <prism/animation.h>
#include <prism/tweening.h>
#include <prism/texthandler.h>
#include <prism/math.h>
#include <prism/timer.h>

#include "beyond_shothandler.h"
#include "beyond_player.h"

static struct {
	TextureData mWhiteTexture;

	int mBlackBackgroundID;
	int mHelpTextID;

	int mIsShowing;

	int mStage;
} gData;

char gBossMessages[][1024] = {
"WHY, WHY WON'T YOU GIVE UP?",
"TAKE THIS, I WILL DRAIN YOU OF YOUR POWERS!",
"NO, WHAT IS THIS? WHERE ARE YOU TAKING THIS POWER FROM?",
"IT CAN'T BE... YOU TWO ARE NOT ALONE, THERE IS SOMEONE ELSE HELPING YOU! CURSE YOU!",
};

static void loadSceneHandler(void* tData) {
	(void)tData;

	gData.mWhiteTexture = loadTexture("$/rd/effects/white.pkg");

	gData.mStage = 0;
	gData.mIsShowing = 0;
}

static void removeHelpTextBG(void* tCaller) {
	(void)tCaller;
	removeHandledAnimation(gData.mBlackBackgroundID);

	gData.mStage++;
	gData.mIsShowing = 0;
}

static void removeHelpText(void* tCaller) {
	(void)tCaller;
	removeHandledText(gData.mHelpTextID);

	tweenDouble(getAnimationTransparencyReference(gData.mBlackBackgroundID), 0.5, 0, quadraticTweeningFunction, 60, removeHelpTextBG, NULL);
}

static void startDrawingHelpText(void* tCaller) {
	(void)tCaller;
	gData.mHelpTextID = addHandledTextWithBuildup(makePosition(20, 200, 55), gBossMessages[gData.mStage], 0, COLOR_WHITE, makePosition(30, 30, 1), makePosition(-5, 0, 0), makePosition(620, 480, 1), INF, 300);
	addTimerCB(500, removeHelpText, NULL);
}

static void showFinalBossHelpText() {
	if (gData.mIsShowing || gData.mStage >= 4) return;

	gData.mBlackBackgroundID = playOneFrameAnimationLoop(makePosition(0, 0, 50), &gData.mWhiteTexture);
	setAnimationColorType(gData.mBlackBackgroundID, COLOR_BLACK);
	setAnimationSize(gData.mBlackBackgroundID, makePosition(640, 480, 1), makePosition(0, 0, 0));
	setAnimationTransparency(gData.mBlackBackgroundID, 0);

	tweenDouble(getAnimationTransparencyReference(gData.mBlackBackgroundID), 0, 0.5, quadraticTweeningFunction, 60, startDrawingHelpText, NULL);

	if (gData.mStage == 1) {
		drainBeyondPlayerPower(showFinalBossHelpText);
	}
	else if (gData.mStage == 2) {
		fillBeyondPlayerPower(showFinalBossHelpText);
	}

	gData.mIsShowing = 1;
}

static void updateShowingHelpText() {
	if (gData.mIsShowing) return;

	int bossShots = getBeyondFinalBossShotsDeflected();
	if (gData.mStage == 0 && bossShots >= 100) {
		showFinalBossHelpText();
	}
	else if (gData.mStage == 1 && bossShots >= 200) {
		showFinalBossHelpText();
	}
}

static void updateSceneHandler(void* tData) {

	updateShowingHelpText();
	
}

ActorBlueprint BeyondFinalBossSceneHandler = {
	.mLoad = loadSceneHandler,
	.mUpdate = updateSceneHandler,
};