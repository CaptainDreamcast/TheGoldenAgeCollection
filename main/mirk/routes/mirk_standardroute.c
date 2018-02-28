#include "mirk_standardroute.h"

#include "../mirk_levelhandler.h"

#include "standardroute/mirk_standard1.h"
#include "standardroute/mirk_standard2.h"
#include "standardroute/mirk_standard3.h"
#include "standardroute/mirk_standard4.h"
#include "standardroute/mirk_standard5.h"
#include "standardroute/mirk_standard6.h"
#include "standardroute/mirk_standard7.h"
#include "standardroute/mirk_standard8.h"
#include "standardroute/mirk_standard9.h"
#include "standardroute/mirk_standard10.h"


static struct {
	int mCurrentLevel;
	int mAmount;

	int mIsFinished;
	
} gData;

static MirkLevel* gLevels[] = {
	&MirkStandardLevel1,
	&MirkStandardLevel2,
	&MirkStandardLevel3,
	&MirkStandardLevel4,
	&MirkStandardLevel5,
	&MirkStandardLevel6,
	&MirkStandardLevel7,
	&MirkStandardLevel8,
	&MirkStandardLevel9,
	&MirkStandardLevel10
};

static void loadStandardRoute() {
	gData.mAmount = (sizeof gLevels) / sizeof(MirkLevel*);
	gData.mCurrentLevel = 0;

	gLevels[gData.mCurrentLevel]->mLoadLevel();
}

static void resetStandardRoute() {
	gData.mIsFinished = 0;
}

static void loadNextLevelInRoute() {
	gData.mCurrentLevel = (gData.mCurrentLevel + 1) % gData.mAmount;
	gLevels[gData.mCurrentLevel]->mLoadLevel();
}

static void updateStandardRoute() {
	gLevels[gData.mCurrentLevel]->mUpdateLevel();

	if (gLevels[gData.mCurrentLevel]->mHasWon()) {
		loadNextLevelInRoute();	
	}
}

static int hasLostStandardRoute() {
	return gLevels[gData.mCurrentLevel]->mHasLost();
}

static int canStandardRouteBePlayed() {
	return !gData.mIsFinished;
}

MirkRoute MirkStandardRoute = {
	.mLoadRoute = loadStandardRoute,
	.mReset = resetStandardRoute,
	.mUpdate = updateStandardRoute,
	.mHasLost = hasLostStandardRoute,
	.mCanBePlayed = canStandardRouteBePlayed,
};