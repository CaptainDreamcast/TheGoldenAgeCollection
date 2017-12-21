#pragma once

#include <tari/actorhandler.h>

extern ActorBlueprint NormUserInterface;

int isNormInUIRange(double x);
int isNormBelowUIRange(double x);
void increaseNormUIRange();
void decreaseNormUIRange();