#include "octo_titlescreen.h"

#include <tari/animation.h>
#include <tari/screeneffect.h>
#include <tari/input.h>

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

static void gotoGameScreenCB(void* tCaller) {
	(void)tCaller;
	setNewScreen(&OctoGameScreen);
}

static void updateTitleScreen() {
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