#include "mirk_rotatelevel.h"

#include <prism/math.h>
#include <prism/animation.h>
#include <prism/system.h>

#include "../standardroute/mirk_standard.h"
#include "../../mirk_mirklinghandler.h"
#include "../../mirk_player.h"
#include "../../mirk_mirkling.h"

static char gFunnyTexts[][1024] = {
	"You thought screen shake was bad? Check this out:",
};


static struct {
	double mAngle;
	double mAngleDelta;
	int mAngleDir;
} gData;

static void setRandomFunnyText() {
	int amount = (sizeof gFunnyTexts) / sizeof(gFunnyTexts[0]);
	int i = randfromInteger(0, amount - 1);
	setMirkStandardFunnyText(gFunnyTexts[i]);


}


static void loadRotateLevel() {
	setMirkStandardWaveText("Wave 11");
	setRandomFunnyText();
	setMirkStandardLevelMirklingAmount(25000);
	setMirkMirklingsGeneratedPerFrame(20);
	setMirkMirklingSpeed(4, 6);
	setMirkMirklingSpawnY(-320);
	loadMirkStandard();

	gData.mAngle = 0;
	gData.mAngleDelta = 0;
	gData.mAngleDir = 1;
}

static void unloadRotateLevel() {
	setAnimationHandlerScreenRotationZ(0, makePosition(0,0,0));
	setMirkPlayerShotRotation(0, makePosition(0,0,0));
	setMirkMirklingSpawnY(-20);
}

static void updateRotation() {
	ScreenSize sz = getScreenSize();
	Position center = makePosition(sz.x / 2, sz.y / 2, 0);
	setAnimationHandlerScreenRotationZ(gData.mAngle, center);
	setMirkPlayerShotRotation(gData.mAngle, center);
	gData.mAngle += gData.mAngleDelta;

	gData.mAngleDelta += randfrom(0.001, 0.002)*gData.mAngleDir;
	if (gData.mAngleDelta > 0.1 && gData.mAngleDir == 1) gData.mAngleDir *= -1;
	else if (gData.mAngleDelta < -0.1 && gData.mAngleDir == -1) gData.mAngleDir *= -1;
	else if (randfrom(0, 100) < 2) gData.mAngleDir *= -1;

	
}

static void updateRotateLevel() {
	updateRotation();
	updateMirkStandard();
}


MirkLevel MirkRotateLevel = {

	.mLoadLevel = loadRotateLevel,
	.mUpdateLevel = updateRotateLevel,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
	.mUnloadLevel = unloadRotateLevel
};
