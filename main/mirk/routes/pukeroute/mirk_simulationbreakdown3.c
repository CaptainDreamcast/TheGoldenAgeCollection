#include "mirk_simulationbreakdown3.h"

#include <prism/math.h>

#include "../../mirk_noise.h"
#include "../../mirk_mirklinghandler.h"
#include "../../mirk_stage.h"

#include "../standardroute/mirk_standard.h"

static struct {
	Duration mNow;
	Duration mDuration;

} gData;

static void setNewScreenNoise() {
	addMirkScreenNoise(randfromInteger(20, 30));
	gData.mNow = 0;
	gData.mDuration = randfromInteger(60, 120);
	invertMirkStageReality();
}

static void loadSimulationBreakdown3() {
	setMirkStandardWaveText("Wave 1");
	setMirkStandardFunnyText("Oh oh oh oh oh oh oh oh oh oh oh oh oh oh oh oh oh this is bad");
	setMirkMirklingSpeed(1, 2);

	setMirkMirklingsGeneratedPerFrame(10);
	setMirkStandardLevelMirklingAmount(15000);
	loadMirkStandard();
	setNewScreenNoise();
}


static void updateSimulationBreakdown3() {
	if (handleDurationAndCheckIfOver(&gData.mNow, gData.mDuration)) {
		setNewScreenNoise();
	}

	updateMirkStandard();
}

MirkLevel MirkSimulationBreakdown3 = {
	.mLoadLevel = loadSimulationBreakdown3,
	.mUpdateLevel = updateSimulationBreakdown3,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
};
