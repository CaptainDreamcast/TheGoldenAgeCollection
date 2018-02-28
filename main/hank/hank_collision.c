#include "hank_collision.h"

#include <prism/log.h>
#include <prism/collision.h>

#include "hank_state.h"

static void swapF(double* x, double* y) {
	double z = *x;
	*x = *y;
	*y = z;
}

static CollisionRect getPlatformRectangle(int y, int x) {

  Position pos;

  pos.x = HankTileToRealPositionX(x);
  pos.y = HankTileToRealPositionWithoutPlatformY(y);

  CollisionRect rect;
  rect.mTopLeft.x = pos.x;
  rect.mBottomRight.x = pos.x + HANK_TILE_SIZE;
  rect.mBottomRight.y = pos.y;
  rect.mTopLeft.y = pos.y + HANK_PLATFORM_SIZE_Y;

  swapF(&rect.mBottomRight.y, &rect.mTopLeft.y);

  return rect;
}

static CollisionRect getCharacterRectangleFromPosition(Position tPos) {

  CollisionRect rect;

  rect.mTopLeft.x = tPos.x + 10;
  rect.mBottomRight.x = tPos.x + HANK_CHARACTER_SIZE_X - 10;
  rect.mBottomRight.y = tPos.y;
  rect.mTopLeft.y = tPos.y + HANK_CHARACTER_SIZE_Y;

  swapF(&rect.mBottomRight.y, &rect.mTopLeft.y);

  return rect;
}

static int charMovesDown(Velocity tVelocity) {
  return tVelocity.y < 0;
}

static int comingFromAbove(CollisionRect tCharRect, CollisionRect tPlatRect, Velocity tVelocity) {
  Position previousPosition;
  previousPosition.x = tCharRect.mTopLeft.x - tVelocity.x;
  previousPosition.y = tCharRect.mBottomRight.y - tVelocity.y;

  return charMovesDown(tVelocity) && !checkCollision(getCharacterRectangleFromPosition(previousPosition), tPlatRect);
}

static int checkCollisionPlatform(CollisionRect tCharRect, CollisionRect tPlatRect, Velocity tVelocity) {

  int isValidCollision = tCharRect.mBottomRight.y > tPlatRect.mBottomRight.y;
  return isValidCollision && comingFromAbove(tCharRect, tPlatRect, tVelocity) && checkCollision(tCharRect, tPlatRect);
}

// TODO: replace with good lib function
static void resolvePlatformCollision(HankCharacterData* tCharacterData, int tY) {
  debugLog("Resolve collision for character");
  tCharacterData->physics.mPosition.y = HankTileToRealPositionY(tY);
  tCharacterData->physics.mVelocity.y = 0;
  tCharacterData->physics.mAcceleration.y = 0;

  if (tCharacterData->state == HANK_CHARACTER_JUMPING) {
    changeHankCharacterState(tCharacterData, HANK_CHARACTER_STANDING);
  }

}

void checkHankCollisionsPlatforms(HankWorldData* tWorldData, HankCharacterData* tCharacterData) {
  if (tCharacterData->state == HANK_CHARACTER_DYING)
    return;

  int j;
  for (j = 0; j < HANK_MAX_TILES_Y; j++) {
    int i;
    for (i = 0; i < HANK_MAX_TILES_X; i++) {
      if (tWorldData->tiles[j][i] == HANK_TILE_PLATFORM) {
        debugLog("Check collision with platform");
        debugInteger(i);
        debugInteger(j);
        if (checkCollisionPlatform(getCharacterRectangleFromPosition(tCharacterData->physics.mPosition), getPlatformRectangle(j, i), tCharacterData->physics.mVelocity)) {
          resolvePlatformCollision(tCharacterData, j);
        }
      }
    }
  }
}

// TODO: replace with good lib function
static void resolvePlatformCollisionEnemy(HankEnemyData* tEnemyData, int tY) {
  debugLog("Resolve collision for enemy");
  tEnemyData->physics.mPosition.y = HankTileToRealPositionY(tY);
  tEnemyData->physics.mVelocity.y = 0;
  tEnemyData->physics.mAcceleration.y = 0;
}

static void checkCollisionsEnemyAndPlatforms(HankWorldData* tWorldData, HankEnemyData* tEnemyData) {

  int j;
  for (j = 0; j < HANK_MAX_TILES_Y; j++) {
    int i;
    for (i = 0; i < HANK_MAX_TILES_X; i++) {
      if (tWorldData->tiles[j][i] == HANK_TILE_PLATFORM) {
        debugLog("Check collision with platform for enemy");
        debugInteger(i);
        debugInteger(j);
        if (checkCollisionPlatform(getCharacterRectangleFromPosition(tEnemyData->physics.mPosition), getPlatformRectangle(j, i), tEnemyData->physics.mVelocity)) {
          resolvePlatformCollisionEnemy(tEnemyData, j);
        }
      }
    }
  }
}
// TODO: move
static void killEnemy(HankCharacterData* tCharacterData, HankEnemyData* tEnemyData) {
  changeHankEnemyState(tEnemyData, HANK_ENEMY_DYING);
  tCharacterData->physics.mVelocity.y = 0;
  tCharacterData->physics.mAcceleration.y = 10;

}

static void killPlayer(HankCharacterData* tCharData) {
  tCharData->physics.mVelocity.y = 0;
  tCharData->physics.mAcceleration.y = 10;
  changeHankCharacterState(tCharData, HANK_CHARACTER_DYING);
}

static void checkCollisionsEnemyAndPlayer(HankWorldData* tWorldData, HankEnemyData* tEnemyData, HankCharacterData* tCharacterData) {
  if (tCharacterData->state == HANK_CHARACTER_DYING)
    return;

  CollisionRect charRect = getCharacterRectangleFromPosition(tCharacterData->physics.mPosition);
  CollisionRect enemyRect = getCharacterRectangleFromPosition(tEnemyData->physics.mPosition);


  if (checkCollision(charRect, enemyRect)) {
    int isKillingEnemy = comingFromAbove(charRect, enemyRect, tCharacterData->physics.mVelocity);

    if (isKillingEnemy) {
      killEnemy(tCharacterData, tEnemyData);
    } else {
      killPlayer(tCharacterData);
    }

  }
}

void checkHankCollisionsEnemies(HankWorldData* tWorldData, HankCharacterData* tCharacterData) {
  int i;
  for (i = 0; i < (int)tWorldData->enemyAmount; i++) {
    if (tWorldData->enemies[i].state != HANK_ENEMY_DYING) {
      checkCollisionsEnemyAndPlatforms(tWorldData, &tWorldData->enemies[i]);
      checkCollisionsEnemyAndPlayer(tWorldData, &tWorldData->enemies[i], tCharacterData);
    }
  }
}

static CollisionRect getScreenRectangle() {
  CollisionRect r;

  r.mTopLeft.x = 0;
  r.mTopLeft.y = HANK_SCREEN_SIZE_Y;
  r.mBottomRight.x = HANK_SCREEN_SIZE_X;
  r.mBottomRight.y = 0;

  return r;
}

int hasHankLeftScreen(HankWorldData* tWorldData, HankCharacterData* tCharacterData) {
  CollisionRect charRect = getCharacterRectangleFromPosition(tCharacterData->physics.mPosition);
  CollisionRect screenRect = getScreenRectangle();

  return charRect.mTopLeft.y < (screenRect.mBottomRight.y - 100);
}

