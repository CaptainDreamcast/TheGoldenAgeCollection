#include "mirk_shakelevel.h"

#include <tari/math.h>
#include <tari/animation.h>
#include <tari/system.h>
#include <tari/stagehandler.h>


#include "../standardroute/mirk_standard.h"
#include "../../mirk_stage.h"
#include "../../mirk_mirkling.h"
#include "../../mirk_mirklinghandler.h"


static char gFunnyTexts[][1024] = {
	"This level is dedicated to Vlambeer.",
};


static void setRandomFunnyText() {
	int amount = (sizeof gFunnyTexts) / sizeof(gFunnyTexts[0]);
	int i = randfromInteger(0, amount - 1);
	setMirkStandardFunnyText(gFunnyTexts[i]);
}


static void loadShakeLevel() {
	setMirkStandardWaveText("Wave 10");
	setRandomFunnyText();
	setMirkMirklingSpeed(2, 4);
	setMirkMirklingsGeneratedPerFrame(10);

	setMirkStandardLevelMirklingAmount(30000);
	loadMirkStandard();
	setMirkMirklingSpawnY(-150);

	int i;
	for (i = 0; i < 7; i++) {
		increaseMirkScreenShake();
	}
}

static void unloadShakeLevel() {
	resetMirkScreenShakeLimit();
	setMirkMirklingSpawnY(-20);
}

static void updateShake() {
	addStageHandlerScreenShake(10);
}

static void updateShakeLevel() {
	updateShake();
	updateMirkStandard();
}


MirkLevel MirkShakeLevel = {

	.mLoadLevel = loadShakeLevel,
	.mUpdateLevel = updateShakeLevel,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
	.mUnloadLevel = unloadShakeLevel
};
