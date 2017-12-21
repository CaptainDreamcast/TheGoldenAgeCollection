#include "beyond_continuehandler.h"

#include <tari/mugenanimationhandler.h>
#include <tari/animation.h>
#include <tari/wrapper.h>
#include <tari/framerate.h>
#include <tari/screeneffect.h>
#include <tari/input.h>
#include <tari/texthandler.h>
#include <tari/math.h>

#include "beyond_titlescreen.h"
#include "beyond_player.h"
#include "beyond_gameoverscreen.h"
#include "beyond_main.h"

static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;
	TextureData mWhiteTexture;
	int mIsActive;

	int mCountdownValueNow;
	int mSecondCountNow;

	int mBlackBackgroundID;
	int mCountdownAnimationID;
	int mValueAnimationID;

	int mTextID;
} gData;

static void loadContinueHandler(void* tData) {
	(void)tData;
	char path[1024];

	sprintf(path, "assets/main/%s/continue/CONTINUE.sff", getBeyondDirectory());
	gData.mSprites = loadMugenSpriteFileWithoutPalette(path);
	sprintf(path, "assets/main/%s/continue/CONTINUE.air", getBeyondDirectory());
	gData.mAnimations = loadMugenAnimationFile(path);

	gData.mWhiteTexture = loadTexture("$/rd/effects/white.pkg");

	gData.mIsActive = 0;
}

static void goToGameOverScreen(void* tCaller) {
	(void)tCaller;
	setNewScreen(&BeyondGameOverScreen); 
}

static void updateValue() {
	gData.mSecondCountNow++;
	
	if(gData.mSecondCountNow >= getFramerate()) {
		gData.mSecondCountNow = 0;

		if (!gData.mCountdownValueNow) {
			resumeWrapper();
			unpauseMugenAnimationHandler();
			addFadeOut(30, goToGameOverScreen, NULL);
			return;
		}
		gData.mCountdownValueNow--;
		changeMugenAnimation(gData.mValueAnimationID, getMugenAnimation(&gData.mAnimations, gData.mCountdownValueNow));
	}
}

static void continueGame() {
	resumeWrapper();
	unpauseMugenAnimationHandler();

	removeMugenAnimation(gData.mCountdownAnimationID);
	removeMugenAnimation(gData.mValueAnimationID);
	removeHandledAnimation(gData.mBlackBackgroundID);
	removeHandledText(gData.mTextID);

	reduceBeyondContinueAmount();
	setBeyondPlayerToFullPower();
	
	gData.mIsActive = 0;
}

static void updateContinueInput() {
	if (hasPressedStartFlank()) {
		continueGame();
	}
}

static void updateContinueHandler(void* tData) {
	(void)tData;

	if (!gData.mIsActive) return;

	updateValue();
	updateContinueInput();
}

ActorBlueprint BeyondContinueHandler = {
	.mLoad = loadContinueHandler,
	.mUpdate = updateContinueHandler,
};

void setBeyondContinueActive()
{
	pauseWrapper();
	pauseMugenAnimationHandler();

	gData.mBlackBackgroundID = playOneFrameAnimationLoop(makePosition(0, 0, 50), &gData.mWhiteTexture);
	setAnimationSize(gData.mBlackBackgroundID, makePosition(640, 480, 1), makePosition(0, 0, 0));
	setAnimationTransparency(gData.mBlackBackgroundID, 0.7);
	setAnimationColor(gData.mBlackBackgroundID, 0, 0, 0);

	gData.mCountdownValueNow = 10;
	gData.mCountdownAnimationID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 11), &gData.mSprites, makePosition(320, 140, 55));
	gData.mValueAnimationID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, gData.mCountdownValueNow), &gData.mSprites, makePosition(320, 240, 55));
	
	char text[1000];
	sprintf(text, "CONTINUES LEFT: %d", getBeyondContinueAmount());
	gData.mTextID = addHandledText(makePosition(200, 300, 55), text, 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, -5, 0), makePosition(INF, INF, INF), INF);

	gData.mSecondCountNow = 0;

	gData.mIsActive = 1;
}
