#include "hank_titlescreen.h"

#include <tari/log.h>
#include <tari/input.h>
#include <tari/math.h>
#include <tari/drawing.h>
#include <tari/system.h>

#include "hank_titledata.h"
#include "hank_drawing.h"
#include "hank_texture.h"
#include "hank_round.h"
#include "hank_roundscreen.h"

#include "../../maingamemenu.h"

#define TITLE_TILT_ACCELERATION 0.001
#define TITLE_TILT_VELOCITY_MAX	0.02

static HankTitleData gTitleData;

static void calculateTilt() {
  gTitleData.tiltVelocity += gTitleData.tiltAcceleration;

  if (gTitleData.tiltVelocity > TITLE_TILT_VELOCITY_MAX) {
    gTitleData.tiltVelocity = TITLE_TILT_VELOCITY_MAX;
  } else if (gTitleData.tiltVelocity < -TITLE_TILT_VELOCITY_MAX) {
    gTitleData.tiltVelocity = -TITLE_TILT_VELOCITY_MAX;
  }
  gTitleData.tiltAngle += gTitleData.tiltVelocity;

  debugDouble(gTitleData.tiltAngle);
  if ((gTitleData.tiltAngle < -0.5 && gTitleData.tiltAcceleration < 0) || (gTitleData.tiltAngle > 0.5 && gTitleData.tiltAcceleration > 0)) {
    debugLog("switch tilt direction");
    gTitleData.tiltAcceleration *= -1;
  }
}

static void checkDone() {

  if (hasPressedStartFlank()) {
	  setNewScreen(&HankRoundScreen);
  }  
}

static void updateTitleScreen() {
  updateSystem();
  updateInput();
  if (hasPressedAbortFlank()) {
	  setNewScreen(&MainGameMenu);
	 return;
  }

  checkDone();
  calculateTilt();
}

static void drawTitleBackground(HankTitleData* tTitleData) {
	debugLog("Draw Title background");

	TextureData textureData = getHankTitleTexture();

	Rectangle tTexturePosition;

	tTexturePosition.topLeft.x = 0;
	tTexturePosition.topLeft.y = 0;
	tTexturePosition.bottomRight.x = textureData.mTextureSize.x - 1;
	tTexturePosition.bottomRight.y = textureData.mTextureSize.y - 1;

	Position pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = HANK_BACKGROUND_POSITION_Z;

	drawSprite(textureData, pos, tTexturePosition);
}

static void drawPressStart(HankTitleData* tTitleData) {
	debugLog("Draw Title background");

	TextureData textureData = getHankPressStartTexture();

	Rectangle tTexturePosition;
	tTexturePosition.topLeft.x = 0;
	tTexturePosition.topLeft.y = 0;
	tTexturePosition.bottomRight.x = textureData.mTextureSize.x - 1;
	tTexturePosition.bottomRight.y = textureData.mTextureSize.y - 1;

	Position pos;
	pos.x = HANK_PRESS_START_X;
	pos.y = HANK_PRESS_START_Y;
	pos.z = HANK_CHARACTER_POSITION_Z;

	drawSprite(textureData, pos, tTexturePosition);
}

static void drawTitle() {
	debugLog("Begin drawing title");
	updateHankTiltingMatrix(gTitleData.tiltAngle);
	drawTitleBackground(&gTitleData);
	drawPressStart(&gTitleData);
}


static void initiateTitle() {
  gTitleData.tiltAcceleration = TITLE_TILT_ACCELERATION;
}

static void loadTitleScreen() {
	setGravity(makePosition(0, 0, 0));
	loadAllHankTextures();
	resetHankRound();
	memset(&gTitleData, 0, sizeof gTitleData);
	initiateTitle();
}


Screen HankTitleScreen = {
	.mLoad = loadTitleScreen,
	.mUpdate = updateTitleScreen,
	.mDraw = drawTitle,
};
