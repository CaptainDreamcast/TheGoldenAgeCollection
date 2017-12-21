#pragma once

#include <stdint.h>

typedef enum {
  HANK_RETURN_NORMAL,
  HANK_RETURN_TO_MENU,
  HANK_RETURN_WON,  //TODO: rename to _WIN
  HANK_RETURN_LOSS
} HankGameReturnType;

typedef enum {
  HANK_FACE_RIGHT,
  HANK_FACE_LEFT
} HankFaceDirection;

typedef struct {
  int x;
  int y;
} HankTilePosition;

