#include "mirk_titlescreen.h"

#include <stdio.h>

#include <tari/input.h>
#include <tari/animation.h>
#include <tari/texthandler.h>
#include <tari/math.h>
#include <tari/memoryhandler.h>
#include <tari/sound.h>

#include "../../maingamemenu.h"

#include "mirk_mirklinghandler.h"
#include "mirk_stage.h"
#include "mirk_collision.h"
#include "mirk_gamescreen.h"
#include "mirk_soundeffect.h"
#include "mirk_routehandler.h"
#include "mirk_titlescreengoof.h"
#include "mirk_endingscreen.h"

static struct {
	TextureData mTransparentBGTexture;
	int mTransparentBG;

	TextureData mBGTexture;
	int mBG;

	int mPressStartText;
	int mHasBeenGoofed;
} gData;

static void changeToGoofText() {
	removeHandledText(gData.mPressStartText);
	Position pressStartPosition = makePosition(20, 300, 13);
	gData.mPressStartText = addHandledTextWithBuildup(pressStartPosition, "Press start and I hope you learned you lesson now. I am the title screen and you are not even a screen.", 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(0, 10, 0), makePosition(600, INF, 0), INF, 60);
	setHandledTextSoundEffects(gData.mPressStartText, getMirkTextSoundEffectCollection());
}

static void changeToSeriousText() {
	removeHandledText(gData.mPressStartText);
	Position pressStartPosition = makePosition(220, 300, 13);
	gData.mPressStartText = addHandledTextWithBuildup(pressStartPosition, "Press start", 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(0, 10, 0), makePosition(600, INF, 0), INF, 60);
}

static void loadTitleScreen() {
	initMirkGameSoundEffects();

	gData.mTransparentBGTexture = loadTexture("assets/main/mirk/text/WAVE_TEXT_BG.pkg");
	gData.mTransparentBG = playOneFrameAnimationLoop(makePosition(0,0,11), &gData.mTransparentBGTexture);
	setAnimationSize(gData.mTransparentBG, makePosition(640, 480, 1), makePosition(0, 0, 0));
	
	gData.mBGTexture = loadTexture("assets/main/mirk/title/TITLE.pkg");
	gData.mBG = playAnimationLoop(makePosition(0, 0, 12), &gData.mBGTexture, createOneFrameAnimation(), makeRectangleFromTexture(gData.mBGTexture));
	
	Position pressStartPosition = makePosition(20, 300, 13);
	gData.mPressStartText = addHandledTextWithBuildup(pressStartPosition, "Press start and please don't shoot at the title screen because I am the title screen and I generally don't appreciate being shot at", 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(0, 10, 0), makePosition(600, INF, 0), INF, 60);
	setHandledTextSoundEffects(gData.mPressStartText, getMirkTextSoundEffectCollection());

	if (gData.mHasBeenGoofed) {
		changeToGoofText();
	}

	loadMirkMirklingsCollisions();
	instantiateActor(MirkStageBP);
	instantiateActor(MirkMirklingHandlerBP);
	reloadMirkRoute();
	resumeWrapper();

	if (hasMirkPlayerFinishedGame()) {
		gData.mHasBeenGoofed = 1;
		setMirkStageReal();
		setMirkMirklingsGeneratedPerFrame(0);
		changeToSeriousText();
	}
}

static void startTitleScreenGoofAndChangeText() {
	startMirkTitleScreenGoof();
	changeToGoofText();
	gData.mHasBeenGoofed = 1;
}

static void updateTitleScreen() {
	if (!gData.mHasBeenGoofed && hasShotGunFlank()) {
		startTitleScreenGoofAndChangeText();
	}
}

static Screen* getNextTitleScreenScreen() {
	if (hasPressedBFlank() || hasPressedAbortFlank()) {
		return &MainGameMenu;
	}
	else if (hasPressedStartFlank()) {
		return &MirkGameScreen;
	}

	return NULL;
}

Screen MirkTitleScreen = {
	.mLoad = loadTitleScreen,
	.mGetNextScreen = getNextTitleScreenScreen,
	.mUpdate = updateTitleScreen
};
