#pragma once

#include <tari/actorhandler.h>

extern ActorBlueprint MirkDeathCountBP;

void increaseMirkDeathCount();
int getMirkDeathCount();
void setMirkDeathCountToContinue();

void setMirkDeathCountReal();
void setMirkDeathCountUnreal();
void invertMirkDeathCountReality();
