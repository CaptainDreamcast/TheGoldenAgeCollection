#include "mirk_routehandler.h"

#include <prism/math.h>
#include <prism/wrapper.h>

#include "routes/mirk_standardroute.h"
#include "routes/mirk_pukeroute.h"

#include "mirk_levelhandler.h"
#include "mirk_route.h"
#include "mirk_gameoverscreen.h"
#include "mirk_titlescreen.h"

static struct {
	int mCurrentRoute;
	int mAmount;
	int mIsContinuing;
} gData;

static MirkRoute* gRoutes[] = {
	//&MirkStandardRoute,
	&MirkPukeRoute,
};

static void resetAllRoutes() {
	int i;
	for (i = 0; i < gData.mAmount; i++) {
		gRoutes[i]->mReset();
	}
}

void reloadMirkRoute() {
	if (gData.mIsContinuing) {
		gData.mIsContinuing = 0;
		return;
	}
	gData.mAmount = (sizeof gRoutes) / sizeof(MirkRoute*);

	int i;
	for (i = 0; i < gData.mAmount; i++) {
		gData.mCurrentRoute = randfromInteger(0, gData.mAmount - 1);
		if (gRoutes[gData.mCurrentRoute]->mCanBePlayed()) break;
	}

	if (!gRoutes[gData.mCurrentRoute]->mCanBePlayed()) {
		resetAllRoutes();
		reloadMirkRoute();
	}

}

void startNewMirkRoute() {
	gRoutes[gData.mCurrentRoute]->mLoadRoute();
}

void updateMirkRouteHandler() {
	if (gRoutes[gData.mCurrentRoute]->mHasLost()) {
		setNewScreen(&MirkGameOverScreen);
	}

	gRoutes[gData.mCurrentRoute]->mUpdate();
}

void setCurrentMirkRouteToContinue()
{
	gData.mIsContinuing = 1;
	if (gRoutes[gData.mCurrentRoute]->mSetToContinue) {
		gRoutes[gData.mCurrentRoute]->mSetToContinue();
	}
}
