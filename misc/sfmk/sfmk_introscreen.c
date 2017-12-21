#include "sfmk_introscreen.h"

#include <stdlib.h>

#include <tari/input.h>
#include <tari/storyboard.h>
#include <tari/drawing.h>
#include <tari/file.h>

#include "sfmk_titlescreen.h"

static struct {
	int mStoryboardID;
	int mIsFinished;
} gData;

#define MAXIMUM_INTRO_AMOUNT 10

static void storyboardFinished(void* tCaller) {
	gData.mIsFinished = 1;
}

static void selectStoryboardAndPlayIt() {

	int intro = rand() % MAXIMUM_INTRO_AMOUNT;
	char path[1024];
	path[0] = '\0';
	while (!isStoryboard(path)) {
		intro--;
		if (intro < 0) intro += MAXIMUM_INTRO_AMOUNT;
		sprintf(path, "assets/misc/sfmk/INTRO/BOARDS/%d.sbd", intro);
	}
	printf("open %s\n", path);
	gData.mStoryboardID = playStoryboard(path);
	setStoryboardFinishedCB(gData.mStoryboardID, storyboardFinished, NULL);
}

static void loadIntroScreen() {
	setupStoryboards();

	selectStoryboardAndPlayIt();

	gData.mIsFinished = 0;
}

static void unloadIntroScreen() {
	shutdownStoryboards();
}

static void updateIntroScreen() {
	updateStoryboards();
}

static Screen* getIntroScreenNextScreen() { 
	if (hasPressedAbortFlank()) {
		return &VersusTitleScreen;
	}
	else if (gData.mIsFinished) {
		return &VersusTitleScreen;
	}

	return NULL; 

}


Screen VersusIntroScreen = {
	.mLoad = loadIntroScreen,
	.mUnload = unloadIntroScreen,
	.mUpdate = updateIntroScreen,
	.mGetNextScreen = getIntroScreenNextScreen
};