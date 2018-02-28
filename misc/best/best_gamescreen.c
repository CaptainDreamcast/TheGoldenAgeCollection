#include "best_gamescreen.h"

#include <prism/input.h>
#include <prism/screeneffect.h>
#include <prism/sound.h>

#include "best_bg.h"
#include "best_player.h"
#include "best_endscreen.h"
#include "../../miscgamemenu.h"

static void loadGameScreen() {
	instantiateActor(BestBackgroundHandler);
	instantiateActor(BestPlayer);

	playTrack(28);
}

static void gotoMiscMenuCB(void* tCaller) {
	(void)tCaller;
	setNewScreen(&MiscGameMenu);
}

static void updateGameScreen() {
	if (hasPressedStartFlank()) {
		setBestEndScreenCrash();
		setNewScreen(&BestEndScreen);
		return;
	}

	if (hasPressedBFlank()) {
		addFadeOut(30, gotoMiscMenuCB, NULL);
	}

	if (hasPressedAbortFlank()) {
		setNewScreen(&MiscGameMenu);
	}
}

Screen BestGameScreen = {
	.mLoad = loadGameScreen,
	.mUpdate = updateGameScreen,
};