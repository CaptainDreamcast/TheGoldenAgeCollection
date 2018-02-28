#ifndef ENEMYDATA_H
#define ENEMYDATA_H

#include <prism/physics.h>
#include <prism/animation.h>

#include "hank_basetypes.h"

typedef enum {

  HANK_ENEMY_WALKING,
  HANK_ENEMY_DYING

} HankEnemyState;

#define HANK_ENEMY_TEXTURE_AMOUNT 2

typedef struct {

  int isRemoved;
  PhysicsObject physics;
  Animation animation;
  double runAccel;
  HankEnemyState state;
  HankFaceDirection faceDirection;

} HankEnemyData;

#endif
