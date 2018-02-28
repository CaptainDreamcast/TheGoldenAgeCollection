#include "fight_titlescreen.h"

#include <prism/animation.h>
#include <prism/screeneffect.h>
#include <prism/input.h>

#include "fight_menuscreen.h"
#include "fight_main.h"

static struct {
	TextureData mTexture;
	int mBGID;
} gData;

static void fadeOutFinished(void* tData) {
	Screen* screen = tData;
	setNewScreen(screen);
}

static void fadeOut(Screen* tScreen) {
	addFadeOut(30, fadeOutFinished, tScreen);
}

static void gotoMainMenuCB(void* tCaller) {
	(void)tCaller;
	stopDreamFight16();
}

static void loadTitleScreen() {
	gData.mTexture = loadTexture("assets/main/fight/title/TITLE.pkg");
	gData.mBGID = playOneFrameAnimationLoop(makePosition(0,0,1), &gData.mTexture);

	addFadeIn(30, NULL, NULL);
}

static void updateTitleScreen() {
	if (hasPressedBFlank()) {
		addFadeOut(30, gotoMainMenuCB, NULL);
	}

	if (hasPressedAbortFlank()) {
		stopDreamFight16();
	}

	if (hasPressedStartFlank()) {
		fadeOut(&DreamMenuScreen);
	}
}

Screen DreamTitleScreen = {
	.mLoad = loadTitleScreen,
	.mUpdate = updateTitleScreen,
};
