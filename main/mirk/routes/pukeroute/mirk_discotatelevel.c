#include "mirk_limitedsightlevel.h"

#include <tari/math.h>
#include <tari/animation.h>
#include <tari/system.h>
#include <tari/animation.h>

#include "../standardroute/mirk_standard.h"

#include "../../mirk_mirkling.h"
#include "../../mirk_player.h"

static char gFunnyTexts[][1024] = {
	"Ah ah ah stayin' alive, stayin alive. Ah ah ah stayin' aliiiiiiiiiiiiiiiive... you know the rest."
};

static struct {
	Duration mNow;
	Duration mDuration;
	int mCurrentTint;

	double mAngle;
	double mAngleDelta;
	int mAngleDir;
} gData;

#define SCREEN_TINT_AMOUNT 4

static double gScreenTints[SCREEN_TINT_AMOUNT][3] = {
	{0.5,0.5,1},
	{1, 0.5,0.5},
	{0.5,1,0.5},
	{1, 0.5, 1},
};

static void setNextScreenTint() {
	int i = gData.mCurrentTint;
	while (i == gData.mCurrentTint) {
		i = randfromInteger(0, SCREEN_TINT_AMOUNT - 1);
	}
	gData.mCurrentTint = i;
}

static void setRandomFunnyText() {
	int amount = (sizeof gFunnyTexts) / sizeof(gFunnyTexts[0]);
	int i = randfromInteger(0, amount - 1);
	setMirkStandardFunnyText(gFunnyTexts[i]);
}


static void loadDiscotateLevel() {
	setMirkStandardWaveText("Wave 16");
	setRandomFunnyText();
	setMirkMirklingSpawnY(-320);
	setMirkStandardLevelMirklingAmount(20000);
	loadMirkStandard();
	gData.mNow = 0;
	gData.mDuration = 20;
	setNextScreenTint();

	gData.mAngle = 0;
	gData.mAngleDelta = 0;
	gData.mAngleDir = 1;
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

static void updateDisco() {
	if (handleDurationAndCheckIfOver(&gData.mNow, gData.mDuration)) {
		setNextScreenTint();
		gData.mNow = 0;
	}

	setAnimationHandlerScreenTint(gScreenTints[gData.mCurrentTint][0], gScreenTints[gData.mCurrentTint][1], gScreenTints[gData.mCurrentTint][2]);

}

static void updateDiscotateLevel() {
	updateRotation();
	updateDisco();
	
	updateMirkStandard();
}

static void unloadDiscotateLevel() {
	resetAnimationHandlerScreenTint();

	setAnimationHandlerScreenRotationZ(0, makePosition(0, 0, 0));
	setMirkPlayerShotRotation(0, makePosition(0, 0, 0));
	setMirkMirklingSpawnY(-20);
}

MirkLevel MirkDiscotateLevel = {

	.mLoadLevel = loadDiscotateLevel,
	.mUpdateLevel = updateDiscotateLevel,
	.mHasLost = hasMirkLostStandard,
	.mHasWon = hasMirkWonStandard,
	.mUnloadLevel = unloadDiscotateLevel
};
