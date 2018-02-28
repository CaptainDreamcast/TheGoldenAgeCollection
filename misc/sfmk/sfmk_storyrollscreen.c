#include "sfmk_storyrollscreen.h"

#include <stdio.h>
#include <string.h>

#include <prism/input.h>
#include <prism/storyboard.h>
#include <prism/actorhandler.h>

#include "sfmk_fighterselectscreen.h"
#include "sfmk_titlescreen.h"

static struct {
	char mName[100];
	int mCurrentStoryboard;

} gData;

static void playNextStoryboard();

static void loadVersusStoryRollScreen() {
	instantiateActor(getStoryboardHandlerActorBlueprint());

	playNextStoryboard();
}

static void storyboardFinishedCB(void* tCaller) {
	(void)tCaller;
	gData.mCurrentStoryboard++;
	if (gData.mCurrentStoryboard == 10) setNewScreen(&VersusFighterSelectScreen);
	else  setNewScreen(&VersusStoryRollScreen);
}

static void playNextStoryboard() {
	char path[1024];
	sprintf(path, "assets/misc/sfmk/%s/BOARDS/%d.sbd", gData.mName, gData.mCurrentStoryboard);
	int id = playStoryboard(path);
	setStoryboardFinishedCB(id, storyboardFinishedCB, NULL);
}

static void updateVersusStoryRollScreen() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&VersusTitleScreen);
	}
}

Screen VersusStoryRollScreen = {
	.mLoad = loadVersusStoryRollScreen,
	.mUpdate = updateVersusStoryRollScreen,
};

void setVersusStoryCharacter(char * tName)
{
	strcpy(gData.mName, tName);
	gData.mCurrentStoryboard = 1;
}
