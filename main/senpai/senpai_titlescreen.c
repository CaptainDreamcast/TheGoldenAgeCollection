#include "senpai_titlescreen.h"

#include "../../maingamemenu.h"

#include <prism/animation.h>
#include <prism/screeneffect.h>
#include <prism/input.h>

#include "senpai_gamescreen.h"

static struct {
	TextureData mBGTexture;

} gData;

static void loadTitleScreen() {
	gData.mBGTexture = loadTexture("assets/main/senpai/title/TITLE.pkg");
	playOneFrameAnimationLoop(makePosition(0, 0, 1), &gData.mBGTexture);

	addFadeIn(30, NULL, NULL);
}

static void gotoGameScreen(void* tCaller) {
	(void)tCaller;
	setNewScreen(&SenpaiGameScreen);
}

static void gotoMainMenuCB(void* tCaller) {
	(void)tCaller;
	setNewScreen(&MainGameMenu);
}


static void updateTitleScreen() {
	if (hasPressedBFlank()) {
		addFadeOut(30, gotoMainMenuCB, NULL);
	}

	if (hasPressedAbortFlank()) {
		setNewScreen(&MainGameMenu);
		return;
	}

	if (hasPressedStartFlank()) {
		addFadeOut(30, gotoGameScreen, NULL);
	}
}

Screen SenpaiTitleScreen = {
	.mLoad = loadTitleScreen,
	.mUpdate = updateTitleScreen
};