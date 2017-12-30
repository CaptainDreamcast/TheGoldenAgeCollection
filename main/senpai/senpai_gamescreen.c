#include "senpai_gamescreen.h"

#include <tari/input.h>
#include <tari/mugenanimationhandler.h>

#include "senpai_titlescreen.h"
#include "senpai_levelhandler.h"
#include "senpai_player.h"
#include "senpai_enemyhandler.h"
#include "senpai_finalboss.h"

static void loadGameScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());

	instantiateActor(SenpaiEnemyHandler);
	instantiateActor(SenpaiPlayer);
	instantiateActor(SenpaiLevelHandler);
	instantiateActor(SenpaiFinalBoss);
}

static void updateGameScreen() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&SenpaiTitleScreen);
	}
}

Screen SenpaiGameScreen = {
	.mLoad = loadGameScreen,
	.mUpdate = updateGameScreen
};