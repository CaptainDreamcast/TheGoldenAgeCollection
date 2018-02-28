#include "hank_state.h"

#include <prism/log.h>

#include "hank_animation.h"

void changeHankCharacterState(HankCharacterData* tCharacterData, HankCharacterState tNewState) {
  debugLog("Changing state char");
  debugInteger(tNewState);
  tCharacterData->state = tNewState;
  resetHankCharacterAnimation(tCharacterData);
}

void changeHankEnemyState(HankEnemyData* tEnemyData, HankEnemyState tNewState) {
  debugLog("Changing state enemy");
  debugInteger(tNewState);
  tEnemyData->state = tNewState;
}
