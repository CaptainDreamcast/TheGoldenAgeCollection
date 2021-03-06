#include "maingamemenu.h"

#include <prism/input.h>
#include <prism/optionhandler.h>
#include <prism/screeneffect.h>
#include <prism/mugenanimationhandler.h>
#include <prism/tweening.h>
#include <prism/sound.h>

#include "titlescreen.h"
#include "mainmenu.h"
#include "controlscreen.h"
#include "stormstorm/stormstorm.h"
#include "bombx/bombx.h"
#include "main/beyond/beyond_main.h"
#include "main/hank/hank_main.h"
#include "main/justice/justice_main.h"
#include "main/cane/cane_main.h"
#include "main/mirk/mirk_main.h"
#include "main/norm/norm_main.h"
#include "main/fight/fight_main.h"
#include "main/senpai/senpai_main.h"
#include "main/slo/slo_main.h"

static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;

	MugenSpriteFile mBGSprites;
	MugenAnimations mBGAnimations;

	int mBGID;
	int mUpperCalendarID;
	int mLowerCalendarID;
	int mTextID;

	int mCalendarFlipID;
	int mBGFadeID;

	int mSelected;
	int mIsFlipping;

	void(*mFuncs[20])();
} gData;

static void gotoStormStormCB(void* tData) {
	(void)tData;
	startControlScreen(startStormStorm, "main", "STORM", 1, &MainGameMenu);
}

static void selectStormStormCB() {
	addFadeOut(30, gotoStormStormCB, NULL);
}

static void gotoBombxCB(void* tData) {
	(void)tData;
	startControlScreen(startBombx, "main", "BOMBX", 1, &MainGameMenu);
}

static void selectBombxCB() {
	addFadeOut(30, gotoBombxCB, NULL);
}

static void gotoEyeOfTheMedusa2CB(void* tData) {
	(void)tData;
	startControlScreen(startEyeOfTheMedusa2, "main", "FRONTIER", 1, &MainGameMenu);
}

static void selectEyeOfTheMedusa2CB() {
	addFadeOut(30, gotoEyeOfTheMedusa2CB, NULL);
}

static void gotoEyeOfTheMedusa3CB(void* tData) {
	(void)tData;
	startControlScreen(startEyeOfTheMedusa3, "main", "BEYOND", 1, &MainGameMenu);
}

static void selectEyeOfTheMedusa3CB() {
	addFadeOut(30, gotoEyeOfTheMedusa3CB, NULL);
}

static void gotoGoldenAgeCB(void* tData) {
	(void)tData;
	setNewScreen(&TitleScreen);
}

static void selectGoldenAgeCB() {
	addFadeOut(30, gotoGoldenAgeCB, NULL);
}

static void gotoHazyHankCB(void* tData) {
	(void)tData;
	startControlScreen(startHazyHank, "main", "HANK", 1, &MainGameMenu);
}

static void selectHazyHankCB() {
	addFadeOut(30, gotoHazyHankCB, NULL);
}

static void gotoFistsOfJusticeCB(void* tData) {
	(void)tData;
	startControlScreen(startFistsOfJustice, "main", "JUSTICE", 1, &MainGameMenu);
}

static void selectFistsOfJusticeCB() {
	addFadeOut(30, gotoFistsOfJusticeCB, NULL);
}

static void gotoLaMorteDiUnCaneCB(void* tData) {
	(void)tData;
	startControlScreen(startLaMorteDiUnCane, "main", "CANE", 1, &MainGameMenu);
}

static void selectLaMorteDiUnCaneCB() {
	addFadeOut(30, gotoLaMorteDiUnCaneCB, NULL);
}

static void gotoMirklingsCB(void* tData) {
	(void)tData;
	startControlScreen(startMirklings, "main", "MIRKLINGS", 0, &MainGameMenu);
}

static void selectMirklingsCB() {
	addFadeOut(30, gotoMirklingsCB, NULL);
}

static void gotoQuestForNormalcyCB(void* tData) {
	(void)tData;
	startControlScreen(startQuestForNormalcy, "main", "NORM", 1, &MainGameMenu);
}

static void selectQuestForNormalcyCB() {
	addFadeOut(30, gotoQuestForNormalcyCB, NULL);
}

