#include "mirk_simulationbreakdown3.h"

#include <tari/math.h>

#include "../../mirk_noise.h"
#include "../../mirk_mirklinghandler.h"
#include "../../mirk_stage.h"
#include "../../mirk_preciouspeople.h"
#include "../../mirk_particles.h"
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
	invertMirkStageReality();
	invertMirkPreciousPeopleReality();
	invertMirkParticleReality();
	invertMirkDeathCountReality();
}

static void loadSimulationBreakdown4() {
	setMirkStandardWaveText("34°10'9.51\"N 73°14'32.78\"E");
	setMirkStandardFunnyText("Going further may not be the best idea. You can tell the game's soon going to start hammering its super-deep leason.");
	setMirkMirklingSpeed(1, 2);

	setMirkMirklingsGeneratedPerFrame(10);
	setMirkStandardLevelMirklingAmount(10000);
	loadMirkStandard();
	setNewScreenNoise();
}


static void updateSimulationBreakdown4() {
	if (handleDurationAndCheckIfOver(&gData.mNow, gData.mDuration)) {
		setNewScreenNoise();
	}

	updateMirkStandard();
}

MirkLevel MirkSimulationBreakdown4 = {
	.mLoadLevel = loadSimulationBreakdown4,
	.mUpdateLevel = updateSimulationBreakdown4,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
};
