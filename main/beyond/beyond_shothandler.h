#pragma once

#include <prism/mugendefreader.h>
#include <prism/actorhandler.h>
#include <prism/physics.h>

void getBeyondShotAngleTowardsPlayer(char* tOutput, void* tCaller);

void getBeyondCurrentSubShotIndex(char* tOutput, void* tCaller);
void addBeyondShot(int tID, int tCollisionList, Position tPosition);
void removeBeyondEnemyShots();
void evaluateBeyondBigBangFunction(char* tDst, void* tCaller);
void evaluateBeyondBounceFunction(char* tDst, void* tCaller);
void evaluateBeyondTextAidFunction(char* tDst, void* tCaller);
void evaluateBeyondAckermannFunction(char* tDst, void* tCaller);
void evaluateBeyondSwirlFunction(char* tDst, void* tCaller);
void evaluateBeyondBlamFunction(char* tDst, void* tCaller);
void evaluateBeyondTransienceFunction(char* tDst, void* tCaller);

int getBeyondFinalBossShotsDeflected();

extern ActorBlueprint BeyondShotHandler;
