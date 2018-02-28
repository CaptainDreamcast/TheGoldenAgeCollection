#pragma once

#include <prism/actorhandler.h>
#include <prism/mugenanimationreader.h>

extern ActorBlueprint BeyondBossHandler;
void  loadBeyondBossFromDefinitionPath(char * tDefinitionPath, MugenAnimations* tAnimations, MugenSpriteFile* tSprites);
void goToBeyondStoryScreen(void* tCaller);
void activateBeyondBoss();
void damageBeyondBoss();

void fetchBeyondBossTimeVariable(char* tDst, void* tCaller);
int isBeyondBossActive();
Position getBeyondBossPosition();
void evaluateBeyondTextAidFunction(char * tDst, void * tCaller);
void addFinalBeyondBossShot(int mID);
void setFinalBeyondBossInvincible();
void setFinalBeyondBossVulnerable();
