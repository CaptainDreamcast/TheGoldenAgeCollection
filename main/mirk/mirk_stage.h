#pragma once

#include <prism/actorhandler.h>
#include <prism/geometry.h>
#include <prism/drawing.h>

extern ActorBlueprint MirkStageBP;

Position* getMirkStagePositionReference();

void drawMirkBloodOnStage(Position p, Color c);
void increaseMirkScreenShake();

void resetMirkScreenShakeLimit();
void invertMirkStageReality();

void setMirkStageReal();
void setMirkStageUnreal();
