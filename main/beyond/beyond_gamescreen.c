#include "beyond_gamescreen.h"

#include <stdio.h>

#include <tari/input.h>
#include <tari/mugenanimationhandler.h>
#include <tari/collisionhandler.h>
#include <tari/sound.h>

#include "beyond_player.h"
#include "beyond_bg.h"
#include "beyond_ui.h"
#include "beyond_enemyhandler.h"
#include "beyond_level.h"
#include "beyond_shothandler.h"
#include "beyond_collision.h"
#include "beyond_boss.h"
#include "beyond_assignment.h"
#include "beyond_itemhandler.h"
#include "beyond_effecthandler.h"
#include "beyond_continuehandler.h"
#include "beyond_gameoptionhandler.h"
#include "beyond_titlescreen.h"
#include "beyond_finalbossscene.h"
#include "beyond_main.h"

static void playGameMusic() {
	if (!isUltimateFrontier() && getBeyondCurrentLevel() == 4) {
		playTrack(14);
	}	
}

static void loadGameScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());
	
	loadBeyondCollisions();
	instantiateActor(BeyondAssignmentHandler);
	instantiateActor(BeyondContinueHandler);
	instantiateActor(BeyondGameOptionHandler);
	instantiateActor(BeyondEffectHandler);
	instantiateActor(BeyondItemHandler);
	instantiateActor(BeyondEnemyHandler);
	instantiateActor(BeyondBackgroundHandler);
	instantiateActor(BeyondUserInterface);
	instantiateActor(BeyondPlayer);
	instantiateActor(BeyondShotHandler);
	instantiateActor(BeyondBossHandler);
	instantiateActor(BeyondFinalBossSceneHandler);

	instantiateActor(BeyondLevelHandler);

	playGameMusic();

	// activateCollisionHandlerDebugMode();
}

static void updateGameScreen() {


	if (hasPressedAbortFlank()) {
		setNewScreen(&BeyondTitleScreen);
	}
}

Screen BeyondGameScreen = {
	.mLoad = loadGameScreen,
	.mUpdate = updateGameScreen,
};
