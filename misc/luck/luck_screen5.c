#include "luck_screen5.h"

#include <stdio.h>
#include <prism/input.h>
#include <prism/sound.h>

#include "luck_hank.h"
#include "luck_narrator.h"
#include "luck_enemyhandler.h"
#include "luck_collision.h"
#include "luck_gamelogic.h"
#include "luck_bg.h"
#include "luck_horseguy.h"
#include "luck_screen6.h"
#include "luck_smashscreen.h"
#include "luck_titlescreen.h"

static void loadScreen5() {
	setLuckHankName("MILK");
	setLuckNarratorText("Seeing a goose-owning ghost as he chugged, he thought \"Ghosts aren't real\" and shrugged.");
	setLuckCurrentScreen(&LuckScreen5);
	setLuckNextScreen(&LuckSmashScreen);
	setLuckSmashScreenNarrationAndAfterScreen("\"How lucky\", Hank pondered aloud, \"a lone goose!\" and flew away from the pursuing crowd.", &LuckScreen6);

	setupLuckGameCollisions();
	instantiateActor(LuckBackgroundBP);
	instantiateActor(LuckHankBP);
	instantiateActor(LuckNarratorBP);
	instantiateActor(LuckEnemyHandlerBP);

	setLuckHorseGuyName("GOOSE");
	instantiateActor(LuckHorseGuyBP);

	playTrack(29);
}

static Screen* getScreen5NextScreen() {
	
	if (hasPressedAbortFlank()) {
		return &LuckTitleScreen;
	}

	return NULL;
}

Screen LuckScreen5 = {
	.mLoad = loadScreen5,
	.mGetNextScreen = getScreen5NextScreen,

};