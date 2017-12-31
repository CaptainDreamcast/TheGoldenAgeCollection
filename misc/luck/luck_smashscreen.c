#include "luck_smashscreen.h"

#include <stdio.h>

#include <tari/input.h>
#include <tari/animation.h>
#include <tari/sound.h>

#include "luck_bg.h"
#include "luck_hank.h"
#include "luck_narrator.h"
#include "luck_enemyhandler.h"
#include "luck_collision.h"
#include "luck_gamelogic.h"
#include "luck_screen1.h"
#include "luck_titlescreen.h"

static struct {
	TextureData mBGTexture;
	int mBG;

	Screen* mNextScreen;
	char mNarratorText[1024];
} gData;

static void loadSmashScreen() {
	setLuckCurrentScreen(&LuckSmashScreen);
	setLuckNextScreen(gData.mNextScreen);

	setLuckNarratorText(gData.mNarratorText);
	instantiateActor(LuckNarratorBP);

	gData.mBGTexture = loadTexture("assets/misc/luck/sprites/SMASH.pkg");
	gData.mBG = playOneFrameAnimationLoop(makePosition(0, 0, 1), &gData.mBGTexture);

	playTrack(29);
}

static Screen* getNextSmashScreenScreen() {
	
	if (hasPressedAbortFlank()) {
		return &LuckTitleScreen;
	}

	if (hasPressedStartFlank()) {
		return gData.mNextScreen;
	}

	return NULL;
}

Screen LuckSmashScreen = {
	.mLoad = loadSmashScreen,
	.mGetNextScreen = getNextSmashScreenScreen,
};

void setLuckSmashScreenNarrationAndAfterScreen(char * tText, Screen * tScreenAfter)
{
	strcpy(gData.mNarratorText, tText);
	gData.mNextScreen = tScreenAfter;
}
