#include "medusa_splashscreen.h"

#include <tari/animation.h>
#include <tari/input.h>

#include "medusa_gamescreen.h"
#include "medusa_titlescreen.h"

static struct {
	TextureData mSplashTexture;

	Duration mNow;
	Duration mDuration;
} gData;

static void loadSplashScreen() {
	gData.mSplashTexture = loadTexture("assets/misc/medusa/SPLASH_SCREEN.pkg");
	playOneFrameAnimationLoop(makePosition(0, 0, 1), &gData.mSplashTexture);
	gData.mNow = 0;
	gData.mDuration = 200;
}

static void updateSplashScreen() {
	if (handleDurationAndCheckIfOver(&gData.mNow, gData.mDuration) || hasPressedStartFlank()) {
		setNewScreen(&MedusaGameScreen);
	}

	if (hasPressedAbortFlank()) {
		setNewScreen(&MedusaTitleScreen);
	}
}

Screen MedusaSplashScreen = {
	.mLoad = loadSplashScreen,
	.mUpdate = updateSplashScreen,
};