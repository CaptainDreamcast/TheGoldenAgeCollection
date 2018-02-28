#include "storm_gamescreen.h"

#include <prism/input.h>
#include <prism/physicshandler.h>
#include <prism/collisionhandler.h>

#include "storm_titlescreen.h"
#include "vectrex_simulation.h"
#include "storm_paddle.h"
#include "storm_collision.h"
#include "storm_ball.h"
#include "storm_enemyhandler.h"
#include "storm_particlehandler.h"
#include "storm_ui.h"

static void loadStormGameScreen() {

	loadVectrexSimulation();
	loadStormCollisions();
	instantiateActor(StormUIHandler);
	instantiateActor(StormParticleHandler);
	instantiateActor(getStormPaddleBP());
	instantiateActor(StormBall);
	instantiateActor(StormEnemyHandler);

	resetStormUI();
	// activateCollisionHandlerDebugMode();
}

static void updateStormGameScreen() {

	if (hasPressedAbortFlank()) {
		setNewScreen(&StormTitleScreen);
	}	
}

Screen StormGameScreen = {
	.mLoad = loadStormGameScreen,
	.mUpdate = updateStormGameScreen,
};