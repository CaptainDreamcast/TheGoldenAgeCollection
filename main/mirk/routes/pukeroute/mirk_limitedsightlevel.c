#include "mirk_limitedsightlevel.h"

#include <prism/math.h>
#include <prism/animation.h>
#include <prism/system.h>
#include <prism/animation.h>

#include "../standardroute/mirk_standard.h"

static char gFunnyTexts[][1024] = {
	"Hohoho, no more properness! Try being blind for a change."
};

static struct {
	TextureData mLimitedSightTexture;
	int mLimitedSight;

} gData;

static void setRandomFunnyText() {
	int amount = (sizeof gFunnyTexts) / sizeof(gFunnyTexts[0]);
	int i = randfromInteger(0, amount - 1);
	setMirkStandardFunnyText(gFunnyTexts[i]);


}


static void loadLimitedSightLevel() {
	setMirkStandardWaveText("Wave 7");
	setRandomFunnyText();
	setMirkStandardLevelMirklingAmount(15000);
	loadMirkStandard();
	gData.mLimitedSightTexture = loadTexture("assets/main/mirk/effects/LIMITEDSIGHT.pkg");
	gData.mLimitedSight = playAnimationLoop(makePosition(0, 0, 9), &gData.mLimitedSightTexture, createOneFrameAnimation(), makeRectangleFromTexture(gData.mLimitedSightTexture));
	setAnimationSize(gData.mLimitedSight, makePosition(640,480,1), makePosition(0,0,0));
}

static void unloadLimitedSightLevel() {
	removeHandledAnimation(gData.mLimitedSight);
	unloadTexture(gData.mLimitedSightTexture);
}




MirkLevel MirkLimitedSightLevel = {

	.mLoadLevel = loadLimitedSightLevel,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
	.mUnloadLevel = unloadLimitedSightLevel
};
