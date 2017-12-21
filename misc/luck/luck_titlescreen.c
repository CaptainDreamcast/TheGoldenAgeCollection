#include "luck_titlescreen.h"

#include <stdio.h>
#include <tari/input.h>
#include <tari/animation.h>

#include "luck_bg.h"
#include "luck_hank.h"
#include "luck_narrator.h"
#include "luck_enemyhandler.h"
#include "luck_collision.h"
#include "luck_gamelogic.h"
#include "luck_screen1.h"

#include "../../miscgamemenu.h"

static struct {
	TextureData mBGTexture;
	int mBG;

} gData;

static void loadTitleScreen() {
	setLuckCurrentScreen(&LuckTitleScreen);
	setLuckNextScreen(&LuckScreen1);

	setLuckNarratorText("This tale happened a long long time ago in a country far far away. Press Start to let the story play.");
	instantiateActor(LuckNarratorBP);

	gData.mBGTexture = loadTexture("assets/misc/luck/sprites/TITLE.pkg");
	gData.mBG = playOneFrameAnimationLoop(makePosition(0, 0, 1), &gData.mBGTexture);
}

static Screen* getNextTitleScreenScreen() {
	
	if (hasPressedAbortFlank()) {
		return &MiscGameMenu;
	}

	if (hasPressedStartFlank()) {
		return &LuckScreen1;
	}

	return NULL;
}

Screen LuckTitleScreen = {
	.mLoad = loadTitleScreen,
	.mGetNextScreen = getNextTitleScreenScreen,
};