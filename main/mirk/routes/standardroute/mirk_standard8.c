#include "mirk_standard8.h"

#include <prism/math.h>

#include "mirk_standard.h"
#include "../../mirk_preciouspeople.h"
#include "../../mirk_mirklinghandler.h"

static char gFunnyTexts[][1024] = {
	"Have a little break. The last level was probably hard on your stomach. You earned this."
};

static char gFailText[1024] = "On the upside, if you can make it through this with one house, you'll be a legend.";

static void setRandomFunnyText() {
	if (getMirkPreciousPeopleAmount() < 2) {
		setMirkStandardFunnyText(gFailText);
		return;
	}

	int amount = (sizeof gFunnyTexts) / sizeof(gFunnyTexts[0]);
	int i = randfromInteger(0, amount - 1);
	setMirkStandardFunnyText(gFunnyTexts[i]);
}


static void loadStandard8() {
	setMirkStandardWaveText("Wave 12");
	setRandomFunnyText();
	setMirkMirklingSpeed(0.5, 1);
	setMirkMirklingsGeneratedPerFrame(5);
	setMirkStandardLevelMirklingAmount(10000);
	loadMirkStandard();
}

MirkLevel MirkStandardLevel8 = {

	.mLoadLevel = loadStandard8,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard
};
