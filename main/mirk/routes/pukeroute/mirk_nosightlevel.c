#include "mirk_nosightlevel.h"

#include <tari/math.h>
#include <tari/animation.h>
#include <tari/system.h>
#include <tari/animation.h>

#include "../standardroute/mirk_standard.h"

static char gFunnyTexts[][1024] = {
	"Let's see how far we can push this joke."
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


static void loadNoSightLevel() {
	setMirkStandardWaveText("Wave 8");
	setRandomFunnyText();
	setMirkStandardLevelMirklingAmount(15000);
	loadMirkStandard();
	gData.mLimitedSightTexture = loadTexture("assets/debug/collision_rect.pkg");
	gData.mLimitedSight = playAnimationLoop(makePosition(0, 0, 9), &gData.mLimitedSightTexture, createOneFrameAnimation(), makeRectangleFromTexture(gData.mLimitedSightTexture));
	setAnimationSize(gData.mLimitedSight, makePosition(640,480,1), makePosition(0,0,0));
	setAnimationColor(gData.mLimitedSight, 0,0,0);
}

static void unloadNoSightLevel() {
	removeHandledAnimation(gData.mLimitedSight);
	unloadTexture(gData.mLimitedSightTexture);
}




MirkLevel MirkNoSightLevel = {
	.mLoadLevel = loadNoSightLevel,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
	.mUnloadLevel = unloadNoSightLevel
};
