#pragma once

#include <tari/actorhandler.h>
#include <tari/geometry.h>

extern ActorBlueprint NormBackgroundBP;


void wobbleNormAnimation(int tAnimationID, Position tCenter);

Position* getNormGameScreenPositionReference();