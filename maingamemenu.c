#include "maingamemenu.h"

#include <tari/input.h>
#include <tari/optionhandler.h>
#include <tari/screeneffect.h>

#include "stormstorm/stormstorm.h"
#include "bombx/bombx.h"

static void gotoStormStormCB(void* tData) {
	(void)tData;
	startStormStorm();
}

static void selectStormStormCB(void* tData) {
	addFadeOut(30, gotoStormStormCB, NULL);
}

static void gotoBombxCB(void* tData) {
	(void)tData;
	startBombx();
}

static void selectBombxCB(void* tData) {
	addFadeOut(30, gotoBombxCB, NULL);
}


static void loadMainGameMenu() {
	instantiateActor(getOptionHandlerBlueprint());
	setOptionButtonA();
	setOptionTextSize(20);
	setOptionTextBreakSize(-5);

	addOption(makePosition(50, 100, 1), "Bombx", selectBombxCB, NULL);
	addOption(makePosition(50, 130, 1), "Storm Storm", selectStormStormCB, NULL);


	addFadeIn(30, NULL, NULL);
}

static void updateMainGameMenu() {
	if (hasPressedAbortFlank()) {
		abortScreenHandling();
	}
}

Screen MainGameMenu = {
	.mLoad = loadMainGameMenu,
	.mUpdate = updateMainGameMenu,
};
