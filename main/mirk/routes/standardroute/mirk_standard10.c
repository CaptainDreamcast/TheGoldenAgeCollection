#include "mirk_standard10.h"

#include <tari/math.h>
#include <tari/texthandler.h>
#include <tari/timer.h>

#include "mirk_standard.h"
#include "../../mirk_preciouspeople.h"
#include "../../mirk_mirklinghandler.h"
#include "../../mirk_collision.h"
#include "../../mirk_mirkling.h"
#include "../../mirk_soundeffect.h"

static struct {
	int mMirkling;

	int mHellText;
} gData;

static void setFunnyText() {
	if (getMirkPreciousPeopleAmount() < 2) {
		setMirkStandardFunnyText("Remember, you could have prevented this. Protect your house. Protect the people precious to you.");
		return;
	}
	
	setMirkStandardFunnyText("Think about this.");
}

static void unleashHell(void* tCaller) {
	(void)tCaller;
	setMirkMirklingSpeed(8, 10);
	setMirkMirklingsGeneratedPerFrame(80);
}

static void setHell() {
	Position p = makePosition(50, 70, 12);
	gData.mHellText = addHandledTextWithBuildup(p, "YOU'RE NOT GONNA LIKE WHAT HAPPENS NOW", 0, COLOR_WHITE, makePosition(50,50,1), makePosition(0,0,0), makePosition(540, INF, INF), 280, 200);
	setHandledTextSoundEffects(gData.mHellText, getMirkTextSoundEffectCollection());
	addTimerCB(280, unleashHell, NULL);
}

static void specialMirklingHit(void* tCaller, void* tCollisionData) {
	(void)tCaller;
	MirkCollisionData* colData = tCollisionData;

	if (colData->mType == getMirkShotCollisionList()) {
		setHell();
	}
	else {
		setMirkStandardLevelMirklingAmount(0);
	}
}

static void generateSpecialMirkling() {
	Position p = makePosition(320, -70, 3);
	Velocity v = makePosition(0, 1, 0);

	gData.mMirkling = addMirkMirklingManual(p, v, 0.5);
	setMirkMirklingRouteHitCB(gData.mMirkling, specialMirklingHit, NULL);
}

static void loadStandard10() {
	setMirkStandardWaveText("Wave 20");
	setFunnyText();
	setMirkMirklingSpeed(1, 1);
	setMirkMirklingsGeneratedPerFrame(0);
	setMirkStandardLevelMirklingAmount(100000);
	loadMirkStandard();
	setMirkStandardFunnyTextPositionAfterLoad(makePosition(180, 240, 12));

	generateSpecialMirkling();
}


MirkLevel MirkStandardLevel10 = {

	.mLoadLevel = loadStandard10,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard
};
