#pragma once

#include "hank_worlddata.h"
#include "hank_characterdata.h"

void handleHankCharacterAnimation(HankWorldData* tWorldData, HankCharacterData* tCharacterData);
void handleHankEnemyAnimation(HankWorldData* tWorldData, HankCharacterData* tCharacterData);

void resetHankCharacterAnimation(HankCharacterData* tCharacterData);

