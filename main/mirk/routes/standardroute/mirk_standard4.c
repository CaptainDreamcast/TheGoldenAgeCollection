#include "mirk_standard4.h"

#include <tari/math.h>

#include "mirk_standard.h"
#include "../../mirk_preciouspeople.h"
#include "../../mirk_mirklinghandler.h"

static char gFunnyTexts[][1024] = {
	"WHOA WHOA WHOA, WHAT JUST HAPPENED HERE, WHAT DID YOU DO?"
};

static void setRandomFunnyText() {
	int amount = (sizeof gFunnyTexts) / sizeof(gFunnyTexts[0]);
	int i = randfromInteger(0, amount - 1);
	setMirkStandardFunnyText(gFunnyTexts[i]);
}


static void loadStandard4() {
	setMirkStandardWaveText("Wave 4");
	setRandomFunnyText();
	setMirkMirklingSpeed(5, 6);
	setMirkMirklingsGeneratedPerFrame(10);
	setMirkStandardLevelMirklingAmount(20000); 
	loadMirkStandard();
}

MirkLevel MirkStandardLevel4 = {

	.mLoadLevel = loadStandard4,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard
};
