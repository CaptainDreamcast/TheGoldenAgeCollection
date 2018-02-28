#include "mirk_simulationbreakdown6.h"

#include <prism/math.h>
#include <prism/texthandler.h>

#include "../../mirk_noise.h"
#include "../../mirk_mirklinghandler.h"
#include "../../mirk_stage.h"
#include "../../mirk_preciouspeople.h"
#include "../../mirk_particles.h"
#include "../../mirk_mirkling.h"
#include "../../mirk_deathcount.h"

#include "../standardroute/mirk_standard.h"

static struct {
	Duration mNow;
	Duration mDuration;

	TextureData mTitleTexture;
	int mTitle;
	int mTitleText;

	TextureData mTransparentBGTexture;
	int mTransparentBG;
} gData;

static void setNewScreenNoise() {
	addMirkScreenNoise(randfromInteger(5, 10));
	gData.mNow = 0;
	gData.mDuration = randfromInteger(30, 40);
	if(randfromInteger(0,1)) invertMirkStageReality();
	if (randfromInteger(0, 1)) invertMirkPreciousPeopleReality();
	if (randfromInteger(0, 1)) invertMirkParticleReality();
	if (randfromInteger(0, 1)) invertMirkMirklingsReality();
	if (randfromInteger(0, 1)) invertMirkDeathCountReality();
}

static void showBreakdownTitle(void* tCaller) {
	(void)tCaller;
	gData.mTransparentBGTexture = loadTexture("assets/main/mirk/text/WAVE_TEXT_BG.pkg");
	gData.mTransparentBG = playOneFrameAnimationLoop(makePosition(0, 0, 19), &gData.mTransparentBGTexture);
	setAnimationSize(gData.mTransparentBG, makePosition(640, 480, 1), makePosition(0, 0, 0));

	gData.mTitleTexture = loadTexture("assets/main/mirk/title/TITLE.pkg");
	gData.mTitle = playAnimationLoop(makePosition(0,0,20), &gData.mTitleTexture, createOneFrameAnimation(), makeRectangleFromTexture(gData.mTitleTexture));
	gData.mTitleText = addHandledTextWithInfiniteDurationOnOneLine(makePosition(70, 300, 21), "PRESS START AND SHOOT AT THE SCREEN", 0, COLOR_WHITE, makePosition(15, 15, 1));
}

static void loadSimulationBreakdown6() {
	setMirkStandardWaveText("CONGRATULATIONS");
	
	setMirkStandardFunnyText("YOU WON");
	setMirkMirklingSpeed(3, 4);

	setMirkMirklingsGeneratedPerFrame(10);
	setMirkStandardLevelMirklingAmount(10000);
	loadMirkStandard();
	setMirkStandardFunnyTextPositionAfterLoad(makePosition(250, 240, 12));
	setNewScreenNoise();
	setMirkStandardStoppedShowingWaveScreenCB(showBreakdownTitle, NULL);
}


static void updateSimulationBreakdown6() {
	if (handleDurationAndCheckIfOver(&gData.mNow, gData.mDuration)) {
		setNewScreenNoise();
	}

	updateMirkStandard();
}

static void unloadSimulationBreakdown6() {
	removeHandledText(gData.mTitleText);
	removeHandledAnimation(gData.mTitle);
	unloadTexture(gData.mTitleTexture);
	removeHandledAnimation(gData.mTransparentBG);
	unloadTexture(gData.mTransparentBGTexture);
}

MirkLevel MirkSimulationBreakdown6 = {
	.mLoadLevel = loadSimulationBreakdown6,
	.mUpdateLevel = updateSimulationBreakdown6,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
	.mUnloadLevel = unloadSimulationBreakdown6,
};
