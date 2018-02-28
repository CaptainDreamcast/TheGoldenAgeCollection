#ifndef WORLDDATA_H
#define WORLDDATA_H

#include "hank_basetypes.h"
#include "hank_enemydata.h"

#include "hank_basedefinitions.h"

typedef enum {
  HANK_TILE_EMPTY,
  HANK_TILE_PLATFORM
} HankTileType;

typedef enum {
  HANK_GAME_RUNNING,
  HANK_GAME_WON,
  HANK_GAME_OVER
} HankWorldState;

#define HankTileToRealPositionX(x)			(x*HANK_TILE_SIZE)
#define HankTileToRealPositionY(y)			(y*HANK_TILE_SIZE+HANK_PLATFORM_SIZE_Y)
#define HankTileToRealPositionWithoutPlatformY(y)	(y*HANK_TILE_SIZE)
#define HankRealPositionToTileX(x)			(x/HANK_TILE_SIZE)
#define HankRealPositionToTileY(y)			((y-HANK_PLATFORM_SIZE_Y)/HANK_TILE_SIZE)
#define HankRealPositionToTileWitoutPlatformY(y)	(y/HANK_TILE_SIZE)

typedef struct {

  HankTileType tiles[HANK_MAX_TILES_Y][HANK_MAX_TILES_X];
  uint32_t enemyAmount;
  HankEnemyData enemies[HANK_MAX_ENEMY_AMOUNT];
  Position exitPosition;
  Position startPosition;
  double tiltVelocity;
  double tiltAngle;
  HankWorldState state;

} HankWorldData;

#endif
