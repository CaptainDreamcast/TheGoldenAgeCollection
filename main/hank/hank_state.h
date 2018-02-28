#ifndef STATE_H
#define STATE_H

#include "hank_characterdata.h"
#include "hank_enemydata.h"

void changeHankCharacterState(HankCharacterData* tCharacterData, HankCharacterState tNewState);
void changeHankEnemyState(HankEnemyData* tEnemyData, HankEnemyState tNewState);

#endif
