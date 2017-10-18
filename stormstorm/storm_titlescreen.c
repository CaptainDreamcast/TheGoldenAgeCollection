#include "storm_titlescreen.h" 

#include <tari/input.h>

#include "../maingamemenu.h"
#include "vectrex_simulation.h"
#include "vectrexanimations.h"

static void loadStormTitleScreen() {
	loadVectrexSimulation();
	addVectrexImage("assets/stormstorm/title/TITLE.pkg", makePosition(200,140,2));
}


static void updateStormTitleScreen() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&MainGameMenu);
	}
}

Screen StormTitleScreen = {
	.mLoad = loadStormTitleScreen,
	.mUpdate = updateStormTitleScreen,
};