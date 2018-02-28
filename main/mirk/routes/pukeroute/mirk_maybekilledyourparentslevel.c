#include "mirk_maybekilledyourparentslevel.h"

#include <stdio.h>

#include <prism/math.h>
#include <prism/animation.h>
#include <prism/system.h>
#include <prism/animation.h>
#include <prism/physicshandler.h>
#include <prism/texthandler.h>

#include "../standardroute/mirk_standard.h"
#include "mirk_willkillyourparentslevel.h"
#include "../../mirk_mirkling.h"
#include "../../mirk_mirklinghandler.h"

static void loadPlayForYouLevel() {
	setMirkStandardWaveText("Wave 14");
	int missAmount = getMirkPlayerParentKillMissedMirklingAmount();
	char funnyText[1024];
	if (missAmount) {
		sprintf(funnyText, "You missed %d Mirklings. Hope you told your parents you loved them. Mirklings is serious business, don't let the MS Paint art and edgy humour fool you.", missAmount);
	}
	else {
		sprintf(funnyText, "Woah, you saved your parents. But if you REALLY loved your parents, you would play Mirklings to the end, just saying.");
	}
	setMirkStandardFunnyText(funnyText);
	setMirkMirklingSpeed(2, 4);
	setMirkMirklingsGeneratedPerFrame(10);
	setMirkStandardLevelMirklingAmount(20000);
	loadMirkStandard();
}

MirkLevel MirkMaybeKilledYourParentsLevel = {

	.mLoadLevel = loadPlayForYouLevel,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
};
