#include "mirk_gamescreen.h"

#include <stdio.h>

#include <tari/actorhandler.h>
#include <tari/input.h>
#include <tari/collisionhandler.h>
#include <tari/stagehandler.h>

#include "mirk_stage.h"
#include "mirk_mirklinghandler.h"
#include "mirk_player.h"
#include "mirk_deathcount.h"
#include "mirk_preciouspeople.h"
#include "mirk_upgrades.h"
#include "mirk_collision.h"
#include "mirk_particles.h"
#include "mirk_levelhandler.h"
#include "mirk_titlescreen.h"
#include "mirk_gameoverscreen.h"
#include "mirk_soundeffect.h"
#include "mirk_explosion.h"
#include "mirk_routehandler.h"
#include "mirk_noise.h"
#include "mirk_pausemenu.h"

static void loadGameScreen() {
	forceMouseCursorToWindow();

	initMirkGameSoundEffects();
	loadMirkMirklingsCollisions();
	instantiateActor(MirkStageBP);
	instantiateActor(MirkMirklingHandlerBP);
	loadMirkPreciousPeople();
	loadMirkUpgrades();
	instantiateActor(MirkDeathCountBP);
	instantiateActor(MirkPlayerBP);
	instantiateActor(MirkPauseMenuBP);
	loadMirkParticles();
	loadMirkExplosions();
	loadMirkScreenNoise();
	startNewMirkRoute();

	// activateCollisionHandlerDebugMode();
	// setCollisionHandlerDebuggingScreenPositionReference(getMirkStagePositionReference());
}

static void updateGameScreen() {
	updateMirkParticles();
	updateMirkRouteHandler();
}

static Screen* getNextGameScreenScreen() {
	if (hasPressedAbortFlank()) {
		return &MirkTitleScreen;
	}

	if (!hasMirkPreciousPeopleLeft()) {
		return &MirkGameOverScreen;
	}

	return NULL;
}

static void unloadGameScreen() {
	releaseMouseCursorFromWindow();
}

Screen MirkGameScreen = {
	.mLoad = loadGameScreen,
	.mUnload = unloadGameScreen,
	.mDraw = NULL,
	.mUpdate = updateGameScreen,
	.mGetNextScreen = getNextGameScreenScreen
};
