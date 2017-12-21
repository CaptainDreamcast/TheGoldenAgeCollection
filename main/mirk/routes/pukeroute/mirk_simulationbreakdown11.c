#include "mirk_simulationbreakdown11.h"

#include <tari/math.h>
#include <tari/texthandler.h>

#include "../../mirk_noise.h"
#include "../../mirk_mirklinghandler.h"
#include "../../mirk_stage.h"
#include "../../mirk_preciouspeople.h"
#include "../../mirk_particles.h"
#include "../../mirk_mirkling.h"
#include "../../mirk_endingscreen.h"

#include "../standardroute/mirk_standard.h"


static void lastMirklingHit(void* tCaller, void* tCollisionData) {
	(void)tCaller;
	(void)tCollisionData;
	setMirkBadEnding();
}

static void loadSimulationBreakdown11() {
	setMirkStandardWaveText("...");
	
	setMirkStandardFunnyText("Time for a super-advanced karma choice, so Mirklings can say \"karma system\" on the box.");
	setMirkMirklingSpeed(1, 4);

	setMirkMirklingsGeneratedPerFrame(0);
	setMirkStandardLevelMirklingAmount(1);
	loadMirkStandard();
	int mirkling = addMirkMirklingManual(makePosition(320, -100, 2), makePosition(0, 1, 0), 0.5);
	setMirkMirklingRouteHitCB(mirkling, lastMirklingHit, NULL);
	setMirkGoodEnding();
}

MirkLevel MirkSimulationBreakdown11 = {
	.mLoadLevel = loadSimulationBreakdown11,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
};
