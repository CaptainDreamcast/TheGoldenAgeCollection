#pragma once

#include <tari/actorhandler.h>

extern ActorBlueprint MirkRouteHandlerBP;

void startNewMirkRoute();
void reloadMirkRoute();
void updateMirkRouteHandler();
void setCurrentMirkRouteToContinue();