#include "mirk_playforyoulevel.h"

#include <tari/math.h>
#include <tari/animation.h>
#include <tari/system.h>
#include <tari/animation.h>
#include <tari/physicshandler.h>
#include <tari/texthandler.h>

#include "../standardroute/mirk_standard.h"
#include "../../mirk_player.h"
#include "../../mirk_mirkling.h"
#include "../../mirk_mirklinghandler.h"


static char gFunnyTexts[][1024] = {
	"You suck at this. Your player license is hereby revoked."
};

static struct {
	Duration mNow;
	Duration mDuration;
	int mIsShooting;
} gData;

static void setRandomFunnyText() {
	int amount = (sizeof gFunnyTexts) / sizeof(gFunnyTexts[0]);
	int i = randfromInteger(0, amount - 1);
	setMirkStandardFunnyText(gFunnyTexts[i]);
}

static void startShooting(void* tCaller) {
	(void)tCaller;
	gData.mIsShooting = 1;
}

static void loadPlayForYouLevel() {
	setMirkStandardWaveText("Wave 22");
	setRandomFunnyText();
	
	setMirkMirklingSpeed(1, 2);
	setMirkMirklingsGeneratedPerFrame(10);
	setMirkStandardLevelMirklingAmount(15000);
	loadMirkStandard();

	pauseMirkPlayerShooting();
	setMirkStandardKeepPlayerShotPaused();
	gData.mNow = 0;
	gData.mDuration = 30;
	gData.mIsShooting = 0;
	setMirkStandardStoppedShowingWaveScreenCB(startShooting, NULL);
}

static void shootAtLowestMirkling() {
	Position p = getLowestMirkMirklingPosition();
	if (p.y == 0) {
		p = makePosition(320, 240, 0);
	}

	addMirkPlayerShotManual(p, 200);
}

static void updatePlayForYouLevel() {
	if (gData.mIsShooting && handleDurationAndCheckIfOver(&gData.mNow, gData.mDuration)) {
		gData.mNow = 0;
		shootAtLowestMirkling();
		gData.mDuration = randfromInteger(30, 40);
	}
	updateMirkStandard();
}

static void unloadPlayForYouLevel() {
	unpauseMirkPlayerShooting();
}




MirkLevel MirkPlayForYouLevel = {

	.mLoadLevel = loadPlayForYouLevel,
	.mUpdateLevel = updatePlayForYouLevel,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
	.mUnloadLevel = unloadPlayForYouLevel
};
