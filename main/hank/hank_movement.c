#include "hank_movement.h"

#include <tari/input.h>
#include <tari/log.h>

#include "hank_state.h"

#define CHARACTER_JUMPING_ACCEL	15.0

static int characterCanJump(HankCharacterData* tCharacterData) {
  return tCharacterData->state == HANK_CHARACTER_STANDING || tCharacterData->state == HANK_CHARACTER_WALKING;
}

static int isJumpingInvoluntarily(HankCharacterData* tCharacterData) {
  return (tCharacterData->state == HANK_CHARACTER_STANDING || tCharacterData->state == HANK_CHARACTER_WALKING) && tCharacterData->physics.mVelocity.y != 0;
}

void checkHankJumpingCharacter(HankWorldData* tWorldData, HankCharacterData* tCharacterData) {
  if (tCharacterData->state == HANK_CHARACTER_DYING)
    return;

  if (hasPressedAFlank() && characterCanJump(tCharacterData)) {
    tCharacterData->physics.mAcceleration.y += CHARACTER_JUMPING_ACCEL;
    changeHankCharacterState(tCharacterData, HANK_CHARACTER_JUMPING);
  } else if (isJumpingInvoluntarily(tCharacterData)) {
    changeHankCharacterState(tCharacterData, HANK_CHARACTER_JUMPING);
  }
}

static void move(HankCharacterData* tCharacterData, int tMultiplier, HankFaceDirection tFaceDirection) {
  tCharacterData->physics.mAcceleration.x = tMultiplier * HANK_CHARACTER_RUN_ACCEL;
  tCharacterData->faceDirection = tFaceDirection;
  if (tCharacterData->state != HANK_CHARACTER_JUMPING && tCharacterData->state != HANK_CHARACTER_WALKING) {
    changeHankCharacterState(tCharacterData, HANK_CHARACTER_WALKING);
  }
}

static int characterCanRun(HankCharacterData* tCharacterData) {
  return 1;
}

void checkHankRunningCharacter(HankWorldData* tWorldData, HankCharacterData* tCharacterData) {
  if (tCharacterData->state == HANK_CHARACTER_DYING)
    return;

  if (characterCanRun(tCharacterData)) {
    if (hasPressedLeft()) {
      move(tCharacterData, -1, HANK_FACE_LEFT);
    } else if (hasPressedRight()) {
      move(tCharacterData, 1, HANK_FACE_RIGHT);
    } else if (tCharacterData->state == HANK_CHARACTER_WALKING) {
      changeHankCharacterState(tCharacterData, HANK_CHARACTER_STANDING);
    }
  }
}

static int isMovingLeft(HankEnemyData* tEnemyData) {
  return tEnemyData->faceDirection == HANK_FACE_LEFT;
}

#define LEFT_BORDER_THRESHOLD 5
#define RIGHT_BORDER_THRESHOLD 0

static int isOnLeftPlatformBorder(HankEnemyData* tEnemyData) {
  int positionInTile = ((int) tEnemyData->physics.mPosition.x) % HANK_TILE_SIZE;
  return positionInTile < LEFT_BORDER_THRESHOLD;
}
static int isOnRightPlatformBorder(HankEnemyData* tEnemyData) {
  int positionInTile = ((int) tEnemyData->physics.mPosition.x) % HANK_TILE_SIZE;
  return positionInTile > RIGHT_BORDER_THRESHOLD;
}

static int hasNoPlatformToTheLeft(HankWorldData* tWorldData, HankEnemyData* tEnemyData) {
  int tX = (int)HankRealPositionToTileX(tEnemyData->physics.mPosition.x);
  int tY = (int)HankRealPositionToTileWitoutPlatformY(tEnemyData->physics.mPosition.y);

  return (tX == 0 || tWorldData->tiles[tY][tX - 1] == HANK_TILE_EMPTY);
}
static int hasNoPlatformToTheRight(HankWorldData* tWorldData, HankEnemyData* tEnemyData) {
  int tX = (int)HankRealPositionToTileX(tEnemyData->physics.mPosition.x);
  int tY = (int)HankRealPositionToTileWitoutPlatformY(tEnemyData->physics.mPosition.y);

  return (tX == (HANK_MAX_TILES_X - 1) || tWorldData->tiles[tY][tX + 1] == HANK_TILE_EMPTY);
}

static int cannotMoveLeft(HankWorldData* tWorldData, HankEnemyData* tEnemyData) {
  return isOnLeftPlatformBorder(tEnemyData) && hasNoPlatformToTheLeft(tWorldData, tEnemyData);
}
static int cannotMoveRight(HankWorldData* tWorldData, HankEnemyData* tEnemyData) {
  return isOnRightPlatformBorder(tEnemyData) && hasNoPlatformToTheRight(tWorldData, tEnemyData);
}

static void turnLeft(HankEnemyData* tEnemyData) {
  tEnemyData->faceDirection = HANK_FACE_LEFT;
}
static void turnRight(HankEnemyData* tEnemyData) {
  tEnemyData->faceDirection = HANK_FACE_RIGHT;
}

static void moveLeft(HankEnemyData* tEnemyData) {
  tEnemyData->physics.mAcceleration.x = -HANK_ENEMY_RUN_ACCEL;
}
static void moveRight(HankEnemyData* tEnemyData) {
  tEnemyData->physics.mAcceleration.x = HANK_ENEMY_RUN_ACCEL;
}

static void checkSingleEnemyMovement(HankWorldData* tWorldData, HankEnemyData* tEnemyData) {

  if (isMovingLeft(tEnemyData)) {
    if (cannotMoveLeft(tWorldData, tEnemyData)) {
      turnRight(tEnemyData);
    } else {
      moveLeft(tEnemyData);
    }
  } else {
    if (cannotMoveRight(tWorldData, tEnemyData)) {
      turnLeft(tEnemyData);
    } else {
      moveRight(tEnemyData);
    }
  }
}

void checkHankMovementEnemies(HankWorldData* tWorldData, HankCharacterData* tCharacterData) {

  int i;
  for (i = 0; i < (int)tWorldData->enemyAmount; i++) {
    debugLog("Movement enemy");
    debugInteger(i);
    debugInteger(tWorldData->enemies[i].state);
    if (tWorldData->enemies[i].state == HANK_ENEMY_DYING)
      continue;
    checkSingleEnemyMovement(tWorldData, &tWorldData->enemies[i]);
  }

}
