#include "mirk_trailerroute.h"

#include <prism/math.h>

#include "mirk_standard.h"

static char gFunnyTexts[][1024] = {
	"Yeah, you hated La Morte di un Cane's trailer, so I'll do the opposite with Mirklings. Pure Gameplay. Suffer.",
};


static void setRandomFunnyText() {
	int amount = (sizeof gFunnyTexts) / sizeof(gFunnyTexts[0]);
	int i = randfromInteger(0, amount - 1);
	setMirkStandardFunnyText(gFunnyTexts[i]);
}

static void loadTrailerRoute() {
	setMirkStandardWaveText("Super trailer wave");
	setRandomFunnyText();
	setMirkStandardLevelMirklingAmount(15000000);
	loadMirkStandard();
}

MirkLevel MirkTrailerRoute = {

	.mLoadLevel = loadTrailerRoute,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard
};
