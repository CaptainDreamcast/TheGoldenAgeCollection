#include "mirk_simulationbreakdown7.h"

#include <prism/math.h>
#include <prism/texthandler.h>

#include "../../mirk_noise.h"
#include "../../mirk_mirklinghandler.h"
#include "../../mirk_stage.h"
#include "../../mirk_preciouspeople.h"
#include "../../mirk_particles.h"
#include "../../mirk_mirkling.h"

#include "../standardroute/mirk_standard.h"

static void loadSimulationBreakdown9() {
	setMirkStandardWaveText("...");
	
	setMirkStandardFunnyText("You were controlling a drone shooting down people in Afghanistan, as you can tell by these hyperrealistic video recordings.");
	setMirkMirklingSpeed(1, 2);

	setMirkMirklingsGeneratedPerFrame(10);
	setMirkStandardLevelMirklingAmount(5000);
	loadMirkStandard();
}

MirkLevel MirkSimulationBreakdown9 = {
	.mLoadLevel = loadSimulationBreakdown9,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
};
