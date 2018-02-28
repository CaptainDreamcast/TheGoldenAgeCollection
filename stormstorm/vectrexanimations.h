#pragma once

#include <prism/actorhandler.h>
#include <prism/physics.h>
#include <prism/animation.h>

extern ActorBlueprint VectrexAnimationHandler;

int addVectrexImage(char* tPath, Position tPosition);
void setVectrexImageBasePosition(int tID, Position* tBasePosition);
void addVectrexOneFrameLine(Position tPosition, double tAngle);
void addVectrexOneFrameDot(Position tPosition);
void addVectrexOneFrameImage(TextureData* tTextures, Position tPosition);