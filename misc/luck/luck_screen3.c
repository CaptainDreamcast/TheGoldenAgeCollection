#include "luck_screen3.h"

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
#include "luck_screen4.h"
#include "luck_titlescreen.h"


static void levelOver(void* tCaller) {
	(void)tCaller;
	moveLuckToNextScreen();
}

static void loadScreen3() {
	setLuckHankName("HORSE");
	setLuckNarratorText("Hank and horse, united in fear, galloped through the nuclear wasteland: \"Let's get away from here!\"");
	setLuckCurrentScreen(&LuckScreen3);
	setLuckNextScreen(&LuckScreen4);

	setLuckScrollingBGName("SCROLLING");
	setupLuckGameCollisions();
	instantiateActor(LuckScrollingBackgroundBP);
	instantiateActor(LuckHankBP);
	instantiateActor(LuckNarratorBP);
	instantiateActor(LuckBoxGeneratorBP);
	setLuckHankEvasionLevel();

	addTimerCB(30*60, levelOver, NULL);

	playTrack(29);
}

static Screen* getScreen3NextScreen() {
	
	if (hasPressedAbortFlank()) {
		return &LuckTitleScreen;
	}

	return NULL;
}

Screen LuckScreen3 = {
	.mLoad = loadScreen3,
	.mGetNextScreen = getScreen3NextScreen,

};