#include "hank_roundscreen.h"

#include <stdio.h>

#include <prism/drawing.h>
#include <prism/animation.h>
#include <prism/input.h>
#include <prism/system.h>
#include <prism/texture.h>

#include "hank_round.h"
#include "hank_titlescreen.h"
#include "hank_gamescreen.h"

#define TEXT_POSITION_Z 2

typedef struct {
  Duration now;
  Duration screenShown;
  Position textPosition;
  TextSize textSize;
  Color textColor;
} RoundScreenData;

static RoundScreenData gRoundData;
static char roundScreenText[100];

static void updateRoundScreen() {
	updateSystem();
	updateInput();
	if (hasPressedAbortFlank()) {
		setNewScreen(&HankTitleScreen);
	}

	if (handleDurationAndCheckIfOver(&gRoundData.now, gRoundData.screenShown)) {
		setNewScreen(&HankGameScreen);
	}
}

static void drawRoundScreen() {
	drawText(roundScreenText, gRoundData.textPosition, gRoundData.textSize, gRoundData.textColor);
}

static void initialize() {
  memset(&gRoundData, 0, sizeof gRoundData);

  sprintf(roundScreenText, "Round: %d", getHankRound());

  gRoundData.screenShown = 60;
  gRoundData.textPosition.x = 200;
  gRoundData.textPosition.y = 200;
  gRoundData.textPosition.z = TEXT_POSITION_Z;
  gRoundData.textSize = 30;
  gRoundData.textColor = COLOR_WHITE;
}


static void loadRoundScreen() {
	increaseHankRound();
	initialize();
}

Screen HankRoundScreen = {
	.mLoad = loadRoundScreen,
	.mUpdate = updateRoundScreen,
	.mDraw = drawRoundScreen,
};