#include "luck_scrolling.h"

#include <tari/stagehandler.h>

static struct {
	int mLayer;
	int mBG1;
	int mBG2;

	char mName[1000];
} gData;

static void loadScrollingBG(void* tData) {
	(void)tData;
	gData.mLayer = addScrollingBackground(1, 1);

	char path[1024];
	sprintf(path, "assets/misc/luck/sprites/%s1_.pkg", gData.mName);
	gData.mBG1 = addBackgroundElement(gData.mLayer, makePosition(0,0,0), path, createOneFrameAnimation());
	sprintf(path, "assets/misc/luck/sprites/%s2_.pkg", gData.mName);
	gData.mBG2 = addBackgroundElement(gData.mLayer, makePosition(1020, 0, 0), path, createOneFrameAnimation());

	setScrollingBackgroundMaxVelocity(gData.mLayer, 10);
}

static void updateScrollingBG(void* tData) {
	(void)tData;
	scrollBackgroundRight(1);

	Position p = *getScrollingBackgroundPositionReference(gData.mLayer);
	if (p.x >= 640) {
		p.x -= 640;
		setScrollingBackgroundPosition(gData.mLayer, p);
	}
}

ActorBlueprint LuckScrollingBackgroundBP = {
	.mLoad = loadScrollingBG,
	.mUpdate = updateScrollingBG,
};

void setLuckScrollingBGName(char * tName)
{
	strcpy(gData.mName, tName);
}
