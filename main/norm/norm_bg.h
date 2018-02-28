#pragma once

#include <prism/actorhandler.h>
#include <prism/geometry.h>

extern ActorBlueprint NormBackgroundBP;


void wobbleNormAnimation(int tAnimationID, Position tCenter);

Position* getNormGameScreenPositionReference();