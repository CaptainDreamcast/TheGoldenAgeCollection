#include "mirk_stage.h"

#include <stdio.h>

#include <tari/animation.h>
#include <tari/stagehandler.h>
#include <tari/input.h>
#include <tari/sound.h>

#include "mirk_preciouspeople.h"

static struct {
	int planeID;
	int realPlaneID;
	int mBGTexture;
	int mRealTexture;
	double mShakeMaximum;

	Vector3D mStageOffset;

	int mIsReal;
} gData;


void increaseMirkScreenShake() {
	gData.mShakeMaximum += 10;
	setStageHandlerMaximumScreenShake(gData.mShakeMaximum);
}

void resetMirkScreenShakeLimit()
{
	gData.mShakeMaximum = 10;
	gData.mShakeMaximum += 10 * (5 - getMirkPreciousPeopleAmount());
	setStageHandlerMaximumScreenShake(gData.mShakeMaximum);
}


static void loadStage(void* tData) {
	(void)tData;

	gData.mStageOffset = makePosition(-300, -300, 0);

	setStageHandlerNoDelayedLoading();
	setStageHandlerTweening();
	gData.planeID = addScrollingBackground(1, 1);
	gData.realPlaneID = addScrollingBackground(1, 1);
	gData.mBGTexture = addBackgroundElement(gData.planeID, gData.mStageOffset, "assets/main/mirk/stage/BG.pkg", createOneFrameAnimation());
	gData.mRealTexture = addBackgroundElement(gData.realPlaneID, gData.mStageOffset, "assets/main/mirk/stage/REAL.pkg", createOneFrameAnimation());
	setScrollingBackgroundInvisible(gData.realPlaneID);

	setStageCameraRange(makeGeoRectangle(0,0,320,1080));
	gData.mShakeMaximum = 0;
	increaseMirkScreenShake();
	playTrack(3);
	gData.mIsReal = 0;
}

void setMirkStageReal() {
	if (gData.mIsReal) return;
	setScrollingBackgroundVisible(gData.realPlaneID);
	setScrollingBackgroundInvisible(gData.planeID);
	pauseTrack();
	gData.mIsReal = 1;
}

void setMirkStageUnreal() {
	if (!gData.mIsReal) return;
	setScrollingBackgroundVisible(gData.planeID);
	setScrollingBackgroundInvisible(gData.realPlaneID);
	resumeTrack();
	gData.mIsReal = 0;
}

void invertMirkStageReality()
{
	if (gData.mIsReal) setMirkStageUnreal();
	else setMirkStageReal();
}


ActorBlueprint MirkStageBP = {
	.mLoad = loadStage,
	.mUnload = NULL,
	.mIsActive = NULL
};

Position* getMirkStagePositionReference()
{
	return getScrollingBackgroundPositionReference(gData.planeID);
}

void drawMirkBloodOnStage(Position p, Color c)
{
	if (c == COLOR_RED) {
		c = COLOR_DARK_RED;
	}
	else if (c == COLOR_BLUE) {
		c = COLOR_DARK_BLUE;
	}
	else if (c == COLOR_GREEN) {
		c = COLOR_DARK_GREEN;
	}
	else if (c == COLOR_YELLOW) {
		c = COLOR_DARK_YELLOW;
	}

	TextureData* tex;
	if (gData.mIsReal) {
		tex = getBackgroundElementTextureData(gData.realPlaneID, gData.mRealTexture);
		c = COLOR_WHITE;
		return;
	}
	else {
		tex = getBackgroundElementTextureData(gData.planeID, gData.mBGTexture);
	}

	p = vecSub(p, gData.mStageOffset);

	drawColoredRectangleToTexture(tex[0], c, makeRectangle((int)p.x, (int)p.y, 2, 2));
}
