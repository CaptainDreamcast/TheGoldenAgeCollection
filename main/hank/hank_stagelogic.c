#include "hank_stagelogic.h"

#include <stdlib.h>

#include <tari/log.h>
#include <tari/input.h>

#include "hank_collision.h"

static int NO_TILT = 0;

void checkHankExit(HankWorldData* tWorldData, HankCharacterData* tCharacterData) {
	double offsetX = tCharacterData->faceDirection == HANK_FACE_LEFT ? 20 : 10;

  double dx = fabs(tWorldData->exitPosition.x - (tCharacterData->physics.mPosition.x + offsetX));
  double dy = fabs(tWorldData->exitPosition.y - (tCharacterData->physics.mPosition.y));


  if (dx < 5 && dy < 5) {
    tWorldData->state = HANK_GAME_WON;
  }
}

#define MAX_TILT_VELOCITY 0.075

static int direction = 0;

static void changeDirection() {
  direction = (direction + 1) % 2;
}

void handleHankScreenTilting(HankWorldData* tWorldData, HankCharacterData* tCharacterData) {

  if (NO_TILT) {
    return;
  }

  if (direction) {
    double tiltAcceleration = ((rand() % 500) * 0.000001);
    debugDouble(tiltAcceleration);
    tWorldData->tiltVelocity += tiltAcceleration;
  } else {
    double tiltAcceleration = -((rand() % 500) * 0.000001);
    debugDouble(tiltAcceleration);
    tWorldData->tiltVelocity += tiltAcceleration;
  }

  if (rand() % 500 == 0) {
    changeDirection();
  }

  if (tWorldData->tiltVelocity > MAX_TILT_VELOCITY) {
    tWorldData->tiltVelocity = MAX_TILT_VELOCITY;
    changeDirection();
  } else if (tWorldData->tiltVelocity < -MAX_TILT_VELOCITY) {
    tWorldData->tiltVelocity = -MAX_TILT_VELOCITY;
    changeDirection();
  }

  tWorldData->tiltAngle += tWorldData->tiltVelocity;
}

HankGameReturnType checkHankGameAbort(HankWorldData* tWorldData, HankCharacterData* tCharacterData) {
  if (hasPressedAbortFlank()) {
    return HANK_RETURN_TO_MENU;
  } else if (tWorldData->state == HANK_GAME_WON) {
    return HANK_RETURN_WON;
  } else if (tWorldData->state == HANK_GAME_OVER) {
    return HANK_RETURN_LOSS;
  } else {
    return HANK_RETURN_NORMAL;
  }
}

void checkHankGameOver(HankWorldData* tWorldData, HankCharacterData* tCharacterData) {
  if (hasHankLeftScreen(tWorldData, tCharacterData)) {
    tWorldData->state = HANK_GAME_OVER;
  }
}
