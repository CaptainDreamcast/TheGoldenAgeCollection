#include "slo_selectscreen.h"

#include <tari/mugenanimationhandler.h>
#include <tari/input.h>
#include <tari/tweening.h>
#include <tari/screeneffect.h>
#include <tari/sound.h>

#include "../../maingamemenu.h"

static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;

	int mTopAnimationID;
	int mBottomAnimationID;

	int mIsTransitioning;
	int mSelected;

} gData;

static void loadSelectScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());

	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/main/slo/SLOMODO.sff");
	gData.mAnimations = loadMugenAnimationFile("assets/main/slo/SLOMODO.air");

	gData.mBottomAnimationID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 10 + gData.mSelected), &gData.mSprites, makePosition(0, 0, 1));

	gData.mIsTransitioning = 0;
	addFadeIn(30, NULL, NULL);

	playTrack(22);
}

static void fadingOverCB(void* tCaller) {
	(void)tCaller;

	removeMugenAnimation(gData.mBottomAnimationID);
	gData.mBottomAnimationID = gData.mTopAnimationID;
	setMugenAnimationPosition(gData.mBottomAnimationID, makePosition(0, 0, 1));

	gData.mIsTransitioning = 0;
}

static void fadeToNewSelection() {
	gData.mTopAnimationID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 10 + gData.mSelected), &gData.mSprites, makePosition(0, 0, 2));
	tweenDouble(getMugenAnimationTransparencyReference(gData.mTopAnimationID), 0, 1, quadraticTweeningFunction, 10, fadingOverCB, NULL);

	if (gData.mSelected == 0) {
		setWrapperTimeDilatation(0.5);
	}
	else if (gData.mSelected == 1) {
		setWrapperTimeDilatation(1);
	}
	else {
		setWrapperTimeDilatation(2);
	}

	gData.mIsTransitioning = 1;
}

static void updateSelection() {
	if (gData.mIsTransitioning) return;

	if (hasPressedLeftFlank()) {
		gData.mSelected--;
		if (gData.mSelected < 0) gData.mSelected += 3;
		fadeToNewSelection();
	}
	else if (hasPressedRightFlank()) {
		gData.mSelected = (gData.mSelected + 1) % 3;
		fadeToNewSelection();
	}
}

static void gotoMainGameMenu(void* tCaller) {
	(void)tCaller;
	setNewScreen(&MainGameMenu);
}

static void updateConfirmation() {
	if (hasPressedAFlank() || hasPressedStartFlank()) {
		addFadeOut(30, gotoMainGameMenu, NULL);
		gData.mIsTransitioning = 1;
	}
}

static void gotoMainMenuCB(void* tCaller) {
	(void)tCaller;
	setNewScreen(&MainGameMenu);
}

static void updateSelectScreen() {

	updateSelection();
	updateConfirmation();

	if (hasPressedBFlank()) {
		addFadeOut(30, gotoMainMenuCB, NULL);
	}

	if (hasPressedAbortFlank() || hasPressedBFlank()) {
		setNewScreen(&MainGameMenu);
	}
}

Screen SloSelectScreen = {
	.mLoad = loadSelectScreen,
	.mUpdate = updateSelectScreen,
};