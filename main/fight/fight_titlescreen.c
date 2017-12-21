#include "fight_titlescreen.h"

#include <tari/animation.h>
#include <tari/screeneffect.h>
#include <tari/input.h>

#include "../../maingamemenu.h"

#include "fight_menuscreen.h"

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

static void loadTitleScreen() {
	gData.mTexture = loadTexture("assets/main/fight/title/TITLE.pkg");
	gData.mBGID = playOneFrameAnimationLoop(makePosition(0,0,1), &gData.mTexture);

	addFadeIn(30, NULL, NULL);
}

static void updateTitleScreen() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&MainGameMenu);
	}

	if (hasPressedStartFlank()) {
		fadeOut(&DreamMenuScreen);
	}
}

Screen DreamTitleScreen = {
	.mLoad = loadTitleScreen,
	.mUpdate = updateTitleScreen,
};