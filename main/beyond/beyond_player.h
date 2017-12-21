#pragma once

#include <tari/actorhandler.h>
#include <tari/geometry.h>
#include <tari/physics.h>

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