#include "cane_logoscreen.h"

#include <stdlib.h>

#include <prism/input.h>
#include <prism/timer.h>
#include <prism/animation.h>

#include "cane_titlescreen.h"

static struct {
	int mIsOver;

	int animationID;
	TextureData logo;
} gData;

static void setLogoScreenOver(void* caller) {
	gData.mIsOver = 1;
}

static void loadLogoScreen() {

	gData.logo = loadTexture("assets/main/cane/sprites/LOGO.pkg");
	gData.animationID = playAnimationLoop(makePosition(0,0,1), &gData.logo, createOneFrameAnimation(), makeRectangleFromTexture(gData.logo));
	fadeInAnimation(gData.animationID, 60);

	addTimerCB(300, setLogoScreenOver, NULL);
	gData.mIsOver = 0;
}


static void unloadLogoScreen() {}
static void updateLogoScreen() {}
static void drawLogoScreen() {}
static Screen* getNextLogoScreenScreen() {
	if (hasPressedAbortFlank()) {
		abortScreenHandling();
	}
	
	if (gData.mIsOver || hasPressedStartFlank()) {
		return &CaneTitleScreen;
	}

	return NULL;
}

Screen CaneLogoScreen = {
	.mLoad = loadLogoScreen,
	.mUnload = unloadLogoScreen,
	.mUpdate = updateLogoScreen,
	.mDraw = drawLogoScreen,
	.mGetNextScreen = getNextLogoScreenScreen
};																											 
