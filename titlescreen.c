#include "titlescreen.h"

#include <tari/animation.h>
#include <tari/screeneffect.h>
#include <tari/input.h>
#include <tari/system.h>
#include <tari/mugenanimationhandler.h>
#include <tari/tweening.h>
#include <tari/math.h>
#include <tari/physicshandler.h>
#include <tari/tweening.h>
#include <tari/sound.h>

#include "mainmenu.h"

static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;

	MugenSpriteFile mBGSprites;
	MugenAnimations mBGAnimations;

	int mCDID;

	int mBGPhysicsID1;
	int mBGID1;
	int mBGPhysicsID2;
	int mBGID2;



	int mIsFadingOver;
} gData;

static void tweenFrontSmallCB(void* tCaller);

static void loadTitleScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());

	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/menu/title/TITLE.sff");
	gData.mAnimations = loadMugenAnimationFile("assets/menu/title/TITLE.air");

	gData.mBGSprites = loadMugenSpriteFileWithoutPalette("assets/menu/title/BGS.sff");
	gData.mBGAnimations = loadMugenAnimationFile("assets/menu/title/BGS.air");


	addMugenAnimation(createOneFrameMugenAnimationForSprite(0, 0), &gData.mSprites, makePosition(0, 0, 10));
	gData.mCDID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 10), &gData.mSprites, makePosition(320, 200, 20));
	tweenDouble(getMugenAnimationScaleXReference(gData.mCDID), 1, 0, quadraticTweeningFunction, 180, tweenFrontSmallCB, NULL);

	gData.mBGPhysicsID1 = addToPhysicsHandler(makePosition(0, -120, 0));
	addAccelerationToHandledPhysics(gData.mBGPhysicsID1, makePosition(-1, 0, 0));
	gData.mBGID1 = addMugenAnimation(getMugenAnimation(&gData.mBGAnimations, randfromInteger(10, 22)), &gData.mBGSprites, makePosition(0, 0, 2));
	setMugenAnimationBaseDrawScale(gData.mBGID1, 8);
	setMugenAnimationBasePosition(gData.mBGID1, getHandledPhysicsPositionReference(gData.mBGPhysicsID1));
	gData.mBGID2 = -1;
	gData.mIsFadingOver = 0;

	playTrack(21);

	addFadeIn(30, NULL, NULL);
}

static void tweenFrontBigCB(void* tCaller) {
	tweenDouble(getMugenAnimationScaleXReference(gData.mCDID), 1, 0, quadraticTweeningFunction, 180, tweenFrontSmallCB, NULL);
}

static void tweenBackSmallCB(void* tCaller) {
	changeMugenAnimation(gData.mCDID, getMugenAnimation(&gData.mAnimations, 10));
	tweenDouble(getMugenAnimationScaleXReference(gData.mCDID), 0, 1, inverseQuadraticTweeningFunction, 180, tweenFrontBigCB, NULL);
}

static void tweenBackBigCB(void* tCaller) {
	tweenDouble(getMugenAnimationScaleXReference(gData.mCDID), 1, 0, quadraticTweeningFunction, 180, tweenBackSmallCB, NULL);
}

static void tweenFrontSmallCB(void* tCaller) {
	changeMugenAnimation(gData.mCDID, getMugenAnimation(&gData.mAnimations, 11));
	tweenDouble(getMugenAnimationScaleXReference(gData.mCDID), 0, 1, inverseQuadraticTweeningFunction, 180, tweenBackBigCB, NULL);
}

static void gotoMainMenu(void* tCaller) {
	(void)tCaller;
	setNewScreen(&MainMenu);
}

static void fadeOverOverCB(void* tCaller) {
	removeMugenAnimation(gData.mBGID1);
	removeFromPhysicsHandler(gData.mBGPhysicsID1);

	setMugenAnimationPosition(gData.mBGID2, makePosition(0, 0, 2));
	gData.mBGID1 = gData.mBGID2;
	gData.mBGPhysicsID1 = gData.mBGPhysicsID2;
	gData.mIsFadingOver = 0;
}

static void startFadeOver() {
	gData.mBGPhysicsID2 = addToPhysicsHandler(makePosition(0, -120, 0));
	addAccelerationToHandledPhysics(gData.mBGPhysicsID2, makePosition(-1, 0, 0));
	gData.mBGID2 = addMugenAnimation(getMugenAnimation(&gData.mBGAnimations, randfromInteger(10, 22)), &gData.mBGSprites, makePosition(0, 0, 1));
	setMugenAnimationBaseDrawScale(gData.mBGID2, 8);
	setMugenAnimationBasePosition(gData.mBGID2, getHandledPhysicsPositionReference(gData.mBGPhysicsID2));

	tweenDouble(getMugenAnimationTransparencyReference(gData.mBGID1), 1, 0, linearTweeningFunction, 30, fadeOverOverCB, NULL);
	gData.mIsFadingOver = 1;
}

static void updateScrollingBackground() {
	if (gData.mIsFadingOver) return;
	Position p = *getHandledPhysicsPositionReference(gData.mBGPhysicsID1);

	if (p.x < -600) {
		startFadeOver();
	}

}

static void updateTitleScreen() {
	updateScrollingBackground();

	if (hasPressedAbortFlank()) {
		abortScreenHandling();
	}

	if (hasPressedStartFlank()) {
		addFadeOut(30, gotoMainMenu, NULL);
	}
}

Screen TitleScreen = {
	.mLoad = loadTitleScreen,
	.mUpdate = updateTitleScreen,
};
