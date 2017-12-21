#ifndef STAGELOGIC_H
#define STAGELOGIC_H

#include "hank_worlddata.h"
#include "hank_characterdata.h"

void checkHankExit(HankWorldData* tWorldData, HankCharacterData* tCharacterData);
void handleHankScreenTilting(HankWorldData* tWorldData, HankCharacterData* tCharacterData);
void checkHankGameOver(HankWorldData* tWorldData, HankCharacterData* tCharacterData);
HankGameReturnType checkHankGameAbort(HankWorldData* tWorldData, HankCharacterData* tCharacterData);

#endif
