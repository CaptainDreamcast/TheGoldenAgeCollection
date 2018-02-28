#include "norm_enemygenerator.h"

#include <prism/datastructures.h>
#include <prism/animation.h>

#include "norm_enemy.h"
#include "norm_button.h"

static struct {
	Duration mNow;
	int mStage;

	int mIsActive;
} gData;

static Duration gFrequencies[] = { 120, 100, 90, 80, 60 };

static void loadEnemyGenerator(void* tData) {
	(void)tData;
	gData.mNow = 0;
	gData.mStage = 0;

	gData.mIsActive = 0;

	loadNormEnemies();
}

static void updateEnemyGenerator(void* tData) {
	(void)tData;
	if (!gData.mIsActive) return;

	if (handleDurationAndCheckIfOver(&gData.mNow, gFrequencies[gData.mStage])) {
		gData.mNow = 0;

		addNormEnemy();
	}
}

ActorBlueprint NormEnemyGenerator = {
	.mLoad = loadEnemyGenerator,
	.mUpdate = updateEnemyGenerator,
};

void increaseNormEnemyGeneration()
{
	gData.mStage++;
}

void decreaseNormEnemyGeneration()
{
	gData.mStage--;
}

void setNormEnemyGeneratorActive()
{
	gData.mIsActive = 1;
}
