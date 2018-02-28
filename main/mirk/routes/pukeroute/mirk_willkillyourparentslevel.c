#include "mirk_maybekilledyourparentslevel.h"

#include <prism/math.h>
#include <prism/animation.h>
#include <prism/system.h>
#include <prism/animation.h>
#include <prism/physicshandler.h>
#include <prism/texthandler.h>

#include "../standardroute/mirk_standard.h"
#include "../../mirk_deathcount.h"
#include "../../mirk_mirkling.h"
#include "../../mirk_mirklinghandler.h"

static struct {
	int mStart;
	int mEnd;
	int mTarget;
} gData;

static void loadWillKillYourParentsLevel() {
	setMirkStandardWaveText("Wave 13");
	setMirkStandardFunnyText("I swear to God if you don't destroy all the Mirklings in this level I will kill your parents and everything you love. I'll do it, I no longer have anything to lose, don't try me.");
	setMirkStandardLevelMirklingAmount(6000);
	setMirkMirklingSpeed(6, 8);
	setMirkMirklingsGeneratedPerFrame(1);
	loadMirkStandard();

	gData.mStart = getMirkDeathCount();
}

static void unloadWillKillYourParentsLevel() {
	gData.mEnd = getMirkDeathCount();
	gData.mTarget = getMirkStandardGeneratedMirklingAmount();
}

int getMirkPlayerParentKillMissedMirklingAmount() {
	return gData.mTarget - (gData.mEnd - gData.mStart);
}

MirkLevel MirkWillKillYourParentsLevel = {

	.mLoadLevel = loadWillKillYourParentsLevel,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
	.mUnloadLevel = unloadWillKillYourParentsLevel,
};
