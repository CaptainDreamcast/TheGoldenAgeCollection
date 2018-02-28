#include "mirk_standard2.h"

#include <prism/math.h>

#include "mirk_standard.h"
#include "../../mirk_preciouspeople.h"

static char gFunnyTexts[][1024] = {
	"Boom boom boom, that was fun, eh? You won't close the game anytime soon, right? BY GOD PLEASE DON'T LEAVE",
};

static char gFailText[] = "Look dude, I know \"everybody plays differently\" and shit, but how did you lose a damn house this soon?";

static void setRandomFunnyText() {
	if (getMirkPreciousPeopleAmount() < 5) {
		setMirkStandardFunnyText(gFailText);
		return;
	}

	int amount = (sizeof gFunnyTexts) / sizeof(gFunnyTexts[0]);
	int i = randfromInteger(0, amount - 1);
	setMirkStandardFunnyText(gFunnyTexts[i]);
}


static void loadStandard2() {
	setMirkStandardWaveText("Wave 2");
	setRandomFunnyText();
	setMirkStandardLevelMirklingAmount(10);
	loadMirkStandard();
}

MirkLevel MirkStandardLevel2 = {

	.mLoadLevel = loadStandard2,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard
};
