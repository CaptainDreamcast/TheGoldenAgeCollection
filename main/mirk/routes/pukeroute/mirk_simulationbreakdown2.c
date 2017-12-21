#include "mirk_simulationbreakdown2.h"

#include <tari/math.h>

#include "../../mirk_noise.h"
#include "../../mirk_mirklinghandler.h"

#include "../standardroute/mirk_standard.h"

static struct {
	Duration mNow;
	Duration mDuration;

} gData;

static void setNewScreenNoise() {
	addMirkScreenNoise(randfromInteger(7, 12));
	gData.mNow = 0;
	gData.mDuration = randfromInteger(15, 30);
}

static void loadSimulationBreakdown2() {
	setMirkStandardWaveText("Wave wave eavesa eave");
	setMirkStandardFunnyText("Hey, this might make for an interesting new gimmick. When life gives you lemons, you know?");
	setMirkMirklingSpeed(3, 4);
	setMirkMirklingsGeneratedPerFrame(20);
	setMirkStandardLevelMirklingAmount(15000);
	loadMirkStandard();
	setNewScreenNoise();
}


static void updateSimulationBreakdown2() {
	if (handleDurationAndCheckIfOver(&gData.mNow, gData.mDuration)) {
		setNewScreenNoise();
	}

	updateMirkStandard();
}

MirkLevel MirkSimulationBreakdown2 = {
	.mLoadLevel = loadSimulationBreakdown2,
	.mUpdateLevel = updateSimulationBreakdown2,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
};
