#include "luck_screen1.h"

#include <stdio.h>
#include <tari/input.h>

#include "luck_bg.h"
#include "luck_hank.h"
#include "luck_narrator.h"
#include "luck_enemyhandler.h"
#include "luck_collision.h"
#include "luck_gamelogic.h"
#include "luck_screen2.h"
#include "luck_titlescreen.h"

static void loadScreen1() {
	setLuckHankName("GOLD");
	setLuckNarratorText("Hank had found a head-sized chunk of gold, yet gold-resistant pirates were abound. He went right in flight.");
	setLuckCurrentScreen(&LuckScreen1);
	setLuckNextScreen(&LuckScreen2);

	setupLuckGameCollisions();
	instantiateActor(LuckBackgroundBP);
	instantiateActor(LuckHankBP);
	instantiateActor(LuckNarratorBP);
	instantiateActor(LuckEnemyHandlerBP);
	setLuckHankSupposedToGoRight();
	setLuckHankPosition(makePosition(320, 150, 2));
}

static Screen* getScreen1NextScreen() {
	
	if (hasPressedAbortFlank()) {
		return &LuckTitleScreen;
	}

	return NULL;
}

Screen LuckScreen1 = {
	.mLoad = loadScreen1,
	.mGetNextScreen = getScreen1NextScreen,

};