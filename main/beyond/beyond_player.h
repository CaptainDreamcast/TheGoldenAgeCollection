#pragma once

#include <prism/actorhandler.h>
#include <prism/geometry.h>
#include <prism/physics.h>

extern ActorBlueprint BeyondPlayer;

Position getBeyondPlayerPosition();
PhysicsObject* getBeyondPlayerPhysics();
double getBeyondPlayerAcceleration();
double getBeyondPlayerSpeed();

void resetBeyondPlayerState();
void resetBeyondLocalPlayerCounts();
void getBeyondLocalDeathCountVariable(char* tDst, void* tCaller);
void getBeyondLocalBombCountVariable(char* tDst, void* tCaller);
void setBeyondPlayerToFullPower();

int getBeyondContinueAmount();
void reduceBeyondContinueAmount();

void disableBeyondPlayerBossCollision();

void drainBeyondPlayerPower(void(*tCB)());
void fillBeyondPlayerPower(void(*tCB)());
void burnBeyondPlayer();