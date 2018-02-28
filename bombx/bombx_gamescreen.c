#include "bombx_gamescreen.h"

#include <prism/input.h>
#include <prism/screeneffect.h>
#include <prism/sound.h>

#include "bombx_titlescreen.h"
#include "bombx_levelhandler.h"

#include "bombx_player.h"
#include "bombx_bombxhandler.h"

static void loadBombxGameScreen() {
	instantiateActor(BombxHandler);
	instantiateActor(BombxLevelHandler);
	instantiateActor(BombxPlayer);
	
	playTrack(23);

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