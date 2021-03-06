#include "fight_warningscreen.h"

#include <prism/animation.h>
#include <prism/timer.h>
#include <prism/tweening.h>
#include <prism/screeneffect.h>
#include <prism/input.h>

#include "fight_titlescreen.h"
#include "fight_main.h"

static struct {
	double mWarningTrans;
	int mWarningID;
	TextureData mWarningTexture;

	double mThisIsTrans;
	int mThisIsID;
	TextureData mThisIsTexture;

	double mExplanationTrans;
	int mExplanationID;
	TextureData mExplanationTexture;
} gData;

static void fadeOutFinished(void* tCaller) {
	(void)tCaller;
	setNewScreen(&DreamTitleScreen);
}

static void fadeOut(void* tCaller) {
	(void)tCaller;
	addFadeOut(30, fadeOutFinished, NULL);
}

static void startFadeIn(void* tCaller) {
	double* dst = tCaller;

	tweenDouble(dst, 0, 1, quadraticTweeningFunction, 60, NULL, NULL);
}

static void loadWarningScreen() {
	gData.mWarningTexture = loadTexture("assets/main/fight/warning/WARNING.pkg");
	gData.mWarningID = playOneFrameAnimationLoop(makePosition(188, 40, 1), &gData.mWarningTexture);
	gData.mWarningTrans = 0;
	setAnimationTransparency(gData.mWarningID, gData.mWarningTrans);

	gData.mThisIsTexture = loadTexture("assets/main/fight/warning/THISIS.pkg");
	gData.mThisIsID = playOneFrameAnimationLoop(makePosition(59, 193, 1), &gData.mThisIsTexture);
	gData.mThisIsTrans = 0;
	setAnimationTransparency(gData.mThisIsID, gData.mThisIsTrans);

	gData.mExplanationTexture = loadTexture("assets/main/fight/warning/EXPLANATION.pkg");
	gData.mExplanationID = playOneFrameAnimationLoop(makePosition(0, 301, 1), &gData.mExplanationTexture);
	gData.mExplanationTrans = 0;
	setAnimationTransparency(gData.mExplanationID, gData.mExplanationTrans);

	addTimerCB(10, startFadeIn, &gData.mWarningTrans);
	addTimerCB(130, startFadeIn, &gData.mThisIsTrans);
	addTimerCB(370, startFadeIn, &gData.mExplanationTrans);
	addTimerCB(750, fadeOut, NULL);

}


static void updateWarningScreen() {
	setAnimationTransparency(gData.mWarningID, gData.mWarningTrans);
	setAnimationTransparency(gData.mThisIsID, gData.mThisIsTrans);
	setAnimationTransparency(gData.mExplanationID, gData.mExplanationTrans);

	if (hasPressedAbortFlank()) {
		stopDreamFight16();
	}

	if (hasPressedStartFlank()) {
		fadeOut(NULL);
	}
}

Screen DreamWarningScreen = {
	.mLoad = loadWarningScreen,
	.mUpdate = updateWarningScreen,
};
