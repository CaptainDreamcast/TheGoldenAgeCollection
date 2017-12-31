#include "cane_gamescreen.h"

#include <stdlib.h>

#include <tari/input.h>
#include <tari/wrapper.h>
#include <tari/animationtree.h>
#include <tari/stagehandler.h>
#include <tari/collisionhandler.h>
#include <tari/mugenanimationhandler.h>
#include <tari/sound.h>

#include "cane_player.h"
#include "cane_bullet.h"
#include "cane_stage.h"
#include "cane_enemies.h"
#include "cane_ui.h"
#include "cane_collision.h"
#include "cane_titlescreen.h"
#include "cane_gamelogic.h"
#include "cane_retryscreen.h"
#include "cane_shadow.h"
#include "cane_kidnapper.h"
#include "cane_manualenemy.h"
#include "cane_resources.h"

static void loadGameScreen() {

	// activateCollisionHandlerDebugMode();
	loadCaneResources(); // TODO: remove

	instantiateActor(getMugenAnimationHandlerActorBlueprint());

	loadCaneGameLogic();
	loadCaneCollisions();
	loadCaneStage();
	setupCaneBulletHandling();
	loadCanePlayer();
	loadCaneKidnapper();
	loadCaneEnemies();
	loadCaneManualEnemy();
	loadGameUI();
	
	playTrack(27);
}

static void unloadGameScreen() {
	shutdownCaneBulletHandling();
}

static void updateGameScreen() {
	updateCaneBulletHandling();
	updateCanePlayer();
	updateCaneKidnapper();
	updateCaneEnemies();
	updateCaneManualEnemy();
	updateCaneStage();
	updateCaneUI();
}

static void drawGameScreen() {
	
}

static Screen* getNextGameScreenScreen() {

	if (hasPressedAbortFlank()) {
		return &CaneTitleScreen;
	}
	else if (isCaneLevelLost()) {
		return &CaneRetryScreen;
	}

	return NULL;
}



Screen CaneGameScreen = {
	.mLoad = loadGameScreen,
	.mUnload = unloadGameScreen,
	.mUpdate = updateGameScreen,
	.mDraw = drawGameScreen,
	.mGetNextScreen = getNextGameScreenScreen
};