static void gotoDreamFight16CB(void* tData) {
	(void)tData;
	startControlScreen(startDreamFight16, "main", "FIGHT", 1, &MainGameMenu);
}

static void selectDreamFight16CB() {
	addFadeOut(30, gotoDreamFight16CB, NULL);
}

static void gotoSenpaiScreamsCB(void* tData) {
	(void)tData;
	startControlScreen(startSenpaiScreams, "main", "SENPAI", 1, &MainGameMenu);
}

static void selectSenpaiScreamsCB() {
	addFadeOut(30, gotoSenpaiScreamsCB, NULL);
}

static void gotoSloMoDoCB(void* tData) {
	(void)tData;
	startControlScreen(startSloMoDo, "main", "SLOMODO", 1, &MainGameMenu);
}

static void selectSloMoDoCB() {
	addFadeOut(30, gotoSloMoDoCB, NULL);
}

static void gotoMainMenuCB(void* tCaller) {
	(void)tCaller;

	setNewScreen(&MainMenu);
}

static void loadMainGameMenu() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());

	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/menu/main/MAIN.sff");
	gData.mAnimations = loadMugenAnimationFile("assets/menu/main/MAIN.air");

	gData.mBGSprites = loadMugenSpriteFileWithoutPalette("assets/menu/title/BGS.sff");
	gData.mBGAnimations = loadMugenAnimationFile("assets/menu/title/BGS.air"); 
	
	addMugenAnimation(createOneFrameMugenAnimationForSprite(0, 1), &gData.mSprites, makePosition(0, 0, 10));
	gData.mBGID = addMugenAnimation(getMugenAnimation(&gData.mBGAnimations, 10 + gData.mSelected), &gData.mBGSprites, makePosition(0, 0, 1));
	setMugenAnimationBaseDrawScale(gData.mBGID, 4);

	gData.mUpperCalendarID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 10 + gData.mSelected), &gData.mSprites, makePosition(31, 39 + 201, 20));
	gData.mLowerCalendarID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 30 + gData.mSelected), &gData.mSprites, makePosition(31, 39 + 201, 20));
	gData.mTextID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 50 + gData.mSelected), &gData.mSprites, makePosition(342, 62, 20));
	 
	gData.mFuncs[0] = selectEyeOfTheMedusa2CB;
	gData.mFuncs[1] = selectFistsOfJusticeCB;
	gData.mFuncs[2] = selectBombxCB;
	gData.mFuncs[3] = selectLaMorteDiUnCaneCB;
	gData.mFuncs[4] = selectSenpaiScreamsCB;
	gData.mFuncs[5] = selectMirklingsCB;
	gData.mFuncs[6] = selectQuestForNormalcyCB;
	gData.mFuncs[7] = selectDreamFight16CB;
	gData.mFuncs[8] = selectEyeOfTheMedusa3CB;
	gData.mFuncs[9] = selectStormStormCB;
	gData.mFuncs[10] = selectSloMoDoCB;
	gData.mFuncs[11] = selectGoldenAgeCB;
	gData.mFuncs[12] = selectHazyHankCB;

	gData.mIsFlipping = 0;

	playTrack(21);

	addFadeIn(30, NULL, NULL);
}

static void finalizeBGFade(void* tCaller) {
	removeMugenAnimation(gData.mBGFadeID);
}

static void finalizeFlippingUpCB(void* tCaller) {
	removeMugenAnimation(gData.mUpperCalendarID);

	gData.mUpperCalendarID = gData.mCalendarFlipID;
	setMugenAnimationPosition(gData.mUpperCalendarID, makePosition(31, 39 + 201, 20));

	gData.mTextID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 50 + gData.mSelected), &gData.mSprites, makePosition(342, 62, 20));

	gData.mIsFlipping = 0;
}

static void keepFlippingUpCB(void* tCaller) {
	removeMugenAnimation(gData.mCalendarFlipID);
	gData.mCalendarFlipID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 10 + gData.mSelected), &gData.mSprites, makePosition(31, 39 + 201, 21));

	tweenDouble(getMugenAnimationScaleYReference(gData.mCalendarFlipID), 0, 1, inverseQuadraticTweeningFunction, 10, finalizeFlippingUpCB, NULL);
}

