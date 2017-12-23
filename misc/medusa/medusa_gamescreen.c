#include "medusa_gamescreen.h"

#include <stdio.h>

#include <tari/input.h>
#include <tari/actorhandler.h>
#include <tari/collisionhandler.h>

#include "medusa_badasshandler.h"
#include "medusa_shothandler.h"
#include "medusa_toucan.h"
#include "medusa_titlescreen.h"
#include "medusa_collision.h"
#include "medusa_resources.h"

static void loadGameScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());

	loadMedusaResources();
	setupMedusaCollisions();
	instantiateActor(MedusaBadassHandler);
	instantiateActor(MedusaShotHandler);
	instantiateActor(MedusaToucan);

	// activateCollisionHandlerDebugMode();
}


static void updateGameScreen() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&MedusaTitleScreen);
	}
}


Screen MedusaGameScreen = {
	.mLoad = loadGameScreen,
	.mUpdate = updateGameScreen,
};
