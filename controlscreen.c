#include "controlscreen.h"

#include <tari/animation.h>
#include <tari/input.h>
#include <tari/screeneffect.h>

#include "titlescreen.h"

static struct {
	void(*mFunc)();
	char mTexturePath[1024];

	TextureData mTexture;
	Screen* mPreviousScreen;
} gData;


static void loadControlScreen() {
	gData.mTexture = loadTexture(gData.mTexturePath);
	playOneFrameAnimationLoop(makePosition(0, 0, 1), &gData.mTexture);

	addFadeIn(30, NULL, NULL);
}

static void gotoPreviousScreenCB(void* tCaller) {
	(void)tCaller;
	setNewScreen(gData.mPreviousScreen);
}

static void gotoGameScreenCB(void* tCaller) {
	gData.mFunc();
}

static void updateControlScreen() {
	if (hasPressedAFlank() || hasPressedStartFlank()) {
		addFadeOut(30, gotoGameScreenCB, NULL);
	}

	if (hasPressedBFlank()) {
		addFadeOut(30, gotoPreviousScreenCB, NULL);
	}

	if (hasPressedAbortFlank()) {
		setNewScreen(&TitleScreen);
	}

}

static Screen ControlScreen = {
	.mLoad = loadControlScreen,
	.mUpdate = updateControlScreen,
};


void startControlScreen(void(*tStartFunc)(), char* tFoldername, char * tGameName, int mIsControllerDependant, Screen* tPreviousScreen)
{
	gData.mPreviousScreen = tPreviousScreen;
	gData.mFunc = tStartFunc;

	char platform[10];
	
#ifdef DREAMCAST
	sprintf(platform, "dc");
#else
	if (mIsControllerDependant && isUsingController()) {
		sprintf(platform, "dc");
	}
	else {
		sprintf(platform, "win");
	}
#endif

	sprintf(gData.mTexturePath, "assets/controls/%s/%s/%s.pkg", platform, tFoldername, tGameName);
	setNewScreen(&ControlScreen);
}
