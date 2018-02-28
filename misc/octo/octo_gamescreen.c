#include "octo_gamescreen.h"

#include <prism/actorhandler.h>
#include <prism/input.h>
#include <prism/mugenanimationhandler.h>
#include <prism/sound.h>

#include "octo_bg.h"
#include "octo_collision.h"
#include "octo_enemyhandler.h"
#include "octo_player.h"
#include "octo_titlescreen.h"
#include "octo_assethandler.h"

static void loadGameScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());
	instantiateActor(getMicrophoneHandlerActorBlueprint());
	setupOctoCollisions();
	setupOctoAssets();
	instantiateActor(OctoBackgroundHandler);
	instantiateActor(OctoEnemyHandler);
	instantiateActor(OctoPlayer);
	
}

static void updateGameScreen() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&OctoTitleScreen);
	}
}

Screen OctoGameScreen = {
	.mLoad = loadGameScreen,
	.mUpdate = updateGameScreen,
};