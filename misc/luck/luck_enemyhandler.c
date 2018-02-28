#include "luck_enemyhandler.h"

#include <prism/timer.h>

#include "luck_door.h"
#include "luck_pirate.h"

static struct {
	int mDoor;

	int mIsGeneratingPirates;

	Duration mGenerateNow;
	Duration mGenerateDuration;
} gData;

static void letPiratesIn(void* tCaller) {
	(void)tCaller;
	removeActor(gData.mDoor);
	gData.mIsGeneratingPirates = 1;
}

static void loadEnemyHandler(void* tData) {
	(void)tData;
	gData.mDoor = instantiateActor(LuckDoorBP);
	gData.mIsGeneratingPirates = 0;
	gData.mGenerateDuration = 31;
	gData.mGenerateNow = gData.mGenerateDuration;
	loadLuckPirates();

	addTimerCB(600, letPiratesIn, NULL);
}

static void updateEnemyHandler(void* tData) {
	(void)tData;

	if (!gData.mIsGeneratingPirates) return;

	if (handleDurationAndCheckIfOver(&gData.mGenerateNow, gData.mGenerateDuration)) {
		addNewLuckPirate();
		gData.mGenerateNow = 0;
	}

}

ActorBlueprint LuckEnemyHandlerBP = {
	.mLoad = loadEnemyHandler,
	.mUpdate = updateEnemyHandler,
};