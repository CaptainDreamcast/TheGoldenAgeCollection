#include "mirk_standard9.h"

#include <prism/math.h>

#include "mirk_standard.h"
#include "../../mirk_preciouspeople.h"
#include "../../mirk_mirklinghandler.h"

static char gFunnyTexts[][1024] = {
	"Mirklings are often used for war because they're so fun to blow up. It's a cruel Mirk-world.",
};

static char gFailText[1024] = "You must be sweating right now! Another joke level and you're dead!";

static void setRandomFunnyText() {
	if (getMirkPreciousPeopleAmount() < 2) {
		setMirkStandardFunnyText(gFailText);
		return;
	}

	int amount = (sizeof gFunnyTexts) / sizeof(gFunnyTexts[0]);
	int i = randfromInteger(0, amount - 1);
	setMirkStandardFunnyText(gFunnyTexts[i]);
}


static void loadStandard9() {
	setMirkStandardWaveText("Wave 18");
	setRandomFunnyText();
	setMirkMirklingSpeed(8, 10);
	setMirkMirklingsGeneratedPerFrame(50);
	setMirkStandardLevelMirklingAmount(100000);
	loadMirkStandard();
}

MirkLevel MirkStandardLevel9 = {

	.mLoadLevel = loadStandard9,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard
};
