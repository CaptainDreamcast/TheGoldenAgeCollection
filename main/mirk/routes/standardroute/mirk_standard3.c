#include "mirk_standard3.h"

#include <prism/math.h>

#include "mirk_standard.h"
#include "../../mirk_preciouspeople.h"
#include "../../mirk_mirklinghandler.h"

static char gFunnyTexts[][1024] = {
	"Mirklings are very shy. Scholars suggest it may be because they are prone to being blown to bits.",
};

static char gFailText[1024] = "I- it's okay, you can still do it. Sure. You can do it. Believe in me. You can do it. Stop looking at me like that.";

static void setRandomFunnyText() {
	if (getMirkPreciousPeopleAmount() < 4) {
		setMirkStandardFunnyText(gFailText);
		return;
	}

	int amount = (sizeof gFunnyTexts) / sizeof(gFunnyTexts[0]);
	int i = randfromInteger(0, amount - 1);
	setMirkStandardFunnyText(gFunnyTexts[i]);
}


static void loadStandard3() {
	setMirkStandardWaveText("Wave 3");
	setRandomFunnyText();
	setMirkMirklingSpeed(100, 200);
	setMirkMirklingsGeneratedPerFrame(1000);
	setMirkStandardLevelMirklingAmount(1000);
	loadMirkStandard();
}

MirkLevel MirkStandardLevel3 = {

	.mLoadLevel = loadStandard3,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard
};
