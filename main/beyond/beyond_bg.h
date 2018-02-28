#pragma once

#include <prism/actorhandler.h>
#include <prism/mugenanimationreader.h>

extern ActorBlueprint BeyondBackgroundHandler;

void setBeyondBackground(char* tPath, MugenSpriteFile* tSprites, MugenAnimations* tOptionalAnimations);
double getBeyondFinalFrontierLevelBackgroundScale();