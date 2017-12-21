#include "hank_animation.h"

#include <tari/animation.h>

void handleHankCharacterAnimation(HankWorldData* tWorldData, HankCharacterData* tCharacterData) {
  HankCharacterState st = tCharacterData->state;
  // TODO: move to state change
  tCharacterData->animation.mFrameAmount = tCharacterData->frameAmount[st];
  tCharacterData->animation.mDuration = tCharacterData->animationDuration[st];

  animate(&tCharacterData->animation);
}

void handleHankEnemyAnimation(HankWorldData* tWorldData, HankCharacterData* tCharacterData) {
  int i;
  for (i = 0; i < (int)tWorldData->enemyAmount; i++) {
    animate(&tWorldData->enemies[i].animation);
  }
}

void resetHankCharacterAnimation(HankCharacterData* tCharacterData) {
  resetAnimation(&tCharacterData->animation);
}
