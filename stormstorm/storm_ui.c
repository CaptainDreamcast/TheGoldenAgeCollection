#include "storm_ui.h"

#include <tari/animation.h>
#include <tari/math.h>
#include <tari/wrapper.h>

#include "vectrexanimations.h"
#include "storm_gameoverscreen.h"

static struct {
	TextureData mNumberTextures[14];
	TextureData mCrossTexture;
	int mScore;
	Position mScorePosition;

	int mEnemiesMissed;
} gData;

static void loadUIHandler(void* tData) {
	(void)tData;
	loadConsecutiveTextures(gData.mNumberTextures, "assets/stormstorm/ui/numbers/NUM.pkg", 10);
	gData.mCrossTexture = loadTexture("assets/stormstorm/ui/CROSS.pkg");
	gData.mScorePosition = makePosition(430, 68, 4);
}

static void drawStormScoreOneFrame(Position tPosition, int tValue)
{
	int isFirst = 1;
	while (isFirst || tValue) {
		int v1 = tValue % 10;
		addVectrexOneFrameImage(&gData.mNumberTextures[v1], tPosition);

		tPosition.x -= 20;
		tValue /= 10;
		isFirst = 0;
	}
}

static void drawMissedEnemies() {
	Position p = makePosition(180, 55, 4);
	
	int i;
	for (i = 0; i < gData.mEnemiesMissed; i++) {
		addVectrexOneFrameImage(&gData.mCrossTexture, p);
		p.x += 40;
	}
}

static void updateUIHandler(void* tData) {
	(void)tData;

	drawMissedEnemies();
	drawStormScoreOneFrame(gData.mScorePosition, gData.mScore);
}

ActorBlueprint StormUIHandler = {
	.mLoad = loadUIHandler,
	.mUpdate = updateUIHandler,
};

void resetStormUI()
{
	gData.mScore = 0;
	gData.mEnemiesMissed = 0;
}

void increaseStormScore()
{
	gData.mScore++;
}

void increaseStormEnemiesMissed()
{
	gData.mEnemiesMissed++;
	if (gData.mEnemiesMissed == 3) {
		setNewScreen(&StormGameOverScreen);
	}
}

void setStormUIScorePosition(Position tPosition)
{
	gData.mScorePosition = tPosition;
}
