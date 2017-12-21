#ifndef COLLISION_H
#define COLLISION_H

#include "hank_worlddata.h"
#include "hank_characterdata.h"

void checkHankCollisionsPlatforms(HankWorldData* tWorldData, HankCharacterData* tCharacterData);
void checkHankCollisionsEnemies(HankWorldData* tWorldData, HankCharacterData* tCharacterData);
int hasHankLeftScreen(HankWorldData* tWorldData, HankCharacterData* tCharacterData);

#endif
