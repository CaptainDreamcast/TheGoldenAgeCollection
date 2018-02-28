#include "norm_gamescreen.h"

#include <stdio.h>

#include <prism/input.h>
#include <prism/sound.h>

#include "norm_player.h"
#include "norm_enemygenerator.h"
#include "norm_ui.h"
#include "norm_debris.h"
#include "norm_bg.h"
#include "norm_title.h"
#include "norm_readytext.h"
#include "norm_timer.h"

static void loadGameScreen() {
	loadNormDebris();
	instantiateActor(NormBackgroundBP);
	instantiateActor(NormPlayer);
	instantiateActor(NormUserInterface);
	instantiateActor(NormEnemyGenerator);
	instantiateActor(NormGoBP);
	instantiateActor(NormTimer);
	playTrack(3);
}

static Screen* getGameScreenNextScreen() {
	if (hasPressedAbortFlank()) {
		return &NormTitleScreen;
	}
	return NULL;
}


Screen NormGameScreen = {
	.mLoad = loadGameScreen,
	.mGetNextScreen = getGameScreenNextScreen,
};