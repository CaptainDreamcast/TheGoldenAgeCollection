#include "luck_screen4.h"

#include <stdio.h>
#include <tari/input.h>
#include <tari/sound.h>

#include "luck_hank.h"
#include "luck_narrator.h"
#include "luck_enemyhandler.h"
#include "luck_collision.h"
#include "luck_gamelogic.h"
#include "luck_bg.h"
#include "luck_horseguy.h"
#include "luck_screen5.h"
#include "luck_smashscreen.h"
#include "luck_titlescreen.h"

static void loadScreen4() {
	setLuckHankName("HORSE");
	setLuckNarratorText("\"If only\", Hank lost himself in thought, \"I could get a drink after this drought.\"");
	setLuckCurrentScreen(&LuckScreen4);
	setLuckNextScreen(&LuckSmashScreen);
	setLuckSmashScreenNarrationAndAfterScreen("Lucky as Hank ever was, he traded his horse for a milk-filled glass.", &LuckScreen5);

	setupLuckGameCollisions();
	instantiateActor(LuckBackgroundBP);
	instantiateActor(LuckHankBP);
	instantiateActor(LuckNarratorBP);
	instantiateActor(LuckEnemyHandlerBP);

	setLuckHorseGuyName("MILK");
	instantiateActor(LuckHorseGuyBP);

	playTrack(29);
}

static Screen* getScreen4NextScreen() {
	
	if (hasPressedAbortFlank()) {
		return &LuckTitleScreen;
	}

	return NULL;
}

Screen LuckScreen4 = {
	.mLoad = loadScreen4,
	.mGetNextScreen = getScreen4NextScreen,

};