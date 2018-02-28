#include "senpai_creditscreen.h"

#include <prism/animation.h>
#include <prism/physicshandler.h>
#include <prism/screeneffect.h>
#include <prism/input.h>

#include "senpai_titlescreen.h"

static struct {
	TextureData mTexture;
	int mPhysicsID;
	int mAnimationID;
} gData;

static void loadCreditScreen() {
	gData.mTexture = loadTexture("assets/main/senpai/credits/CREDITS.pkg");

	gData.mPhysicsID = addToPhysicsHandler(makePosition(0, 600, 0));
	addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(0, -1, 0));

	gData.mAnimationID = playOneFrameAnimationLoop(makePosition(0, 0, 1), &gData.mTexture);
	setAnimationBasePositionReference(gData.mAnimationID, getHandledPhysicsPositionReference(gData.mPhysicsID));
}

static void gotoTitleCB(void* tCaller) {
	setNewScreen(&SenpaiTitleScreen);
}

static void updateCreditScreen() {
	Position p = *getHandledPhysicsPositionReference(gData.mPhysicsID);

	if (hasPressedAbortFlank()) {
		setNewScreen(&SenpaiTitleScreen);
	}

	if (p.y < -1000 || hasPressedAFlank() || hasPressedStartFlank()) {
		addFadeOut(30, gotoTitleCB, NULL);
	}
}

Screen SenpaiCreditScreen = {
	.mLoad = loadCreditScreen,
	.mUpdate = updateCreditScreen,
};