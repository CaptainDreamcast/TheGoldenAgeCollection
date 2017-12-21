#include "mirk_standard5.h"

#include <tari/math.h>

#include "mirk_standard.h"
#include "../../mirk_preciouspeople.h"
#include "../../mirk_mirklinghandler.h"

static char gFunnyTexts[][1024] = {
	"The cramp in your hand means you're alive.",
};

static char gFailText[1024] = "Don't worry about the broken houses my dude, next levels are gonna be chill. No, really. Then again, maybe not, I don't know man, I'm working on Wave 5 now how should I know.";

static void setRandomFunnyText() {
	if (getMirkPreciousPeopleAmount() < 3) {
		setMirkStandardFunnyText(gFailText);
		return;
	}

	int amount = (sizeof gFunnyTexts) / sizeof(gFunnyTexts[0]);
	int i = randfromInteger(0, amount - 1);
	setMirkStandardFunnyText(gFunnyTexts[i]);
}


static void loadStandard5() {
	setMirkStandardWaveText("Wave 5");
	setRandomFunnyText();
	setMirkMirklingSpeed(1, 2);
	setMirkMirklingsGeneratedPerFrame(10);
	setMirkStandardLevelMirklingAmount(10000);
	loadMirkStandard();
}

MirkLevel MirkStandardLevel5 = {

	.mLoadLevel = loadStandard5,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard
};
