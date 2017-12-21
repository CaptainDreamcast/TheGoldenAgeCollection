#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "hank_worlddata.h"
#include "hank_characterdata.h"

void checkHankJumpingCharacter(HankWorldData* tWorldData, HankCharacterData* tCharacterData);
void checkHankRunningCharacter(HankWorldData* tWorldData, HankCharacterData* tCharacterData);
void checkHankMovementEnemies(HankWorldData* tWorldData, HankCharacterData* tCharacterData);

#endif
