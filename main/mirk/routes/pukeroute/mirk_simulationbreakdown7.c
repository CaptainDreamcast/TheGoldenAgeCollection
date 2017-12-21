#include "mirk_simulationbreakdown7.h"

#include <tari/math.h>
#include <tari/texthandler.h>

#include "../../mirk_noise.h"
#include "../../mirk_mirklinghandler.h"
#include "../../mirk_stage.h"
#include "../../mirk_preciouspeople.h"
#include "../../mirk_particles.h"
#include "../../mirk_mirkling.h"

#include "../standardroute/mirk_standard.h"

static void loadSimulationBreakdown7() {
	setMirkStandardWaveText("Wave 46");
	
	setMirkStandardFunnyText("Mirklings will proceed as originally scheduled now. Just blast away, carefully.");
	setMirkMirklingSpeed(3, 4);

	setMirkMirklingsGeneratedPerFrame(10);
	setMirkStandardLevelMirklingAmount(8000);
	loadMirkStandard();
	setMirkGameUnreal();
}

MirkLevel MirkSimulationBreakdown7 = {
	.mLoadLevel = loadSimulationBreakdown7,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
};
