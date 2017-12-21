#include "mirk_standard1.h"

#include <tari/math.h>

#include "mirk_standard.h"

static char gFunnyTexts[][1024] = {
	"Welcome to Mirklings, first game to kill a man through repetitive gameplay.",
};


static void setRandomFunnyText() {
	int amount = (sizeof gFunnyTexts) / sizeof(gFunnyTexts[0]);
	int i = randfromInteger(0, amount - 1);
	setMirkStandardFunnyText(gFunnyTexts[i]);
}


static void loadStandard1() {
	setMirkStandardWaveText("Wave 1");
	setRandomFunnyText();
	setMirkStandardLevelMirklingAmount(15000);
	loadMirkStandard();
}

MirkLevel MirkStandardLevel1 = {

	.mLoadLevel = loadStandard1,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard
};
