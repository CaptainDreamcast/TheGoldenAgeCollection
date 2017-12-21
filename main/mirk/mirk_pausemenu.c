#include "mirk_pausemenu.h"

#include <tari/optionhandler.h>
#include <tari/animation.h>
#include <tari/wrapper.h>
#include <tari/input.h>
#include <tari/sound.h>
#include <tari/soundeffect.h>

#include "mirk_pussymode.h"
#include "mirk_titlescreen.h"

static struct {
	int mContinue;
	int mPussyMode;
	int mToTitle;
	TextureData mBGTexture;
	int mBG;
		
	int mIsMuted;
	int mToggleSound;

	int mOptionX;

	int mIsPausingPossible;
	int mIsPaused;
} gData;

void setMirkPauseMenuImpossible()
{
	gData.mIsPausingPossible = 0;
}

void setMirkPauseMenuPossible()
{
	gData.mIsPausingPossible = 1;
}

static void loadPauseMenu(void* tData) {
	(void)tData;

	setupOptionHandler();
	setOptionButtonA();
	setOptionButtonStart();
	setOptionTextSize(17);
	setOptionTextBreakSize(-5);
	gData.mBGTexture = loadTexture("assets/debug/collision_rect.pkg");
	gData.mOptionX = 140;

	gData.mIsPaused = 0;
	gData.mIsPausingPossible = 1;
}

static void stopPauseMenu();

static void selectContinue(void* tCaller) {
	(void)tCaller;
	stopPauseMenu();
}

static void selectTitle(void* tCaller) {
	(void) tCaller;
	setNewScreen(&MirkTitleScreen);
}

static void selectPussyModeOff(void* tCaller);

static void selectPussyModeOn(void* tCaller) {
	(void)tCaller;
	setMirkPussyModeOn();
	removeOption(gData.mPussyMode);
	gData.mPussyMode = addOption(makePosition(gData.mOptionX, 310, 40), "Turn pussy mode OFF", selectPussyModeOff, NULL);
}

static void selectPussyModeOff(void* tCaller) {
	(void)tCaller;
	setMirkPussyModeOff();
	removeOption(gData.mPussyMode);
	gData.mPussyMode = addOption(makePosition(gData.mOptionX, 310, 40), "Turn pussy mode ON", selectPussyModeOn, NULL);
}

static void turnSoundOn() {

	setVolume(1);
	setSoundEffectVolume(1);
	gData.mIsMuted = 0;
}

static void turnSoundOff() {
	setVolume(0);
	setSoundEffectVolume(0);
	gData.mIsMuted = 1; 
}

static void selectToggleSound(void* tCaller) {
	(void)tCaller;

	if (gData.mIsMuted) turnSoundOn();
	else turnSoundOff();

}

static void startPauseMenu() {

	gData.mBG = playOneFrameAnimationLoop(makePosition(100, 200, 39), &gData.mBGTexture);
	setAnimationColorType(gData.mBG, COLOR_BLACK);
	setAnimationSize(gData.mBG, makePosition(440, 140, 1), makePosition(0, 0, 0));
	gData.mContinue = addOption(makePosition(gData.mOptionX, 220, 40), "Continue", selectContinue, NULL);
	gData.mToTitle = addOption(makePosition(gData.mOptionX, 250, 40), "Return to title", selectTitle, NULL);
	gData.mToggleSound = addOption(makePosition(gData.mOptionX, 280, 40), "Toggle sound", selectToggleSound, NULL);

	if (isInMirkPussyMode()) {
		gData.mPussyMode = addOption(makePosition(gData.mOptionX, 310, 40), "Turn pussy mode OFF", selectPussyModeOff, NULL);
	}
	else {
		gData.mPussyMode = addOption(makePosition(gData.mOptionX, 310, 40), "Turn pussy mode ON", selectPussyModeOn, NULL);
	}

	pauseWrapper();
	gData.mIsPaused = 1;
}

static void stopPauseMenu() {
	removeHandledAnimation(gData.mBG);
	removeOption(gData.mContinue);
	removeOption(gData.mToTitle);
	removeOption(gData.mToggleSound);
	removeOption(gData.mPussyMode);

	resumeWrapper();
	gData.mIsPaused = 0;
}

static void updatePauseMenu(void* tData) {
	(void)tData;

	if (!gData.mIsPausingPossible) return;


	if (!gData.mIsPaused && hasPressedStartFlank()) {
		startPauseMenu();
	}

	if (gData.mIsPaused) {
		updateOptionHandler();
	}

}
static void unloadPauseMenu(void* tData) {
	(void)tData;

	shutdownOptionHandler();
}

static void drawPauseMenu(void* tData) {
	(void)tData;
	if (!gData.mIsPaused) return;

	drawOptionHandler();
}

ActorBlueprint MirkPauseMenuBP = {
	.mLoad = loadPauseMenu,
	.mUpdate = updatePauseMenu,
	.mDraw = drawPauseMenu,
	.mUnload = unloadPauseMenu,
};
