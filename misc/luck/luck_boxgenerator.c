#include "luck_boxgenerator.h"

#include <prism/animation.h>
#include <prism/math.h>

#include "luck_box.h"

static struct {

	Duration mNow;
	Duration mNext;

} gData;

static void loadBoxGenerator(void* tData) {
	(void)tData;
	loadLuckBoxes();
	gData.mNow = 0;
	gData.mNext = randfromInteger(10, 60);
}

static void updateBoxGenerator(void* tData) {
	(void)tData;
	if (handleDurationAndCheckIfOver(&gData.mNow, gData.mNext)) {
		addLuckBox();
		gData.mNow = 0;
		gData.mNext = randfromInteger(30, 60);
	}
}

ActorBlueprint LuckBoxGeneratorBP = {
	.mLoad = loadBoxGenerator,
	.mUpdate = updateBoxGenerator,
};