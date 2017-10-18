#include "bombx_gamescreen.h"

#include <tari/input.h>
#include <tari/screeneffect.h>

#include "bombx_titlescreen.h"
#include "bombx_levelhandler.h"

static void loadBombxGameScreen() {
	instantiateActor(BombxLevelHandler);

	addFadeIn(30, NULL, NULL);
}



static void updateBombxGameScreen() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&BombxTitleScreen);
	}
}

Screen BombxGameScreen = {
	.mLoad = loadBombxGameScreen,
	.mUpdate = updateBombxGameScreen,
};