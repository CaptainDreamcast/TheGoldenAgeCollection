#include "mirk_invisiblemirklingslevel.h"

#include <tari/math.h>
#include <tari/animation.h>
#include <tari/system.h>
#include <tari/animation.h>

#include "../standardroute/mirk_standard.h"
#include "../../mirk_mirkling.h"
#include "../../mirk_mirklinghandler.h"

static char gFunnyTexts[][1024] = {
	"Heads up: The Mirklings in this level are invisible. No, they really are, keep bombing the air and you'll see."
};

static void setRandomFunnyText() {
	int amount = (sizeof gFunnyTexts) / sizeof(gFunnyTexts[0]);
	int i = randfromInteger(0, amount - 1);
	setMirkStandardFunnyText(gFunnyTexts[i]);
}


static void loadInvisibleMirklingsLevel() {
	setMirkStandardWaveText("Round 19");
	setRandomFunnyText();
	setMirkStandardLevelMirklingAmount(20000);
	setMirkMirklingSpeed(1, 2);
	setMirkMirklingsGeneratedPerFrame(10);
	loadMirkStandard();
	setMirkMirklingsInvisible();
}

static void unloadInvisibleMirklingsLevel() {
	setMirkMirklingsVisible();
}




MirkLevel MirkInvisibleMirklingsLevel = {

	.mLoadLevel = loadInvisibleMirklingsLevel,
	.mUpdateLevel = updateMirkStandard,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
	.mUnloadLevel = unloadInvisibleMirklingsLevel
};
