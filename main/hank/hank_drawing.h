#ifndef DRAWING_H
#define DRAWING_H

#include "hank_worlddata.h"
#include "hank_characterdata.h"
#include "hank_titledata.h"

void drawHank(HankWorldData* tWorldData, HankCharacterData* tCharacterData);
void updateHankTiltingMatrix(double tTiltAngle);

#endif

