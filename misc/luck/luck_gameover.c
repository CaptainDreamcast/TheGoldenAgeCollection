#include "luck_gameover.h"

#include <tari/animation.h>
#include <tari/optionhandler.h>
#include <tari/input.h>

#include "luck_gamelogic.h"
#include "luck_titlescreen.h"

static struct {
	TextureData mTexture;
	int mAnimation;
} gData;

static void selectContinue(void* tCaller) {
	(void)tCaller;
	moveLuckToCurrentScreen();
}

static void selectTitle(void* tCaller) {
	(void)tCaller;
	setNewScreen(&LuckTitleScreen);
}

static void loadGameOverScreen() {
	instantiateActor(getOptionHandlerBlueprint());
	setOptionButtonA();
	setOptionButtonStart();
	setOptionTextSize(20);
	setOptionTextBreakSize(-5);

	addOption(makePosition(150, 300, 2), "Continue", selectContinue, NULL);
	addOption(makePosition(150, 330, 2), "Return to Title", selectTitle, NULL);

	gData.mTexture = loadTexture("assets/misc/luck/sprites/GAMEOVER.pkg");
	playOneFrameAnimationLoop(makePosition(0, 0, 1), &gData.mTexture);
}

static Screen* getNextGameOverScreenScreen() {
	
	if (hasPressedAbortFlank()) {
		return &LuckTitleScreen;
	}

	return NULL;
}


Screen LuckGameOverScreen = {
	.mLoad = loadGameOverScreen,
	.mGetNextScreen = getNextGameOverScreenScreen,
};