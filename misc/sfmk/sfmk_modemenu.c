#include "sfmk_modemenu.h"

#include <stdlib.h>
#include <stdint.h>

#include <tari/input.h>
#include <tari/file.h>
#include <tari/texture.h>
#include <tari/animation.h>
#include <tari/drawing.h>
#include <tari/screeneffect.h>
#include <tari/timer.h>

#include "sfmk_titlescreen.h"
#include "sfmk_animation.h"
#include "sfmk_fighterselectscreen.h"

typedef struct {

	uint16_t mPositionX;
	uint16_t mPositionY;

} SingleModeMenuOptionHeader;

typedef struct {

	uint16_t mAfterInputDuration;
	uint8_t mMagic;
	uint8_t mVersion;

	uint8_t mAlternateColor;
	uint8_t mSeparator1[3];

	float mAlternateColorMinValue;
	float mAlternateColorMaxValue;

	SingleModeMenuOptionHeader mSingleMode[4];

} ModeMenuHeader;

typedef struct {
	int mBackgroundAnimationID;
	int mForegroundAnimationID;

	TextureData mForeground;
} Option;

static struct {
	ModeMenuHeader mHeader;

	TextureData mBG;

	TextureData mModeBackground;

	VersusAnimationFlickering* mModeSelectedFlickering;
	TextureData mModeSelectedBackground;
	int mHasAlternateBackground;

	int mModeSelectedAnimationID;

	Option mOptions[4];

	int mSelectedOption;
	int mLastSelectedOption;

	int mHasSelectedSomething;
	Screen* mNextScreen;
	Screen* mReturnScreen;
} gData;

const char * const cModeNames[] = { "STORY", "STORY", "STORY", "STORY" };

static void unsetSelectedOption() {
	if (gData.mHasAlternateBackground) {
		stopVersusAnimationFlickering(gData.mModeSelectedFlickering);
		removeHandledAnimation(gData.mModeSelectedAnimationID);
	}
	else {
		setAnimationColor(gData.mOptions[gData.mSelectedOption].mBackgroundAnimationID, 1, 1, 1);
	}

	gData.mSelectedOption = -1;
}

static void setSelectedOption(int tOption) {
	if (gData.mSelectedOption != -1) {
		unsetSelectedOption();
	}

	gData.mSelectedOption = tOption;
	if (gData.mHasAlternateBackground) {
		double x = gData.mHeader.mSingleMode[gData.mSelectedOption].mPositionX;
		double y = gData.mHeader.mSingleMode[gData.mSelectedOption].mPositionY;
		gData.mModeSelectedAnimationID = playAnimationLoop(makePosition(x, y, 3), &gData.mModeSelectedBackground, createOneFrameAnimation(), makeRectangleFromTexture(gData.mModeSelectedBackground));
		gData.mModeSelectedFlickering = setVersusAnimationFlickering(gData.mModeSelectedAnimationID, 100);
	}
	else {
		double r, g, b;
		getRGBFromColor(gData.mHeader.mAlternateColor, &r, &g, &b);
		setAnimationColor(gData.mOptions[gData.mSelectedOption].mBackgroundAnimationID, r, g, b);
	}
}

