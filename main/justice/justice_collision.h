#ifndef COLLISION_H
#define COLLISION_H

#include <prism/physics.h>
#include <prism/script.h>
#include <prism/collisionanimation.h>

typedef struct {	
	Acceleration force;
	int strength;
	int id;
} JusticeCollisionData;

void loadJusticeCollision();
JusticeCollisionData makeHittableJusticeCollisionData();
JusticeCollisionData makeJusticePunchCollisionData(int strength, Acceleration force);
int getJusticeEnemyCollisionListID();
int getJusticeEnemyAttackCollisionListID();
int getJusticePlayerCollisionListID();
int getJusticePlayerAttackCollisionListID();
void constraintJusticeIntoLevel(Position* p, Position* screenPositionReference);

void updateJusticeCollisionDataID(JusticeCollisionData* cData);
void adjustJusticeZ(Position* p);

ScriptPosition loadSingleJusticeCollisionAnimation(void* caller, ScriptPosition position);
ScriptPosition loadJusticePunchCollisionAnimation(ScriptPosition position, CollisionAnimation* collisionAnimation, Animation animation);

#endif
