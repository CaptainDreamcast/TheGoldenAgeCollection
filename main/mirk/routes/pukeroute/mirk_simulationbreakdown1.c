#include "mirk_simulationbreakdown1.h"

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
	gData.mDuration = randfromInteger(15, 60);
}

static void loadSimulationBreakdown1() {
	setMirkStandardWaveText("Wave ?309");
	setMirkStandardFunnyText("Ah, we're having small graphics glitches. Nothing new here, you should be used to this level of quality by now.");
	setMirkMirklingSpeed(1, 2);
	setMirkMirklingsGeneratedPerFrame(10);
	setMirkStandardLevelMirklingAmount(15000);
	loadMirkStandard();
	setNewScreenNoise();
}


static void updateSimulationBreakdown1() {
	if (handleDurationAndCheckIfOver(&gData.mNow, gData.mDuration)) {
		setNewScreenNoise();
	}

	updateMirkStandard();
}

MirkLevel MirkSimulationBreakdown1 = {
	.mLoadLevel = loadSimulationBreakdown1,
	.mUpdateLevel = updateSimulationBreakdown1,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
};
