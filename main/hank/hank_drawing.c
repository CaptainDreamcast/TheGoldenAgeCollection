#include "hank_drawing.h"

#include <stdlib.h>

#include <tari/log.h>
#include <tari/drawing.h>
#include <tari/math.h>

#include "hank_texture.h"

static double inverseScreenY(double y) {
  return (HANK_SCREEN_SIZE_Y - y);
}

static double getDrawY(double y, Rectangle tTexturePosition) {
  double sizeY = tTexturePosition.bottomRight.y - tTexturePosition.topLeft.y;

  return inverseScreenY(y + sizeY);
}

void updateHankTiltingMatrix(double tTiltAngle) {
  setDrawingParametersToIdentity();
  setDrawingRotationZ(tTiltAngle, makePosition(HANK_MIDDLE_OF_SCREEN_X, HANK_MIDDLE_OF_SCREEN_Y, 0));
}

static void drawCharacter(HankWorldData* tWorldData, HankCharacterData* tCharacterData) {
  debugLog("Draw character");
  TextureData textureData = getHankCharacterTexture(tCharacterData->animation.mFrame);
  HankFaceDirection faceDirection = tCharacterData->faceDirection;

  Rectangle tTexturePosition;
  tTexturePosition.topLeft.x = (faceDirection == HANK_FACE_RIGHT) ? 0 : (textureData.mTextureSize.x - 1);
  tTexturePosition.topLeft.y = 0;
  tTexturePosition.bottomRight.x = (faceDirection != HANK_FACE_RIGHT) ? 0 : (textureData.mTextureSize.x - 1);
  tTexturePosition.bottomRight.y = textureData.mTextureSize.y - 1;

  Position drawPos = tCharacterData->physics.mPosition;
  drawPos.y = getDrawY(drawPos.y, tTexturePosition);

  drawSprite(textureData, drawPos, tTexturePosition);
}

static void drawEnemies(HankWorldData* tWorldData) {
  debugLog("Draw enemies");

  int i;
  for (i = 0; i < (int)tWorldData->enemyAmount; i++) {
    HankFaceDirection faceDirection = tWorldData->enemies[i].faceDirection;
    TextureData textureData = getHankEnemyTexture(tWorldData->enemies[i].animation.mFrame);

    Rectangle tTexturePosition;
    tTexturePosition.topLeft.x = (faceDirection == HANK_FACE_RIGHT) ? 0 : (textureData.mTextureSize.x - 1);
    tTexturePosition.topLeft.y = 0;
    tTexturePosition.bottomRight.x = (faceDirection != HANK_FACE_RIGHT) ? 0 : (textureData.mTextureSize.x - 1);
    tTexturePosition.bottomRight.y = textureData.mTextureSize.y - 1;

    Position drawPos = tWorldData->enemies[i].physics.mPosition;
    drawPos.y = getDrawY(drawPos.y, tTexturePosition);

    drawSprite(textureData, drawPos, tTexturePosition);
  }
}

static void drawBackground(HankWorldData* tWorldData) {
  debugLog("Draw background");

  TextureData textureData = getHankBackgroundTexture();
  Rectangle tTexturePosition;

  tTexturePosition.topLeft.x = 0;
  tTexturePosition.topLeft.y = 0;
  tTexturePosition.bottomRight.x = textureData.mTextureSize.x - 1;
  tTexturePosition.bottomRight.y = textureData.mTextureSize.y - 1;

  Position position;
  position.x = 0;
  position.y = 0;
  position.z = HANK_BACKGROUND_POSITION_Z;

  drawSprite(textureData, position, tTexturePosition);
}

static void drawPlatforms(HankWorldData* tWorldData) {
  debugLog("Draw platforms");

  TextureData textureData = getHankPlatformTexture();

  Rectangle tTexturePosition;
  tTexturePosition.topLeft.x = 0;
  tTexturePosition.topLeft.y = 0;
  tTexturePosition.bottomRight.x = textureData.mTextureSize.x - 1;
  tTexturePosition.bottomRight.y = textureData.mTextureSize.y - 1;

  int i, j;
  for (j = 0; j < HANK_MAX_TILES_Y; j++) {
    for (i = 0; i < HANK_MAX_TILES_X; i++) {

      if (tWorldData->tiles[j][i] == HANK_TILE_PLATFORM) {
        debugLog("Draw single tile");
        debugInteger(i);
        debugInteger(j);
        Position position;
        position.x = HankTileToRealPositionX(i);
        position.y = HankTileToRealPositionWithoutPlatformY(j);
        position.y = getDrawY(position.y, tTexturePosition);
        position.z = HANK_PLATFORM_POSITION_Z;

        drawSprite(textureData, position, tTexturePosition);
      }
    }
  }
}

static void drawExit(HankWorldData* tWorldData) {
  debugLog("Draw Exit");

  TextureData textureData = getHankExitTexture();

  Rectangle tTexturePosition;
  tTexturePosition.topLeft.x = 0;
  tTexturePosition.topLeft.y = 0;
  tTexturePosition.bottomRight.x = textureData.mTextureSize.x - 1;
  tTexturePosition.bottomRight.y = textureData.mTextureSize.y - 1;

  Position pos = tWorldData->exitPosition;
  pos.y = getDrawY(pos.y, tTexturePosition);

  drawSprite(textureData, pos, tTexturePosition);
}

void drawHank(HankWorldData* tWorldData, HankCharacterData* tCharacterData) {
  debugLog("Begin drawing");
  updateHankTiltingMatrix(tWorldData->tiltAngle);
  drawBackground(tWorldData);
  drawPlatforms(tWorldData);
  drawCharacter(tWorldData, tCharacterData);
  drawEnemies(tWorldData);
  drawExit(tWorldData);
}

