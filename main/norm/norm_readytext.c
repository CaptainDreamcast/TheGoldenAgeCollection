#include "norm_readytext.h"

#include <prism/animation.h>
#include <prism/timer.h>
#include <prism/tweening.h>
#include <prism/soundeffect.h>

#include "norm_enemygenerator.h"

static struct {
	TextureData mReadyTexture;
	int mReadyAnimationID;

	TextureData mGoTexture;
	int mGoAnimationID;

	int mReadySFX;
	int mGoSFX;

	double mTweenedFactor;

	int mIsReadyTextActive;
	int mIsGoTextActive;
} gData;

static double tweenGoText(double t) {
	(void)t;
	return 1;
}

static void goTextOver(void* tCaller) {
	(void)tCaller;
	gData.mIsGoTextActive = 0;
}

static void loadGoText(void* tData) {
	(void)tData;

	gData.mGoTexture = loadTexture("assets/main/norm/sprites/GO.pkg");
	gData.mGoSFX = loadSoundEffect("assets/main/norm/music/GO.wav");

	gData.mGoAnimationID = playOneFrameAnimationLoop(makePosition(200, 250, 7), &gData.mGoTexture);
	gData.mIsGoTextActive = 1;

	playSoundEffect(gData.mGoSFX);

	tweenDouble(&gData.mTweenedFactor, 1.0, 2.5, tweenGoText, 30, goTextOver, NULL);
}

static void unloadGoText(void* tData) {
	(void)tData;

	removeHandledAnimation(gData.mGoAnimationID);
	setNormEnemyGeneratorActive();
}

static void updateGoText(void* tData) {
	(void)tData;

	setAnimationScale(gData.mGoAnimationID, makePosition(gData.mTweenedFactor, gData.mTweenedFactor, 1), makePosition(128, 128, 0));
}

static int isGoTextActive(void* tData) {
	(void)tData;
	return gData.mIsGoTextActive;
}

ActorBlueprint NormGoBP = {
	.mLoad = loadGoText,
	.mUnload = unloadGoText,
	.mUpdate = updateGoText,
	.mIsActive = isGoTextActive,
};


static void readyTextOver(void* tCaller) {
	(void)tCaller;
	gData.mIsReadyTextActive = 0;
}

static double tweenReadyText(double t) {
	if (t < 0.5) return quadraticTweeningFunction(1-t);

	return quadraticTweeningFunction(t);
}

static void loadReadyTextBP(void* tData) {
	(void)tData;

	gData.mReadyTexture = loadTexture("assets/main/norm/sprites/READY.pkg");


	gData.mReadySFX = loadSoundEffect("assets/main/norm/music/READY.wav");
	

	gData.mReadyAnimationID = playOneFrameAnimationLoop(makePosition(200, 250, 7), &gData.mReadyTexture);
	gData.mIsReadyTextActive = 1;

	playSoundEffect(gData.mReadySFX);

	tweenDouble(&gData.mTweenedFactor, 0.1, 2.5, tweenReadyText, 50, readyTextOver, NULL);
}

static void unloadReadyTextBP(void*tData) {
	(void)tData;
	removeHandledAnimation(gData.mReadyAnimationID);

	instantiateActor(NormGoBP);
}

static void updateReadyTextBP(void* tData) {
	(void)tData;

	setAnimationScale(gData.mReadyAnimationID, makePosition(gData.mTweenedFactor, gData.mTweenedFactor, 1), makePosition(128, 128, 0));
}

static int isReadyTextActive(void* tData) {
	(void)tData;
	return gData.mIsReadyTextActive;
}

ActorBlueprint NormReadyTextBP = {
	.mLoad = loadReadyTextBP,
	.mUnload = unloadReadyTextBP,
	.mUpdate = updateReadyTextBP,
	.mIsActive = isReadyTextActive,
};