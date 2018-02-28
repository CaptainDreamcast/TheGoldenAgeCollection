#include "justice_collision.h"

#include <stdio.h>
#include <stdlib.h>

#include <prism/collisionhandler.h>
#include <prism/math.h>
#include <prism/memoryhandler.h>

static struct {

	int enemyCollisionList;
	int enemyAttackCollisionList;
	int playerCollisionList;
	int playerAttackCollisionList;
	int gIDs;

} gData;

void loadJusticeCollision() {
	gData.enemyCollisionList = addCollisionListToHandler();
	gData.playerCollisionList = addCollisionListToHandler();
	gData.enemyAttackCollisionList = addCollisionListToHandler();
	gData.playerAttackCollisionList = addCollisionListToHandler();

	addCollisionHandlerCheck(gData.enemyCollisionList, gData.playerAttackCollisionList);
	addCollisionHandlerCheck(gData.playerCollisionList, gData.enemyAttackCollisionList);
}

JusticeCollisionData makeHittableJusticeCollisionData() {
	JusticeCollisionData ret;
	ret.force = makePosition(0,0,0);
	ret.strength = 0;
	ret.id = gData.gIDs++;
	return ret;
}

JusticeCollisionData makeJusticePunchCollisionData(int strength, Acceleration force) {
	JusticeCollisionData ret;
	ret.force = force;
	ret.strength = strength;
	ret.id = gData.gIDs++;
	return ret;
}

int getJusticeEnemyCollisionListID() {
	return gData.enemyCollisionList;
}

int getJusticeEnemyAttackCollisionListID() {
	return gData.enemyAttackCollisionList;
}

int getJusticePlayerCollisionListID() {
 	return gData.playerCollisionList;
}

int getJusticePlayerAttackCollisionListID() {
	return gData.playerAttackCollisionList;
}

void updateJusticeCollisionDataID(JusticeCollisionData* cData) {
	cData->id = gData.gIDs++;
}

void constraintJusticeIntoLevel(Position* p, Position* screenPositionReference) {
	*p = vecAdd(*p, vecScale(*screenPositionReference, -1));
	p->x = max(p->x, -110);
	p->x = min(p->x, 640-128);
	p->y = max(p->y, 40);
	p->y = min(p->y, 480-256);
	*p = vecAdd(*p, *screenPositionReference);
}

void adjustJusticeZ(Position* p) {
	double t = getLinearInterpolationFactor(0, 480, p->y);
	p->z = interpolateLinear(3, 4, t);
}

ScriptPosition loadSingleJusticeCollisionAnimation(void* caller, ScriptPosition position) {
	CollisionAnimation* cAnimation = caller;

	Position topLeft;
	Position bottomRight;
	position = getNextScriptDouble(position, &topLeft.x);
	position = getNextScriptDouble(position, &topLeft.y);
	position = getNextScriptDouble(position, &topLeft.z);
	position = getNextScriptDouble(position, &bottomRight.x);
	position = getNextScriptDouble(position, &bottomRight.y);
	position = getNextScriptDouble(position, &bottomRight.z);

	CollisionRect rect = makeCollisionRect(topLeft, bottomRight);
	Collider* col = allocMemory(sizeof(Collider));
	*col = makeColliderFromRect(rect);
	vector_push_back_owned(&cAnimation->mFrames, col);

	return position;
}

ScriptPosition loadJusticePunchCollisionAnimation(ScriptPosition position, CollisionAnimation* collisionAnimation, Animation animation) {
	ScriptRegion collisionRegion = getScriptRegionAtPosition(position);
	*collisionAnimation = makeEmptyCollisionAnimation();
	executeOnScriptRegion(collisionRegion, loadSingleJusticeCollisionAnimation, collisionAnimation);
	collisionAnimation->mAnimation = animation;
	position = getPositionAfterScriptRegion(position.mRegion, collisionRegion);	
	return position;
}
