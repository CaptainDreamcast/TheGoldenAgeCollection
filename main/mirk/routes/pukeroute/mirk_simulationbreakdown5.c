#include "mirk_simulationbreakdown5.h"

#include <tari/math.h>

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

static void loadSimulationBreakdown5() {
	setMirkStandardWaveText("Welcome Massacre");
	
	setMirkStandardFunnyText("I warned you.");
	setMirkMirklingSpeed(3, 4);

	setMirkMirklingsGeneratedPerFrame(10);
	setMirkStandardLevelMirklingAmount(20000);
	loadMirkStandard();
	setMirkStandardFunnyTextPositionAfterLoad(makePosition(200, 240, 12));
	setNewScreenNoise();
}


static void updateSimulationBreakdown5() {
	if (handleDurationAndCheckIfOver(&gData.mNow, gData.mDuration)) {
		setNewScreenNoise();
	}

	updateMirkStandard();
}

MirkLevel MirkSimulationBreakdown5 = {
	.mLoadLevel = loadSimulationBreakdown5,
	.mUpdateLevel = updateSimulationBreakdown5,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
};
