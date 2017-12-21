#include "bombx_gamescreen.h"

#include <tari/input.h>
#include <tari/screeneffect.h>

#include "bombx_titlescreen.h"
#include "bombx_levelhandler.h"

#include "bombx_player.h"
#include "bombx_bombxhandler.h"

static void loadBombxGameScreen() {
	instantiateActor(BombxHandler);
	instantiateActor(BombxLevelHandler);
	instantiateActor(BombxPlayer);
	

	addFadeIn(30, NULL, NULL);
}



static void updateBombxGameScreen() {
	if (hasPressedRFlank()) {
		resetBombxLevel();
	}

	if (hasPressedAbortFlank()) {
		setNewScreen(&BombxTitleScreen);
	}
}

Screen BombxGameScreen = {
	.mLoad = loadBombxGameScreen,
	.mUpdate = updateBombxGameScreen,
};