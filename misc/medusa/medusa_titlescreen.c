#include "medusa_titlescreen.h"

#include <tari/animation.h>
#include <tari/input.h>
#include <tari/screeneffect.h>

#include "../../miscgamemenu.h"
#include "medusa_splashscreen.h"
#include "medusa_gamescreen.h"

static struct {
	TextureData mBackgroundTexture;

} gData;

static void loadTitleScreen() {
	gData.mBackgroundTexture = loadTexture("assets/misc/medusa/TITLE.pkg");
	playOneFrameAnimationLoop(makePosition(0, 0, 1), &gData.mBackgroundTexture);
	addFadeIn(30, NULL, NULL);
}

static void gotoSplashScreen(void* tCaller) {
	(void)tCaller;
	setNewScreen(&MedusaSplashScreen);
}

static void updateTitleScreen() {

	if (hasPressedStartFlank()) {
		addFadeOut(30, gotoSplashScreen, NULL);
	}

	if (hasPressedAbortFlank()) {
		setNewScreen(&MiscGameMenu);
	}
}

Screen MedusaTitleScreen = {
	.mLoad = loadTitleScreen,
	.mUpdate = updateTitleScreen
};