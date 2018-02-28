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

static void loadSimulationBreakdown8() {
	setMirkStandardWaveText("...");
	
	setMirkStandardFunnyText("You know what? I give up. There were no Mirklings.");
	setMirkMirklingSpeed(3, 4);
	
	setMirkMirklingsGeneratedPerFrame(3);
	setMirkStandardLevelMirklingAmount(2000);
	loadMirkStandard();
	setMirkGameReal();
}

MirkLevel MirkSimulationBreakdown8 = {
	.mLoadLevel = loadSimulationBreakdown8,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
};
