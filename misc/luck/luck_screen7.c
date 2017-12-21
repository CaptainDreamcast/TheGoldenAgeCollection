#include "luck_screen7.h"

#include <stdio.h>
#include <tari/input.h>

#include "luck_hank.h"
#include "luck_narrator.h"
#include "luck_enemyhandler.h"
#include "luck_collision.h"
#include "luck_gamelogic.h"
#include "luck_bg.h"
#include "luck_horseguy.h"
#include "luck_screen8.h"
#include "luck_smashscreen.h"
#include "luck_titlescreen.h"

static void loadScreen7() {
	setLuckHankName("GOOSE");
	setLuckNarratorText("As the journey's end was closing near, Hank's trading skills now knew no peer.");
	setLuckCurrentScreen(&LuckScreen7);
	setLuckNextScreen(&LuckSmashScreen);
	setLuckSmashScreenNarrationAndAfterScreen("It was the usual routine. A stone for a goose, Hank's conscience was clean.", &LuckScreen8);

	setupLuckGameCollisions();
	instantiateActor(LuckBackgroundBP);
	instantiateActor(LuckHankBP);
	instantiateActor(LuckNarratorBP);
	instantiateActor(LuckEnemyHandlerBP);

	setLuckHorseGuyName("STONE");
	instantiateActor(LuckHorseGuyBP);
}

static Screen* getScreen7NextScreen() {
	
	if (hasPressedAbortFlank()) {
		return &LuckTitleScreen;
	}

	return NULL;
}

Screen LuckScreen7 = {
	.mLoad = loadScreen7,
	.mGetNextScreen = getScreen7NextScreen,

};