static void flipCalendarUp() {
	gData.mCalendarFlipID = gData.mLowerCalendarID;

	gData.mSelected = (gData.mSelected + 1) % 13;
	gData.mLowerCalendarID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 30 + gData.mSelected), &gData.mSprites, makePosition(31, 39 + 201, 20));
	removeMugenAnimation(gData.mTextID);

	setMugenAnimationPosition(gData.mCalendarFlipID, makePosition(31, 39 + 201, 21));
	
	tweenDouble(getMugenAnimationScaleYReference(gData.mCalendarFlipID), 1, 0, quadraticTweeningFunction, 10, keepFlippingUpCB, NULL);

	gData.mBGFadeID = gData.mBGID;
	setMugenAnimationPosition(gData.mBGFadeID, makePosition(0, 0, 2));
	gData.mBGID = addMugenAnimation(getMugenAnimation(&gData.mBGAnimations, 10 + gData.mSelected), &gData.mBGSprites, makePosition(0, 0, 1));
	setMugenAnimationBaseDrawScale(gData.mBGID, 4);
	tweenDouble(getMugenAnimationTransparencyReference(gData.mBGFadeID), 1, 0, linearTweeningFunction, 19, finalizeBGFade, NULL);

	gData.mIsFlipping = 1;
}


static void finalizeFlippingDownCB(void* tCaller) {
	removeMugenAnimation(gData.mLowerCalendarID);

	gData.mLowerCalendarID = gData.mCalendarFlipID;
	setMugenAnimationPosition(gData.mLowerCalendarID, makePosition(31, 39 + 201, 20));

	gData.mTextID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 50 + gData.mSelected), &gData.mSprites, makePosition(342, 62, 20));

	gData.mIsFlipping = 0;
}

static void keepFlippingDownCB(void* tCaller) {
	removeMugenAnimation(gData.mCalendarFlipID);
	gData.mCalendarFlipID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 30 + gData.mSelected), &gData.mSprites, makePosition(31, 39 + 201, 21));

	tweenDouble(getMugenAnimationScaleYReference(gData.mCalendarFlipID), 0, 1, inverseQuadraticTweeningFunction, 10, finalizeFlippingDownCB, NULL);
}

static void flipCalendarDown() {
	gData.mCalendarFlipID = gData.mUpperCalendarID;

	gData.mSelected--;
	if (gData.mSelected < 0) gData.mSelected += 13;

	gData.mUpperCalendarID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 10 + gData.mSelected), &gData.mSprites, makePosition(31, 39 + 201, 20));
	removeMugenAnimation(gData.mTextID);

	setMugenAnimationPosition(gData.mCalendarFlipID, makePosition(31, 39 + 201, 21));

	tweenDouble(getMugenAnimationScaleYReference(gData.mCalendarFlipID), 1, 0, quadraticTweeningFunction, 10, keepFlippingDownCB, NULL);

	gData.mBGFadeID = gData.mBGID;
	setMugenAnimationPosition(gData.mBGFadeID, makePosition(0, 0, 2));
	gData.mBGID = addMugenAnimation(getMugenAnimation(&gData.mBGAnimations, 10 + gData.mSelected), &gData.mBGSprites, makePosition(0, 0, 1));
	setMugenAnimationBaseDrawScale(gData.mBGID, 4);
	tweenDouble(getMugenAnimationTransparencyReference(gData.mBGFadeID), 1, 0, linearTweeningFunction, 19, finalizeBGFade, NULL);

	gData.mIsFlipping = 1;
}

static void updateDirectionalInput() {
	if (gData.mIsFlipping) return;

	if (hasPressedUpFlank()) {
		flipCalendarUp();
	}
	else if (hasPressedDownFlank()) {
		flipCalendarDown();
	}

}

static void updateSelectionInput() {
	if (hasPressedAFlank() || hasPressedStartFlank()) {
		gData.mFuncs[gData.mSelected]();
	}

}

static void updateMainGameMenu() {
	updateDirectionalInput();
	updateSelectionInput();

	if (hasPressedBFlank()) {
		addFadeOut(30, gotoMainMenuCB, NULL);
	}

	if (hasPressedAbortFlank()) {
		setNewScreen(&TitleScreen);
	}
}

Screen MainGameMenu = {
	.mLoad = loadMainGameMenu,
	.mUpdate = updateMainGameMenu,
};
