#include "justice_gameoverscreen.h"

#include <tari/file.h>
#include <tari/texture.h>
#include <tari/animation.h>
#include <tari/input.h>
#include <tari/timer.h>

#include "justice_gamestate.h"
#include "justice_titlescreen.h"

static struct {

	TextureData background;
	int isCounterExpired;
	

} gData;

static void countdownFinished(void* caller);


static void loadGameOverScreen() {
	gData.isCounterExpired = 0;


	setWorkingDirectory("/assets/main/justice/gameover/");
	gData.background = loadTexture("BG.pkg");
	playAnimationLoop(makePosition(0, 0, 2), &gData.background, createOneFrameAnimation(), makeRectangleFromTexture(gData.background));

	addTimerCB(600, countdownFinished, NULL);
}



static void unloadGameOverScreen() {
		
}

static void countdownFinished(void* caller) {
	(void) caller;
	
	gData.isCounterExpired = 1;

}

static void updateGameOverScreen() {

}

static void drawGameOverScreen() {
	
}

static Screen* getGameOverScreenNextScreen() {
	
	if(hasPressedAbortFlank()) {
		return &JusticeTitleScreen;		
	}
	else if(hasPressedStartFlank()) {
		return &JusticeTitleScreen;	
	}

	if(gData.isCounterExpired) {
		return &JusticeTitleScreen;
	}

	return NULL;
}


Screen JusticeGameOverScreen =  {
	.mLoad = loadGameOverScreen,
	.mUpdate = updateGameOverScreen,
	.mDraw = drawGameOverScreen,
	.mUnload = unloadGameOverScreen,
	.mGetNextScreen = getGameOverScreenNextScreen
};

