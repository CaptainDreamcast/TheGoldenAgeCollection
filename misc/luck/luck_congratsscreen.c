#include "luck_congratsscreen.h"

#include <stdio.h>

#include <prism/input.h>
#include <prism/animation.h>
#include <prism/timer.h>

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
	int mNarrationAmount;
	int mCurrent;
	int mTimer;
} gData;

static char gNarrations[6][1024] = {
	"At last, you've reached the journey's end, for this I want to thank you, friend.",
	"Hank's trading mission now fulfilled, with only 6 bystanders killed.",
	"They weren't killed but just knocked out, so please don't call dear Hank a fraud.",
	"The story's moral as it stands, is never venture into foreign lands,",
	"without a pirate stone and duck, or else rely on your dumb luck.",
	"Yeah that rhyme was horrible, okay?. Thanks for playing anyway.",
};

static void setNextNarration(void* tCaller) {
	(void)tCaller;
	if (gData.mCurrent == gData.mNarrationAmount) {
		setNewScreen(&LuckTitleScreen);
	}

	setLuckNarratorText(gNarrations[gData.mCurrent++]);
	gData.mTimer = addTimerCB(800, setNextNarration, NULL);
}


static void loadCongratsScreen() {
	setLuckCurrentScreen(&LuckCongratsScreen);
	setLuckNextScreen(&LuckTitleScreen);

	gData.mNarrationAmount = 6;
	gData.mCurrent = 0;

	setNextNarration(NULL);
	instantiateActor(LuckNarratorBP);

	gData.mBGTexture = loadTexture("assets/misc/luck/sprites/CONGRATS.pkg");
	gData.mBG = playOneFrameAnimationLoop(makePosition(0, 0, 1), &gData.mBGTexture);
}

static Screen* getNextCongratsScreenScreen() {
	
	if (hasPressedAbortFlank()) {
		return &LuckTitleScreen;
	}

	if (hasPressedStartFlank()) {
		if (gData.mTimer != -1) {
			removeTimer(gData.mTimer);
			gData.mTimer = -1;
		}
		if (gData.mCurrent >= gData.mNarrationAmount) {
			return &LuckTitleScreen;
		}
		setNextNarration(NULL);
	}
	else if (hasPressedAFlank()) {
		if (gData.mTimer != -1) {
			removeTimer(gData.mTimer);
			gData.mTimer = -1;
		}
		
		if (gData.mCurrent >= gData.mNarrationAmount) {
			return &LuckTitleScreen;
		}
		setNextNarration(NULL);

	}

	

	return NULL;
}

Screen LuckCongratsScreen = {
	.mLoad = loadCongratsScreen,
	.mGetNextScreen = getNextCongratsScreenScreen,
};