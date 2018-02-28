#include "hank_generateLevel.h"

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <prism/log.h>
#include <prism/math.h>
#include <prism/physics.h>

#include "hank_basedefinitions.h"
#include "hank_texture.h"
#include "hank_state.h"

HankWorldData* gHankWorldData;

static void placePlatform(int y, int x, size_t size) {

  int i;
  for (i = x; i < (int)(x + size); i++) {
    gHankWorldData->tiles[y][i] = HANK_TILE_PLATFORM;
  }
}

static int impossibleToPlacePlatform(int y, int x) {
  if (x > 0 && gHankWorldData->tiles[y][x - 1] == HANK_TILE_PLATFORM)
    return 1;

  return gHankWorldData->tiles[y - 1][x] == HANK_TILE_PLATFORM;
}

static void generatePlatforms() {

  logg("Generate Platforms");

  placePlatform(0, 0, HANK_MAX_TILES_X);

  int j, i, k;
  for (j = 1; j < HANK_MAX_TILES_Y; j++) {
    for (i = 0; i < HANK_MAX_TILES_X - 1; i++) {
      debugLog("Try place Platform");
      debugInteger(i);
      debugInteger(j);

      if (impossibleToPlacePlatform(j, i))
        continue;

      debugLog("Placement possible");
      size_t maxSize = 1;
      for (k = i + 1; k < HANK_MAX_TILES_X; k++) {
        if (impossibleToPlacePlatform(j, k))
          break;
        else
          maxSize++;
      }
      size_t size = (rand() % maxSize) + 1;

      debugInteger(maxSize);
      debugInteger(size);

      if (rand() % 10 > 7) {
        debugLog("Place platform");
        placePlatform(j, i, size);
      }
    }
  }
}

static int impossibleToPlaceEnemy(int y, int x) {

  int i;
  for (i = 0; i < (int)gHankWorldData->enemyAmount; i++) {
    if (gHankWorldData->enemies[i].physics.mPosition.x == HankTileToRealPositionX(x) && gHankWorldData->enemies[i].physics.mPosition.y == HankTileToRealPositionY(y)) {
      return 0;
    }
  }

  return gHankWorldData->tiles[y][x] != HANK_TILE_PLATFORM;
}

static void placeEnemy(int y, int x) {

  double pX = HankTileToRealPositionX(x);
  double pY = HankTileToRealPositionY(y);

  gHankWorldData->enemyAmount++;
  int id = gHankWorldData->enemyAmount - 1;

  gHankWorldData->enemies[id].isRemoved = 0;
  gHankWorldData->enemies[id].physics.mPosition.x = pX;
  gHankWorldData->enemies[id].physics.mPosition.y = pY;
  gHankWorldData->enemies[id].physics.mPosition.z = HANK_ENEMY_POSITION_Z;
  gHankWorldData->enemies[id].runAccel = HANK_ENEMY_RUN_ACCEL;
  gHankWorldData->enemies[id].animation.mFrameAmount = HANK_ENEMY_FRAME_AMOUNT;
  gHankWorldData->enemies[id].animation.mDuration = 30;
  changeHankEnemyState(&gHankWorldData->enemies[id], HANK_ENEMY_WALKING);
  resetAnimation(&gHankWorldData->enemies[id].animation);
}

static void generateEnemies() {
  logg("Generate enemies");

  int placedEnemies = 0;

  int enemiesToBePlaced = (rand() % HANK_MAX_ENEMY_AMOUNT) + 1;

  logInteger(enemiesToBePlaced);

  int j, i;
  int keepPlacingEnemies = 1;
  while (keepPlacingEnemies) {
    for (j = 1; j < HANK_MAX_TILES_Y; j++) {
      for (i = 0; i < HANK_MAX_TILES_X; i++) {
        debugLog("Try place Enemy");
        debugInteger(i);
        debugInteger(j);

        if (impossibleToPlaceEnemy(j, i))
          continue;

        debugLog("Possible to place enemy");

        if (rand() % 10 > 8) {
          debugLog("Place enemy");
          placeEnemy(j, i);
          placedEnemies++;
          if (placedEnemies == enemiesToBePlaced) {
            keepPlacingEnemies = 0;
            break;
          }
        }
      }
      if (!keepPlacingEnemies) {
        break;
      }
    }

  }
}

static int impossibleToPlaceExit(int y, int x) {
  return gHankWorldData->tiles[y][x] != HANK_TILE_PLATFORM;
}

HankTilePosition exitHankPositionTile;

static void placeExit(int y, int x) {
  gHankWorldData->exitPosition.x = HankTileToRealPositionX(x);
  gHankWorldData->exitPosition.y = HankTileToRealPositionY(y);
  gHankWorldData->exitPosition.z = HANK_EXIT_POSITION_Z;

  exitHankPositionTile.x = x;
  exitHankPositionTile.y = y;
}

static void generateExit() {
  logg("Generate Exit");

  int keepPlacingExit = 1;
  while (keepPlacingExit) {
    int j, i;
    for (j = 1; j < HANK_MAX_TILES_Y; j++) {
      for (i = 0; i < HANK_MAX_TILES_X; i++) {
        debugLog("Try place Exit");
        debugInteger(i);
        debugInteger(j);

        if (impossibleToPlaceExit(j, i))
          continue;

        debugLog("Exit placement possible");

        if (rand() % 100 > 98) {
          debugLog("Place Exit");
          placeExit(j, i);
          keepPlacingExit = 0;
          break;
        }
      }
      if (!keepPlacingExit) {
        break;
      }
    }
  }
}

static int vis[HANK_MAX_TILES_Y][HANK_MAX_TILES_X];

static void checkReachableRecursive(int y, int x) {
  if (x < 0 || y < 0 || x >= HANK_MAX_TILES_X || y >= HANK_MAX_TILES_Y)
    return;

  if (vis[y][x])
    return;

  debugLog("Visit");
  debugInteger(x);
  debugInteger(y);

  vis[y][x] = 1;

  if (gHankWorldData->tiles[y][x] == HANK_TILE_PLATFORM) {
    checkReachableRecursive(y + 2, x);
    checkReachableRecursive(y + 1, x);
    checkReachableRecursive(y + 1, x + 1);
    checkReachableRecursive(y + 1, x - 1);
    checkReachableRecursive(y, x + 2);
    checkReachableRecursive(y, x - 2);
    checkReachableRecursive(y, x + 1);
    checkReachableRecursive(y, x - 1);
  }
}

static int isNotPlayable() {
  logg("Check playability");
  memset(vis, 0, sizeof vis);

  int y = (int)HankRealPositionToTileY(gHankWorldData->startPosition.y);
  int x = (int)HankRealPositionToTileX(gHankWorldData->startPosition.x);

  checkReachableRecursive(y, x);

  return (!vis[exitHankPositionTile.y][exitHankPositionTile.x]);
}

static void generateGravity() {
  logg("Generate gravity");
  Gravity grav;
  grav.x = 0;
  grav.y = -HANK_GRAVITY;
  grav.z = 0;
  setGravity(grav);
}

void generateHankLevel(HankWorldData* tWorldData) {
  logg("Generate Level");
  gHankWorldData = tWorldData;
  srand((unsigned int)time(NULL));
  int attempt = 0;
  do {
    attempt++;
    logInteger(attempt);

    memset(tWorldData, 0, sizeof *tWorldData);

    generatePlatforms();
    generateEnemies();
    generateExit();
  } while (isNotPlayable());

  generateGravity();
}
