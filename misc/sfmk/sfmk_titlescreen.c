#include "sfmk_titlescreen.h"

#include <stdlib.h>

#include <tari/screeneffect.h>
#include <tari/timer.h>
#include <tari/input.h>
#include <tari/file.h>
#include <tari/physicshandler.h>

#include "../../miscgamemenu.h"

#include "sfmk_introscreen.h"
#include "sfmk_animation.h"
#include "sfmk_modemenu.h"

typedef struct {

	uint16_t mScreenReloadDuration;
	uint16_t mPressStartDuration;

	uint16_t mPressStartPositionX;
	uint16_t mPressStartPositionY;

	uint16_t mPressStartSizeX;
	uint16_t mPressStartSizeY;

	char Separator1[4]; //16

} TitleScreenHeader;

static struct {
	TitleScreenHeader mHeader;
	Screen* mNextScreen;
	Screen* mReturnScreen;

	TextureData mBG;
	int mBGID;

	TextureData mPressStart;
	int mPressStartID;
	VersusAnimationFlickering* mPressStartFlickering;
} gData;


static void fadeOutFinished(void* tCaller) {
	setScreenBlack();
	gData.mReturnScreen = gData.mNextScreen;
}


static void gotoIntro(void* tCaller) {
	gData.mNextScreen = &VersusIntroScreen;
	addFadeOut(30, fadeOutFinished, NULL);
}

static void loadTitleScreen() {
	
	
	mountRomdisk("assets/misc/sfmk/MENU/TITLE.img", "TITLE");
	fileToMemory(&gData.mHeader, sizeof(TitleScreenHeader), "/TITLE/title.hdr");

	addVerticalLineFadeIn(20, NULL, NULL);
	addTimerCB(gData.mHeader.mScreenReloadDuration, gotoIntro, NULL);


	gData.mPressStart = loadTexture("/TITLE/PRESSSTART.pkg");
	gData.mPressStartID = playAnimationLoop(makePosition(gData.mHeader.mPressStartPositionX, gData.mHeader.mPressStartPositionY, 2), &gData.mPressStart, createOneFrameAnimation(), makeRectangleFromTexture(gData.mPressStart));
	setAnimationSize(gData.mPressStartID, makePosition(gData.mHeader.mPressStartSizeX, gData.mHeader.mPressStartSizeY, 1), makePosition(0,0,0));
	gData.mPressStartFlickering = setVersusAnimationFlickering(gData.mPressStartID, 100);

	gData.mBG = loadTexture("/TITLE/BACKGROUND.pkg");
	gData.mBGID = playAnimationLoop(makePosition(0, 0, 1), &gData.mBG, createOneFrameAnimation(), makeRectangleFromTexture(gData.mBG));
	setAnimationSize(gData.mBGID, makePosition(640, 480, 1), makePosition(0, 0, 0));

	gData.mNextScreen = NULL;
	gData.mReturnScreen = NULL;

	unmountRomdisk("TITLE");
}

static void setPressStartBlinkingFaster() {
	setVersusAnimationFlickeringDuration(gData.mPressStartFlickering, 5);
}

static void setModeMenuFadeout(void* tCaller) {
	addFadeOut(20, fadeOutFinished, NULL);
}

static void gotoModeMenu() {
	gData.mNextScreen = &VersusModeMenu;
	setPressStartBlinkingFaster();
	addTimerCB(gData.mHeader.mPressStartDuration, setModeMenuFadeout, NULL);
}

static Screen* getNextTitleScreenScreen() {
	if (hasPressedAbortFlank()) {
		return &MiscGameMenu;
	}
	
	if (gData.mNextScreen == NULL && hasPressedStartFlank()) {
		gotoModeMenu();
	}

	return gData.mReturnScreen;
}

Screen VersusTitleScreen = {
	.mLoad = loadTitleScreen,
	.mGetNextScreen = getNextTitleScreenScreen
};


