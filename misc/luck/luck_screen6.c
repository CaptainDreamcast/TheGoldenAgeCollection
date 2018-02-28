#include "luck_screen6.h"

#include <stdio.h>
#include <prism/input.h>
#include <prism/timer.h>
#include <prism/sound.h>

#include "luck_hank.h"
#include "luck_narrator.h"
#include "luck_enemyhandler.h"
#include "luck_collision.h"
#include "luck_gamelogic.h"
#include "luck_scrolling.h"
#include "luck_boxgenerator.h"
#include "luck_screen7.h"
#include "luck_titlescreen.h"

static void levelOver(void* tCaller) {
	(void)tCaller;
	moveLuckToNextScreen();
}

static void loadScreen6() {
	setLuckHankName("GOOSE");
	setLuckNarratorText("Hank flies away through the nuclear sky. Yet things are about to go awry.");
	setLuckCurrentScreen(&LuckScreen6);
	setLuckNextScreen(&LuckScreen7);

	setLuckScrollingBGName("SKY");
	setupLuckGameCollisions();
	instantiateActor(LuckScrollingBackgroundBP);
	instantiateActor(LuckHankBP);
	instantiateActor(LuckNarratorBP);
	instantiateActor(LuckBoxGeneratorBP);
	setLuckHankEvasionLevel();

	addTimerCB(30 * 60, levelOver, NULL);

	playTrack(29);
}

static Screen* getScreen6NextScreen() {
	
	if (hasPressedAbortFlank()) {
		return &LuckTitleScreen;
	}

	return NULL;
}

Screen LuckScreen6 = {
	.mLoad = loadScreen6,
	.mGetNextScreen = getScreen6NextScreen,

};