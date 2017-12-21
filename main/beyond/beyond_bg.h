#pragma once

#include <tari/actorhandler.h>
#include <tari/mugenanimationreader.h>

extern ActorBlueprint BeyondBackgroundHandler;

void setBeyondBackground(char* tPath, MugenSpriteFile* tSprites, MugenAnimations* tOptionalAnimations);
double getBeyondFinalFrontierLevelBackgroundScale();