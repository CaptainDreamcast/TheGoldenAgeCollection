#pragma once

#include <tari/actorhandler.h>

extern ActorBlueprint BeyondLevelHandler;

void setBeyondLevelToStart();
void goToNextBeyondLevel();
void fetchBeyondStagePartTime(char* tDst, void* tCaller);
void advanceBeyondStagePart();
int getBeyondCurrentLevel();
int getBeyondStagePart();