#ifndef PHYSICS_H
#define PHYSICS_H

#include "hank_worlddata.h"
#include "hank_characterdata.h"

void handleHankPhysicsForCharacter(HankWorldData* tWorldData, HankCharacterData* tCharacterData);
void handleHankPhysicsForEnemies(HankWorldData* tWorldData, HankCharacterData* tCharacterData);

#endif
