#pragma once

#include <tari/actorhandler.h>

extern ActorBlueprint ObjectClockHandler;

void resetObjectClock();
int increaseObjectClockAndReturnIfNewDay();