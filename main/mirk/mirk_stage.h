#pragma once

#include <tari/actorhandler.h>
#include <tari/geometry.h>
#include <tari/drawing.h>

extern ActorBlueprint MirkStageBP;

Position* getMirkStagePositionReference();

void drawMirkBloodOnStage(Position p, Color c);
void increaseMirkScreenShake();

void resetMirkScreenShakeLimit();
void invertMirkStageReality();

void setMirkStageReal();
void setMirkStageUnreal();
