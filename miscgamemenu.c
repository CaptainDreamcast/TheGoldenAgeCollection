#include "miscgamemenu.h"

#include <prism/input.h>
#include <prism/optionhandler.h>
#include <prism/screeneffect.h>
#include <prism/mugenanimationhandler.h>
#include <prism/sound.h>

#include "mainmenu.h"
#include "controlscreen.h"
#include "titlescreen.h"

#include "misc/luck/luck_main.h"
#include "misc/best/bestgameever.h"
#include "misc/sfmk/sfmk_main.h"
#include "misc/octo/octo_main.h"
#include "misc/medusa/eyeofthemedusa.h"
#include "misc/hack/hack_main.h"
#include "misc/object/object_main.h"

static struct {
	MugenSpriteFile mSprites;

	int mAnimationIDs[10];
	void(*mFuncs[10])();

	int mSelected;
 } gData;

static void gotoHankInLuckCB(void* tData) {
	(void)tData;
	startControlScreen(startHankInLuck, "misc", "LUCK", 1, &MiscGameMenu);
}

static void selectHankInLuckCB() {
	addFadeOut(30, gotoHankInLuckCB, NULL);
}


static void gotoTheBestGameEverCB(void* tData) {
	(void)tData;
	startControlScreen(startTheBestGameEver, "misc", "BEST", 1, &MiscGameMenu);
}

static void selectTheBestGameEverCB() {
	addFadeOut(30, gotoTheBestGameEverCB, NULL);
}

static void gotoStreetFighterVersusMortalKombatCB(void* tData) {
	(void)tData;
	startControlScreen(startStreetFighterVersusMortalKombat, "misc", "SFMK", 1, &MiscGameMenu);
}

static void selectStreetFighterVersusMortalKombatCB() {
	addFadeOut(30, gotoStreetFighterVersusMortalKombatCB, NULL);
}

static void gotoEyeOfTheMedusaCB(void* tData) {
	(void)tData;
	startControlScreen(startEyeOfTheMedusa, "misc", "MEDUSA", 1, &MiscGameMenu);
}

static void selectEyeOfTheMedusaCB() {
	addFadeOut(30, gotoEyeOfTheMedusaCB, NULL);
}

static void gotoOctolord3CB(void* tData) {
	(void)tData;
	startControlScreen(startOctolord3, "misc", "OCTO", 0, &MiscGameMenu);
}

static void selectOctolord3CB() {
	addFadeOut(30, gotoOctolord3CB, NULL);
}

static void gotoHackathonSimulator2017CB(void* tData) {
	(void)tData;
	startControlScreen(startHackathonSimulator2017, "misc", "HACK", 1, &MiscGameMenu);
}

static void selectHackathonSimulator2017CB() {
	addFadeOut(30, gotoHackathonSimulator2017CB, NULL);
}

static void gotoObjectificationCB(void* tData) {
	(void)tData;
	startControlScreen(startObjectification, "misc", "OBJECT", 1, &MiscGameMenu);
}

static void selectObjectificationCB() {
	addFadeOut(30, gotoObjectificationCB, NULL);
}

static void gotoMainMenuCB(void* tCaller) {
	setNewScreen(&MainMenu);
}

static void setSelectedActive() {
	int i;
	for (i = 0; i < 7; i++) {
		if (i == gData.mSelected) {
			setMugenAnimationTransparency(gData.mAnimationIDs[i], 1);
		}
		else {
			setMugenAnimationTransparency(gData.mAnimationIDs[i], 0.5);
		}
	}


}

static int gLeft[] = {
	6, 4, 0, 5, 1, 3, 2
};

static int gRight[] = {
	2, 4, 6, 5, 1, 3, 0
};

static int gUp[] = {
	4, 5, 1, 6, 3, 2, 4
};

static int gDown[] = {
	3, 2, 5, 4, 6, 1, 3
};

static void loadMiscGameMenu() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());
	
	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/menu/misc/MISC.sff");

	addMugenAnimation(createOneFrameMugenAnimationForSprite(0, 0), &gData.mSprites, makePosition(0, 0, 1));

	gData.mAnimationIDs[0] = addMugenAnimation(createOneFrameMugenAnimationForSprite(1, 0), &gData.mSprites, makePosition(322, 240, 3));
	gData.mAnimationIDs[1] = addMugenAnimation(createOneFrameMugenAnimationForSprite(1, 1), &gData.mSprites, makePosition(640, 0, 2));
	gData.mAnimationIDs[2] = addMugenAnimation(createOneFrameMugenAnimationForSprite(1, 2), &gData.mSprites, makePosition(640, 236, 2));
	gData.mAnimationIDs[3] = addMugenAnimation(createOneFrameMugenAnimationForSprite(1, 3), &gData.mSprites, makePosition(0, 480, 2));
	gData.mAnimationIDs[4] = addMugenAnimation(createOneFrameMugenAnimationForSprite(1, 4), &gData.mSprites, makePosition(0, 0, 2));
	gData.mAnimationIDs[5] = addMugenAnimation(createOneFrameMugenAnimationForSprite(1, 5), &gData.mSprites, makePosition(640, 480, 2));
	gData.mAnimationIDs[6] = addMugenAnimation(createOneFrameMugenAnimationForSprite(1, 6), &gData.mSprites, makePosition(0, 236, 2));
	
	gData.mFuncs[0] = selectTheBestGameEverCB;
	gData.mFuncs[1] = selectHackathonSimulator2017CB;
	gData.mFuncs[2] = selectHankInLuckCB;
	gData.mFuncs[3] = selectEyeOfTheMedusaCB;
	gData.mFuncs[4] = selectObjectificationCB;
	gData.mFuncs[5] = selectOctolord3CB;
	gData.mFuncs[6] = selectStreetFighterVersusMortalKombatCB;

	addMugenAnimation(createOneFrameMugenAnimationForSprite(2, 0), &gData.mSprites, makePosition(0, 0, 4));

	setSelectedActive();

	playTrack(21);

	addFadeIn(30, NULL, NULL);
}

static void updateDirectionalInput() {
	if (hasPressedRightFlank()) {
		int from = gData.mSelected;
		gData.mSelected = gRight[gData.mSelected];
		gLeft[gData.mSelected] = from;
		setSelectedActive();
	} else if (hasPressedLeftFlank()) {
		int from = gData.mSelected;
		gData.mSelected = gLeft[gData.mSelected];
		gRight[gData.mSelected] = from;
		setSelectedActive();
	}
	
	if (hasPressedUpFlank()) {
		int from = gData.mSelected;
		gData.mSelected = gUp[gData.mSelected];
		gDown[gData.mSelected] = from;
		setSelectedActive();
	}
	else if (hasPressedDownFlank()) {
		int from = gData.mSelected;
		gData.mSelected = gDown[gData.mSelected];
		gUp[gData.mSelected] = from;
		setSelectedActive();
	}
}

static void updateSelectionInput() {
	if (hasPressedAFlank() || hasPressedStartFlank()) {
		gData.mFuncs[gData.mSelected]();
	}
}

static void updateMiscGameMenu() {
	updateDirectionalInput();
	updateSelectionInput();

	if (hasPressedBFlank()) {
		addFadeOut(30, gotoMainMenuCB, NULL);
	}

	if (hasPressedAbortFlank()) {
		setNewScreen(&TitleScreen);
	}
}

Screen MiscGameMenu = {
	.mLoad = loadMiscGameMenu,
	.mUpdate = updateMiscGameMenu,
};
