#include "mirk_pukeroute.h"

#include <tari/wrapper.h>

#include "../mirk_levelhandler.h"
#include "../mirk_endingscreen.h"

#include "standardroute/mirk_trailerroute.h"
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
#include "pukeroute/mirk_rotatelevel.h"
#include "pukeroute/mirk_shakelevel.h"
#include "pukeroute/mirk_limitedsightlevel.h"
#include "pukeroute/mirk_nosightlevel.h"
#include "pukeroute/mirk_discolevel.h"
#include "pukeroute/mirk_invisiblemirklingslevel.h"
#include "pukeroute/mirk_textblockingviewlevel.h"
#include "pukeroute/mirk_playforyoulevel.h"
#include "pukeroute/mirk_simulationbreakdown1.h"
#include "pukeroute/mirk_simulationbreakdown2.h"
#include "pukeroute/mirk_simulationbreakdown3.h"
#include "pukeroute/mirk_simulationbreakdown4.h"
#include "pukeroute/mirk_simulationbreakdown5.h"
#include "pukeroute/mirk_simulationbreakdown6.h"
#include "pukeroute/mirk_simulationbreakdown7.h"
#include "pukeroute/mirk_simulationbreakdown8.h"
#include "pukeroute/mirk_simulationbreakdown9.h"
#include "pukeroute/mirk_simulationbreakdown10.h"
#include "pukeroute/mirk_simulationbreakdown11.h"
#include "pukeroute/mirk_willkillyourparentslevel.h"
#include "pukeroute/mirk_maybekilledyourparentslevel.h"
#include "pukeroute/mirk_discotatelevel.h"

static struct {
	int mCurrentLevel;
	int mAmount;

	int mIsFinished;
	int mIsContinuing;

} gData;


static MirkLevel* gLevels[] = {
	&MirkStandardLevel1,
	&MirkStandardLevel2,
	&MirkStandardLevel3,
	&MirkStandardLevel4,
	&MirkStandardLevel5,
	&MirkStandardLevel6,
	&MirkLimitedSightLevel,
	&MirkNoSightLevel,
	&MirkStandardLevel7,
	&MirkShakeLevel,						// 10

	&MirkRotateLevel,
	&MirkStandardLevel8,
	&MirkWillKillYourParentsLevel,
	&MirkMaybeKilledYourParentsLevel,		// 15
	&MirkDiscoLevel,
	&MirkDiscotateLevel,
	&MirkStandardLevel9,
	&MirkInvisibleMirklingsLevel,
	&MirkStandardLevel10,					//20
	
	&MirkTextBlockingViewLevel,
	&MirkPlayForYouLevel,
	&MirkSimulationBreakdown1,
	&MirkSimulationBreakdown2,
	&MirkSimulationBreakdown3,			//25

	&MirkSimulationBreakdown4,
	&MirkSimulationBreakdown5,
	&MirkSimulationBreakdown6,
	&MirkSimulationBreakdown7,
	&MirkSimulationBreakdown8, //30
	&MirkSimulationBreakdown9,
	&MirkSimulationBreakdown10,
	&MirkSimulationBreakdown11,
};

static void loadPukeRoute() {
	gData.mAmount = (sizeof gLevels) / sizeof(MirkLevel*);

	if (!gData.mIsContinuing) {
		gData.mCurrentLevel = 0;
	}
	gData.mIsContinuing = 0;

	gLevels[gData.mCurrentLevel]->mLoadLevel();
}

static void resetPukeRoute() {
	gData.mIsFinished = 0;
}

static void loadNextLevelInRoute() {
	if (gLevels[gData.mCurrentLevel]->mUnloadLevel) {
		gLevels[gData.mCurrentLevel]->mUnloadLevel();
	}
	gData.mCurrentLevel++;

	if (gData.mCurrentLevel >= gData.mAmount) {
		setNewScreen(&MirkEndingScreen);
		return;
	}

	gLevels[gData.mCurrentLevel]->mLoadLevel();
}

static void updatePukeRoute() {
	gLevels[gData.mCurrentLevel]->mUpdateLevel();

	if (gLevels[gData.mCurrentLevel]->mHasWon()) {
		loadNextLevelInRoute();	
	}
}

static int hasLostPukeRoute() {
	return gLevels[gData.mCurrentLevel]->mHasLost();
}

static int canPukeRouteBePlayed() {
	return !gData.mIsFinished;
}

static void setPukeRouteToContinue() {
	gData.mIsContinuing = 1;
}

MirkRoute MirkPukeRoute = {
	.mLoadRoute = loadPukeRoute,
	.mReset = resetPukeRoute,
	.mUpdate = updatePukeRoute,
	.mHasLost = hasLostPukeRoute,
	.mCanBePlayed = canPukeRouteBePlayed,
	.mSetToContinue = setPukeRouteToContinue,
};
