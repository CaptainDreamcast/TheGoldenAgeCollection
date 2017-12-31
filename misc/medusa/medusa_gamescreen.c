#include "medusa_gamescreen.h"

#include <stdio.h>

#include <tari/input.h>
#include <tari/actorhandler.h>
#include <tari/collisionhandler.h>
#include <tari/sound.h>
#include <tari/timer.h>

#include "medusa_badasshandler.h"
#include "medusa_shothandler.h"
#include "medusa_toucan.h"
#include "medusa_titlescreen.h"
#include "medusa_collision.h"
#include "medusa_resources.h"

static struct {
	int mLevel;
} gData;

static void reloadScreenCB(void* tData) {
	(void)tData;

	gData.mLevel++;
	setNewScreen(&MedusaGameScreen);
}

static void loadGameScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());

	loadMedusaResources();
	setupMedusaCollisions();
	instantiateActor(MedusaBadassHandler);
	instantiateActor(MedusaShotHandler);
	instantiateActor(MedusaToucan);

	playTrackOnce(16+gData.mLevel);

	if (gData.mLevel == 0) {
		addTimerCB(1380, reloadScreenCB, NULL);
	}

	// activateCollisionHandlerDebugMode();
}

static void updateGameScreen() {
	if (hasPressedAbortFlank() ||hasPressedRFlank()) {
		setNewScreen(&MedusaTitleScreen);
	}
}


Screen MedusaGameScreen = {
	.mLoad = loadGameScreen,
	.mUpdate = updateGameScreen,
};

void resetMedusaLevel()
{
	gData.mLevel = 0;
}
