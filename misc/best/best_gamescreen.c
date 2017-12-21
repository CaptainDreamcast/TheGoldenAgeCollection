#include "best_gamescreen.h"

#include <tari/input.h>

#include "best_bg.h"
#include "best_player.h"
#include "../../miscgamemenu.h"

static void loadGameScreen() {
	instantiateActor(BestBackgroundHandler);
	instantiateActor(BestPlayer);
}

static void updateGameScreen() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&MiscGameMenu);
	}
}

Screen BestGameScreen = {
	.mLoad = loadGameScreen,
	.mUpdate = updateGameScreen,
};