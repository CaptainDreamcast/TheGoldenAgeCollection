#include "mirk_standard6.h"

#include <tari/math.h>

#include "mirk_standard.h"
#include "../../mirk_preciouspeople.h"
#include "../../mirk_mirklinghandler.h"

static char gFunnyTexts[][1024] = {
	"Mirklings are closely related to the pufferfish. A pufferfish's natural prey are mollusks.",
};

static char gFailText[1024] = "Your gameplay style is like a car crash in motion, we all know it's not gonna end well, but we can't avert our eyes.";

static void setRandomFunnyText() {
	if (getMirkPreciousPeopleAmount() < 2) {
		setMirkStandardFunnyText(gFailText);
		return;
	}

	int amount = (sizeof gFunnyTexts) / sizeof(gFunnyTexts[0]);
	int i = randfromInteger(0, amount - 1);
	setMirkStandardFunnyText(gFunnyTexts[i]);
}


static void loadStandard6() {
	setMirkStandardWaveText("Wave 6");
	setRandomFunnyText();
	setMirkMirklingSpeed(6, 8);
	setMirkMirklingsGeneratedPerFrame(40);
	setMirkStandardLevelMirklingAmount(40000);
	loadMirkStandard();
}

MirkLevel MirkStandardLevel6 = {

	.mLoadLevel = loadStandard6,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard
};
