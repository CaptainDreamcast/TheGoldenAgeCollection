#pragma once

#include <tari/physics.h>
#include <tari/animation.h>

#include "hank_basetypes.h"

typedef enum {
  HANK_CHARACTER_STANDING = 0,
  HANK_CHARACTER_WALKING,
  HANK_CHARACTER_JUMPING,
  HANK_CHARACTER_DYING
} HankCharacterState;

typedef struct {

  PhysicsObject physics;
  Animation animation;
  Frame frameAmount[4];
  Duration animationDuration[4];
  double runAccel;
  HankCharacterState state;
  HankFaceDirection faceDirection;

} HankCharacterData;

