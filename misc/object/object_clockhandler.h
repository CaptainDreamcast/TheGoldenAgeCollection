#pragma once

#include <prism/actorhandler.h>

extern ActorBlueprint ObjectClockHandler;

void resetObjectClock();
int increaseObjectClockAndReturnIfNewDay();