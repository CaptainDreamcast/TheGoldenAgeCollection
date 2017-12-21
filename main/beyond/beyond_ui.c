#include "beyond_ui.h"

#include <tari/animation.h>
#include <tari/texthandler.h>
#include <tari/math.h>
#include <tari/mugenanimationhandler.h>

#include "beyond_main.h"

static struct {
	MugenSpriteFile mSprites;
	MugenAnimation* mAnimation;
	int mAnimationID;

	MugenAnimation* mStageDisplayAnimation;
	int mStageDisplayAnimationID;

	int mPowerTextID;
	int mLifeAmountTextID;
	int mBombAmountTextID;
} gData;

static void loadUserInterface(void* tData) {
	(void)tData;
	
	char path[1024];
	sprintf(path, "assets/main/%s/ui/UI.sff", getBeyondDirectory());
	gData.mSprites = loadMugenSpriteFileWithoutPalette(path);
	gData.mAnimation = createOneFrameMugenAnimationForSprite(0, 0);

	gData.mAnimationID = addMugenAnimation(gData.mAnimation, &gData.mSprites, makePosition(0, 327, 40));

	gData.mLifeAmountTextID = addHandledText(makePosition(540, 350, 45), "0", 0, COLOR_WHITE, makePosition(30, 30, 30), makePosition(-5, -5, 0), makePosition(INF, INF, INF), INF);
	gData.mBombAmountTextID = addHandledText(makePosition(540, 387, 45), "0", 0, COLOR_WHITE, makePosition(30, 30, 30), makePosition(-5, -5, 0), makePosition(INF, INF, INF), INF);

	if (isUltimateFrontier()) {
		gData.mPowerTextID = addHandledText(makePosition(480, 424, 45), "0.00", 0, COLOR_WHITE, makePosition(25, 25, 30), makePosition(-5, -5, 0), makePosition(INF, INF, INF), INF);
	}
	else {
		gData.mPowerTextID = addHandledText(makePosition(480, 420, 45), "0.00", 0, COLOR_WHITE, makePosition(30, 30, 30), makePosition(-5, -5, 0), makePosition(INF, INF, INF), INF);
	}
}

ActorBlueprint BeyondUserInterface = {
	.mLoad = loadUserInterface,
};

void loadBeyondStageDisplay(MugenSpriteFile * tSprites)
{
	gData.mStageDisplayAnimation = createOneFrameMugenAnimationForSprite(9500, 0);
	gData.mStageDisplayAnimationID = addMugenAnimation(gData.mStageDisplayAnimation, tSprites, makePosition(15, 327+29, 45));
}

static void setUltimateFrontierPowerText(int tPower) {
	char text[100];
	sprintf(text, "%d/128", tPower);
	setHandledText(gData.mPowerTextID, text);
}

static void setRealBeyondPowerText(int tPower) {
	int decimal = tPower / 100;
	int fraction = tPower % 100;

	char text[100];
	if (fraction < 10) {
		sprintf(text, "%d.0%d", decimal, fraction);
	}
	else {
		sprintf(text, "%d.%d", decimal, fraction);
	}
	setHandledText(gData.mPowerTextID, text);
}

void setBeyondPowerText(int tPower)
{
	if (isUltimateFrontier()) {
		setUltimateFrontierPowerText(tPower);
	}
	else {
		setRealBeyondPowerText(tPower);
	}
}

void setBeyondLifeText(int tLifeAmount)
{
	char text[100];
	sprintf(text, "%d", tLifeAmount);
	setHandledText(gData.mLifeAmountTextID, text);
}

void setBeyondBombText(int tBombAmount)
{
	char text[100];
	sprintf(text, "%d", tBombAmount);
	setHandledText(gData.mBombAmountTextID, text);
}