static void loadModeMenu() {
	mountRomdisk("assets/misc/sfmk/MENU/MAINMENU.img", "MAINMENU");
	fileToMemory(&gData.mHeader, sizeof(ModeMenuHeader), "/MAINMENU/MAINMENU.hdr");

	gData.mBG = loadTexture("/MAINMENU/BACKGROUND.pkg");
	int id = playAnimationLoop(makePosition(0,0,1), &gData.mBG, createOneFrameAnimation(), makeRectangleFromTexture(gData.mBG));
	setAnimationSize(id, makePosition(640, 480, 1), makePosition(0, 0, 0));

	gData.mModeBackground = loadTexture("/MAINMENU/MODEBACKGROUND.pkg");
	
	if (canLoadTexture("/MAINMENU/MODEALTERNATE.pkg")) {
		gData.mModeSelectedBackground = loadTexture("/MAINMENU/MODEALTERNATE.pkg");
		gData.mHasAlternateBackground = 1;
	}
	else {
		gData.mHasAlternateBackground = 0;
	}

	int i;
	for (i = 0; i < 4; i++) {
		double x = gData.mHeader.mSingleMode[i].mPositionX;
		double y = gData.mHeader.mSingleMode[i].mPositionY;

		char path[1024];
		sprintf(path, "/MAINMENU/%s.pkg", cModeNames[i]);
		gData.mOptions[i].mForeground = loadTexture(path);
		gData.mOptions[i].mForegroundAnimationID = playAnimationLoop(makePosition(x, y, 5), &gData.mOptions[i].mForeground, createOneFrameAnimation(), makeRectangleFromTexture(gData.mOptions[i].mForeground));
		gData.mOptions[i].mBackgroundAnimationID = playAnimationLoop(makePosition(x, y, 2), &gData.mModeBackground, createOneFrameAnimation(), makeRectangleFromTexture(gData.mModeBackground));
	}

	unmountRomdisk("MAINMENU");

	gData.mNextScreen = NULL;
	gData.mReturnScreen = NULL;
	gData.mHasSelectedSomething = 0;

	gData.mSelectedOption = -1;
	setSelectedOption(gData.mLastSelectedOption);
	addVerticalLineFadeIn(20, NULL, NULL);
}

static void fadeOutFinished(void* tCaller) {
	setScreenBlack();
	gData.mReturnScreen = gData.mNextScreen;
}

static void setFadeOut() {
	addFadeOut(20, fadeOutFinished, NULL);
}

static void optionBlinkingFinished(void* tCaller) {
	setFadeOut();
}

static void setSelectedOptionUpOrDown() {
	int option = (gData.mSelectedOption + 2) % 4;
	setSelectedOption(option);
}

static void setSelectedOptionRight() {
	int option;
	if (gData.mSelectedOption == 0 || gData.mSelectedOption == 2) option = gData.mSelectedOption + 1;
	else option = gData.mSelectedOption - 1;
	setSelectedOption(option);
}

static void setSelectedOptionLeft() {
	int option;
	if (gData.mSelectedOption == 1 || gData.mSelectedOption == 3) option = gData.mSelectedOption - 1;
	else option = gData.mSelectedOption + 1;
	setSelectedOption(option);
}

static void selectOption() {
	gData.mHasSelectedSomething = 1;

	if (gData.mSelectedOption == 0) gData.mNextScreen = &VersusFighterSelectScreen;
	else if (gData.mSelectedOption == 1) gData.mNextScreen = &VersusFighterSelectScreen;
	else if (gData.mSelectedOption == 2) gData.mNextScreen = &VersusFighterSelectScreen;
	else gData.mNextScreen = &VersusFighterSelectScreen;

	gData.mLastSelectedOption = gData.mSelectedOption;

	if (gData.mHasAlternateBackground) {
		setVersusAnimationFlickeringDuration(gData.mModeSelectedFlickering, 5);
		addTimerCB(100, optionBlinkingFinished, NULL);
	}
	else {
		setFadeOut();
	}
}

static void returnToTitleScreen() {
	gData.mHasSelectedSomething = 1;
	gData.mNextScreen = &VersusTitleScreen;
	setFadeOut();
}

static void updateModeMenu() {
	if (gData.mHasSelectedSomething) return;

	if (hasPressedUpFlank() || hasPressedDownFlank()) {
		setSelectedOptionUpOrDown();
	}
	if (hasPressedLeftFlank()) {
		setSelectedOptionLeft();
	}
	if (hasPressedRightFlank()) {
		setSelectedOptionRight();
	}

	if (hasPressedAFlank() || hasPressedStartFlank()) {
		selectOption();
	}
	else if (hasPressedBFlank()) {
		returnToTitleScreen();
	}
}

static Screen* getNextModeMenuScreen() {
	if (hasPressedAbortFlank()) {
		return &VersusTitleScreen;
	}

	return gData.mReturnScreen;
}


Screen VersusModeMenu = {
	.mLoad = loadModeMenu,
	.mUpdate = updateModeMenu,
	.mGetNextScreen = getNextModeMenuScreen
};