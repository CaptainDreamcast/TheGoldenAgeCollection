#include "mirk_standard7.h"

#include <tari/math.h>

#include "mirk_standard.h"
#include "../../mirk_preciouspeople.h"
#include "../../mirk_mirklinghandler.h"

static char gFunnyTexts[][1024] = {
	"What a relief, huh? You can finally enjoy Mirkling's HD graphics again."
};

static void setRandomFunnyText() {
	int amount = (sizeof gFunnyTexts) / sizeof(gFunnyTexts[0]);
	int i = randfromInteger(0, amount - 1);
	setMirkStandardFunnyText(gFunnyTexts[i]);
}


static void loadStandard7() {
	setMirkStandardWaveText("Wave 9");
	setRandomFunnyText();
	setMirkMirklingSpeed(4, 8);
	setMirkMirklingsGeneratedPerFrame(40);
	setMirkStandardLevelMirklingAmount(50000);
	loadMirkStandard();
}

MirkLevel MirkStandardLevel7 = {

	.mLoadLevel = loadStandard7,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard
};
