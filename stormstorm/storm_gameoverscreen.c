#include "storm_gameoverscreen.h"

#include <prism/input.h>

#include "vectrex_simulation.h"
#include "vectrexanimations.h"
#include "storm_titlescreen.h"
#include "storm_ui.h"

static void loadStormGameOverScreen() {

	loadVectrexSimulation();
	instantiateActor(StormUIHandler);

	addVectrexImage("assets/stormstorm/gameover/GAMEOVER.pkg", makePosition(170, 140, 2));
	setStormUIScorePosition(makePosition(460, 208, 4));
}

static void updateStormGameOverScreen() {

	if (hasPressedAbortFlank()) {
		setNewScreen(&StormTitleScreen);
	}
	else if (hasPressedStartFlank() || hasPressedAFlank()) {
		setNewScreen(&StormTitleScreen);
	}
}

Screen StormGameOverScreen = {
	.mLoad = loadStormGameOverScreen,
	.mUpdate = updateStormGameOverScreen,
};
