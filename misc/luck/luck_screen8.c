#include "luck_screen8.h"

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
#include "luck_titlescreen.h"
#include "luck_finalboss.h"
#include "luck_congratsscreen.h"


static void loadScreen8() {
	setLuckHankName("STONE");
	setLuckNarratorText("Stone in hand and fiends in reach, finally Hank is unleashed.");
	setLuckCurrentScreen(&LuckScreen8);
	setLuckNextScreen(&LuckCongratsScreen);

	setupLuckGameCollisions();
	instantiateActor(LuckBackgroundBP);
	instantiateActor(LuckHankBP);
	instantiateActor(LuckNarratorBP);

	loadLuckFinalBosses();
	addLuckFinalBoss(makePosition(400, 130, 2));
	addLuckFinalBoss(makePosition(350, 200, 2));
	addLuckFinalBoss(makePosition(480, 150, 2));

	playTrack(29);
}

static void updateScreen8() {
	if (!getLuckFinalBossAmount()) {
		moveLuckToNextScreen();
	}
}

static Screen* getScreen8NextScreen() {
	
	if (hasPressedAbortFlank()) {
		return &LuckTitleScreen;
	}

	return NULL;
}

Screen LuckScreen8 = {
	.mLoad = loadScreen8,
	.mUpdate = updateScreen8,
	.mGetNextScreen = getScreen8NextScreen,

};