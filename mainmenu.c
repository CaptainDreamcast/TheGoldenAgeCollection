#include "mainmenu.h"

#include <tari/input.h>
#include <tari/optionhandler.h>
#include <tari/screeneffect.h>
#include <tari/mugenanimationhandler.h>
#include <tari/tweening.h>

#include "maingamemenu.h"
#include "miscgamemenu.h"
#include "titlescreen.h"

static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;

	int mBGID1;
	int mBGID2;

	int mTextID1;
	int mTextID2;

	int mSelected;
	int mIsUpdatingSelection;

	double mMainScaleFactor;
} gData;

static void gotoMainGameMenu(void* tCaller) {
	(void)tCaller;
	setNewScreen(&MainGameMenu);
}

static void selectMainGameMenu() {
	addFadeOut(30, gotoMainGameMenu, NULL);
}

static void gotoMiscGameMenu(void* tCaller) {
	(void)tCaller;
	setNewScreen(&MiscGameMenu);
}

static void selectMiscGameMenu() {
	addFadeOut(30, gotoMiscGameMenu, NULL);
}

static void selectBonusGames();
static void selectMainGames();

static void loadMainMenu() {
	gData.mMainScaleFactor = 1.25;


	instantiateActor(getMugenAnimationHandlerActorBlueprint());
	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/menu/type/TYPE.sff");
	gData.mAnimations = loadMugenAnimationFile("assets/menu/type/TYPE.air");

	gData.mBGID1 = addMugenAnimation(createOneFrameMugenAnimationForSprite(0, 0), &gData.mSprites, makePosition(320, 0, 2));
	setMugenAnimationBaseDrawScale(gData.mBGID1, gData.mMainScaleFactor);

	gData.mBGID2 = addMugenAnimation(createOneFrameMugenAnimationForSprite(0, 1), &gData.mSprites, makePosition(320, 480, 1));

	gData.mTextID1 = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 21), &gData.mSprites, makePosition(320, 120, 3));
	gData.mTextID2 = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 10), &gData.mSprites, makePosition(320, 360, 3));

	gData.mIsUpdatingSelection = 0;

	if (gData.mSelected) {
		selectBonusGames();
	} else{
		selectMainGames();
	}

	

	addFadeIn(30, NULL, NULL);
}

static void selectionUpdateOverCB(void* tCaller) {
	(void)tCaller;
	gData.mIsUpdatingSelection = 0;
}

static void selectMainGames() {
	setMugenAnimationPosition(gData.mBGID2, makePosition(320, 480, 1));
	tweenDouble(getMugenAnimationBaseScaleReference(gData.mBGID2), gData.mMainScaleFactor, 1, linearTweeningFunction, 10, NULL, NULL);
	tweenDouble(&getMugenAnimationPositionReference(gData.mTextID2)->y, 360 - 120 * 0.25, 360, linearTweeningFunction, 10, NULL, NULL);
	changeMugenAnimation(gData.mTextID2, getMugenAnimation(&gData.mAnimations, 10));

	setMugenAnimationPosition(gData.mBGID1, makePosition(320, 0, 2));
	tweenDouble(getMugenAnimationBaseScaleReference(gData.mBGID1), 1, gData.mMainScaleFactor, linearTweeningFunction, 10, NULL, NULL);
	tweenDouble(&getMugenAnimationPositionReference(gData.mTextID1)->y, 120, 120 * gData.mMainScaleFactor, linearTweeningFunction, 10, selectionUpdateOverCB, NULL);
	changeMugenAnimation(gData.mTextID1, getMugenAnimation(&gData.mAnimations, 21));

	gData.mIsUpdatingSelection = 1;
	gData.mSelected = 0;
}


static void selectBonusGames() {
	setMugenAnimationPosition(gData.mBGID1, makePosition(320, 0, 1));
	tweenDouble(getMugenAnimationBaseScaleReference(gData.mBGID1), gData.mMainScaleFactor, 1, linearTweeningFunction, 10, NULL, NULL);
	tweenDouble(&getMugenAnimationPositionReference(gData.mTextID1)->y, 120 * gData.mMainScaleFactor, 120, linearTweeningFunction, 10, NULL, NULL);
	changeMugenAnimation(gData.mTextID1, getMugenAnimation(&gData.mAnimations, 20));
	
	setMugenAnimationPosition(gData.mBGID2, makePosition(320, 480, 2));
	tweenDouble(getMugenAnimationBaseScaleReference(gData.mBGID2), 1, gData.mMainScaleFactor, linearTweeningFunction, 10, NULL, NULL);
	tweenDouble(&getMugenAnimationPositionReference(gData.mTextID2)->y, 360, 360 - 120*0.25, linearTweeningFunction, 10, selectionUpdateOverCB, NULL);
	changeMugenAnimation(gData.mTextID2, getMugenAnimation(&gData.mAnimations, 11));

	gData.mIsUpdatingSelection = 1;
	gData.mSelected = 1;
}

static void changeSelection() {
	if (gData.mIsUpdatingSelection) return;

	if (gData.mSelected) {
		selectMainGames();
	}
	else {
		selectBonusGames();
	}

}

static void selectGame() {
	if (gData.mSelected) {
		selectMiscGameMenu();
	}
	else {
		selectMainGameMenu();
	}
}

static void updateScreenInput() {
	if (hasPressedUpFlank() || hasPressedDownFlank()) {
		changeSelection();
	}

	if (hasPressedAFlank() || hasPressedStartFlank()) {
		selectGame();
	}
}

static void updateMainMenu() {
	updateScreenInput();
	
	if (hasPressedAbortFlank()) {
		setNewScreen(&TitleScreen);
	}
}

Screen MainMenu = {
	.mLoad = loadMainMenu,
	.mUpdate = updateMainMenu,
};
