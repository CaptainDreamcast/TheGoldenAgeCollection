#include "bombx_congratsscreen.h"

#include <tari/screeneffect.h>
#include <tari/animation.h>
#include <tari/input.h>

#include "bombx_titlescreen.h"

static struct {
	TextureData mBGTexture;

} gData;

static void loadCongratsScreen() {
	gData.mBGTexture = loadTexture("assets/bombx/congrats/CONGRATS.pkg");
	playOneFrameAnimationLoop(makePosition(0, 0, 1), &gData.mBGTexture);

	addFadeIn(30, NULL, NULL);
}

static void gotoTileScreen(void* tCaller) {
	(void)tCaller;
	setNewScreen(&BombxTitleScreen);
}

static void updateCongratsScreen() {

	if (hasPressedStartFlank()) {
		addFadeOut(30, gotoTileScreen, NULL);
	}

	if (hasPressedAbortFlank()) {
		setNewScreen(&BombxTitleScreen);
	}
}

Screen BombxCongratsScreen = {
	.mLoad = loadCongratsScreen,
	.mUpdate = updateCongratsScreen,
};