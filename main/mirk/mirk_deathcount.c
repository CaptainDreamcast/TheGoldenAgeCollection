#include "mirk_deathcount.h"

#include <stdio.h>
#include <string.h>

#include <prism/texthandler.h>
#include <prism/animation.h>
#include <prism/math.h>

static struct {
	int mDeathCountText;
	int mDeathCount;

	char mCounterNameText[100];

	TextureData mBGTexture;
	int mBG;

	int mIsReal;
	int mIsContinuing;
} gData;

static void updateDeathCount(void* tCaller);

static void loadDeathCount(void* tData) {
	(void)tData;

	if (!gData.mIsContinuing) {
		gData.mDeathCount = 0;
	}
	gData.mIsContinuing = 0;

	gData.mDeathCountText = addHandledText(makePosition(26, 28, 15), "Death Count: 0", 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, 0, 0), makePosition(INF, INF, INF), INF);

	gData.mBGTexture = loadTexture("assets/main/mirk/counter/BG.pkg");
	gData.mBG = playAnimationLoop(makePosition(20, 20, 14), &gData.mBGTexture, createOneFrameAnimation(), makeRectangle(0, 0, 200, 32));
	
	strcpy(gData.mCounterNameText, "Death");
	gData.mIsReal = 0;

	updateDeathCount(NULL);
}

static void updateDeathCount(void* tData) {
	(void)tData;

	char nText[1024];
	sprintf(nText, "%s Count: %d", gData.mCounterNameText, gData.mDeathCount);
	setHandledText(gData.mDeathCountText, nText);

	int rawLen = strlen(nText);
	int len = rawLen * 20 + (rawLen - 1) * (-5) + 6 * 2;
	int bglen = min(len, 512);
	setAnimationTexturePosition(gData.mBG, makeRectangle(0,0, bglen, 32));
}

ActorBlueprint MirkDeathCountBP = {
	.mLoad = loadDeathCount,
	.mUpdate = updateDeathCount
};

void increaseMirkDeathCount()
{
	gData.mDeathCount++;
}

int getMirkDeathCount() {
	return gData.mDeathCount;
}

void setMirkDeathCountToContinue()
{
	gData.mIsContinuing = 1;
}

void setMirkDeathCountReal()
{
	if (gData.mIsReal) return;

	strcpy(gData.mCounterNameText, "Kill");
	setAnimationColor(gData.mBG, 0, 0, 0);
	gData.mIsReal = 1;
}

void setMirkDeathCountUnreal()
{
	if (!gData.mIsReal) return;
	
	strcpy(gData.mCounterNameText, "Death");
	setAnimationColor(gData.mBG, 1, 1, 1);
	gData.mIsReal = 0;
}

void invertMirkDeathCountReality()
{
	if (gData.mIsReal) setMirkDeathCountUnreal();
	else setMirkDeathCountReal();
}
