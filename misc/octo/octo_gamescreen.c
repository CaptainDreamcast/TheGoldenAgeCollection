#include "octo_gamescreen.h"

#include <tari/actorhandler.h>
#include <tari/input.h>
#include <tari/mugenanimationhandler.h>
#include <tari/sound.h>

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