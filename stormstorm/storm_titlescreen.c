#include "storm_titlescreen.h" 

#include <tari/input.h>

#include "../maingamemenu.h"
#include "vectrex_simulation.h"
#include "vectrexanimations.h"
#include "storm_gamescreen.h"
#include "storm_gameoverscreen.h"
#include "storm_particlehandler.h"

static void loadStormTitleScreen() {

	loadVectrexSimulation();
	instantiateActor(StormParticleHandler);
	addVectrexImage("assets/stormstorm/title/TITLE.pkg", makePosition(200,140,3));
}


static void updateParticles() {
	int amount = 40;
	int active = getStormTitleParticleAmount();

	int needed = amount - active;
	int i;
	for (i = 0; i < needed; i++) {
		addStormTitleSparcleParticle();
	}
}

static void updateScreenInput() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&MainGameMenu);
	}
	else if (hasPressedStartFlank() || hasPressedAFlank()) {
		setNewScreen(&StormGameScreen);
	}
}

static void updateStormTitleScreen() {
	updateParticles();
	updateScreenInput();
}

Screen StormTitleScreen = {
	.mLoad = loadStormTitleScreen,
	.mUpdate = updateStormTitleScreen,
};
