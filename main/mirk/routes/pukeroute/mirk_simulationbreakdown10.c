#include "mirk_simulationbreakdown10.h"

#include <tari/math.h>
#include <tari/texthandler.h>

#include "../../mirk_noise.h"
#include "../../mirk_mirklinghandler.h"
#include "../../mirk_stage.h"
#include "../../mirk_preciouspeople.h"
#include "../../mirk_particles.h"
#include "../../mirk_mirkling.h"

#include "../standardroute/mirk_standard.h"

static void loadSimulationBreakdown10() {
	setMirkStandardWaveText("...");
	
	setMirkStandardFunnyText("Sorry, but I really want my BAFTA, and adding a cheap emotional appeal to your game is the easiest way.");
	setMirkMirklingSpeed(1, 4);

	setMirkMirklingsGeneratedPerFrame(5);
	setMirkStandardLevelMirklingAmount(5000);
	loadMirkStandard();
}

MirkLevel MirkSimulationBreakdown10 = {
	.mLoadLevel = loadSimulationBreakdown10,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
};
