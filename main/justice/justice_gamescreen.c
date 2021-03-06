#include "justice_gamescreen.h"

#include <prism/file.h>
#include <prism/datastructures.h>
#include <prism/wrapper.h>
#include <prism/input.h>
#include <prism/memoryhandler.h>
#include <prism/collisionhandler.h>
#include <prism/mugenanimationhandler.h>
#include <prism/sound.h>

#include <stdio.h>

#include "justice_system.h"
#include "justice_stage.h"
#include "justice_player.h"
#include "justice_enemies.h"
#include "justice_collision.h"
#include "justice_userinterface.h"
#include "justice_gamestate.h"
#include "justice_titlescreen.h"
#include "justice_continuescreen.h"
#include "justice_congratsscreen.h"
#include "justice_sound.h"

static void loadGameScreen() {
	
	char levelpath[300];

	sprintf(levelpath, "/assets/main/justice/%s", getCurrentJusticeLevelName());
	
	//activateCollisionHandlerDebugMode();
	instantiateActor(getMugenAnimationHandlerActorBlueprint());

	resetJusticeLevelState();
	loadJusticeCollision();
	setWorkingDirectory("/assets/main/justice/player");
	loadJusticePlayer();
	setWorkingDirectory(levelpath);
	loadJusticeEnemies();
	setWorkingDirectory("/assets/main/justice/ui");
	loadJusticeUserInterface();
	setWorkingDirectory("/assets/main/justice/");
	loadJusticeSound();
	setWorkingDirectory(levelpath);
	loadJusticeStage();
	
	setWorkingDirectory("/");
	playTrack(26);
}

static void unloadGameScreen() {
	resumeJusticeGame();	
}

static void updateGameScreen() {
	updateJusticeStage();
	updateJusticeEnemies();
	updateJusticePlayer();
	updateJusticeUserInterface();
}

static void drawGameScreen() {
	
}

static Screen* getGameScreenNextScreen() {
	
	if(hasPressedAbortFlank()) {
		return &JusticeTitleScreen;
	}
	
	if(hasClearedJusticeGame()) {
		return &JusticeCongratsScreen;
	}
	
	if(hasClearedJusticeLevel()) {
		return &JusticeGameScreen;
	}
	
	if(hasJusticeLost()) {
		return &ContinueScreen;
	}	

	return NULL;
}


Screen JusticeGameScreen =  {
	.mLoad = loadGameScreen,
	.mUpdate = updateGameScreen,
	.mDraw = drawGameScreen,
	.mUnload = unloadGameScreen,
	.mGetNextScreen = getGameScreenNextScreen
};

