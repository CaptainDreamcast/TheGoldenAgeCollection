#include "octo_titlescreen.h"

#include <prism/animation.h>
#include <prism/screeneffect.h>
#include <prism/input.h>

#include "../../miscgamemenu.h"
#include "octo_gamescreen.h"

static struct {
	TextureData mBGTexture;

} gData;

static void loadTitleScreen() {
	gData.mBGTexture = loadTexture("assets/misc/octo/TITLE.pkg");
	playOneFrameAnimationLoop(makePosition(0,0,1), &gData.mBGTexture);
	addFadeIn(30, NULL, NULL);
}

static void gotoMiscMenuCB(void* tCaller) {
	(void)tCaller;
	setNewScreen(&MiscGameMenu);
}

static void gotoGameScreenCB(void* tCaller) {
	(void)tCaller;
	setNewScreen(&OctoGameScreen);
}

static void updateTitleScreen() {
	if (hasPressedBFlank()) {
		addFadeOut(30, gotoMiscMenuCB, NULL);
	}

	if (hasPressedAbortFlank()) {
		setNewScreen(&MiscGameMenu);
	}

	if (hasPressedStart()) {
		addFadeOut(30, gotoGameScreenCB, NULL);
	}
}

Screen OctoTitleScreen = {
	.mLoad = loadTitleScreen,
	.mUpdate = updateTitleScreen,
};