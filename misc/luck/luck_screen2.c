#include "luck_screen2.h"

#include <stdio.h>
#include <prism/input.h>
#include <prism/sound.h>

#include "luck_bg.h"
#include "luck_hank.h"
#include "luck_narrator.h"
#include "luck_enemyhandler.h"
#include "luck_collision.h"
#include "luck_gamelogic.h"
#include "luck_horseguy.h"
#include "luck_screen3.h"
#include "luck_smashscreen.h"
#include "luck_titlescreen.h"

static void loadScreen2() {
	setLuckHankName("GOLD");
	setLuckNarratorText("Hank discovered a man with horse in tow, that the horse would be Hank's he did not know.");
	setLuckCurrentScreen(&LuckScreen2);
	setLuckNextScreen(&LuckSmashScreen);
	setLuckSmashScreenNarrationAndAfterScreen("With a clunk Hank got his horse, inside his heart was no remorse.", &LuckScreen3);

	setupLuckGameCollisions();
	instantiateActor(LuckBackgroundBP);
	instantiateActor(LuckHankBP);
	instantiateActor(LuckNarratorBP);
	instantiateActor(LuckEnemyHandlerBP);

	setLuckHorseGuyName("HORSE");
	instantiateActor(LuckHorseGuyBP);

	playTrack(29);

}

static Screen* getScreen2NextScreen() {
	
	if (hasPressedAbortFlank()) {
		return &LuckTitleScreen;
	}

	return NULL;
}

Screen LuckScreen2 = {
	.mLoad = loadScreen2,
	.mGetNextScreen = getScreen2NextScreen,

};