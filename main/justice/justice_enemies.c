#include "justice_enemies.h"

#include <math.h>

#include <tari/datastructures.h>
#include <tari/animation.h>
#include <tari/texture.h>
#include <tari/collision.h>
#include <tari/script.h>
#include <tari/memoryhandler.h>
#include <tari/physicshandler.h>
#include <tari/collisionhandler.h>
#include <tari/system.h>
#include <tari/math.h>
#include <tari/file.h>
#include <tari/collisionanimation.h>
#include <tari/timer.h>

#include <tari/log.h>


#include "justice_collision.h"
#include "justice_player.h"
#include "justice_userinterface.h"
#include "justice_sound.h"

typedef enum {

	STATE_IDLE,
	STATE_WALKING,
	STATE_HIT,
	STATE_DEATH,
	STATE_PUNCH

} EnemyState;

typedef struct {
	Animation idleAnimation;
	TextureData idleTextures[10];

	Animation walkingAnimation;
	TextureData walkingTextures[10];

	Animation deathAnimation;
	TextureData deathTextures[20];

	Animation hitAnimation;
	TextureData hitTextures[10];

	Animation punchAnimation;
	TextureData punchTextures[10];

	JusticeCollisionData punchCollisionData;
	CollisionAnimation punchCollisionAnimation;
	
	int health;
	Collider col;

	double maxVelocity;
	Vector3D dragCoefficient;
	
	Position center;
	double speed;

	int hasFallThroughDeath;

} EnemyType;

typedef struct {

	int id;

	int physicsID;
	int collisionID;
	int animationID;
	int shadowID;
	int health;
	int type;
	int getHitFromID;
	JusticeCollisionData collisionData;
	JusticeCollisionData punchCollisionData;

	int direction;
	int collisionAnimationID;
	int isAllowedToPunch;

	Position target;
	Position* position;
	
	Velocity* velocity;

	EnemyState state;

	Position static128Position;

} ActiveEnemy;

static struct {
	Vector enemyTypes;
	List activeEnemies;
	Position* screenPositionReference;

	int areFrozen;
} gData;

static ScriptPosition loadSingleEnemyTypeAnimation(ScriptPosition pos, Animation* animation, TextureData* textureData) {
	*animation = createEmptyAnimation();
	int v;	
	pos = getNextScriptInteger(pos, &v);
	animation->mFrameAmount = v;
	pos = getNextScriptInteger(pos, &v);
	animation->mDuration = v;
	
	int i;
	for(i = 0; i < (int)animation->mFrameAmount; i++) {
		char file[100];
		char path[100];
		pos = getNextScriptString(pos, file);
		sprintf(path, "sprites/%s", file);
		textureData[i] = loadTexture(path);
	}

	return pos;
}

static ScriptPosition loadSingleEnemyType(void* caller, ScriptPosition pos) {
	char word[100];
	EnemyType* enemyType = caller;

	pos = getNextScriptString(pos, word);
	if(!strcmp(word, "IDLE_ANIMATION")) {
		pos = loadSingleEnemyTypeAnimation(pos, &enemyType->idleAnimation, enemyType->idleTextures);
	} else if(!strcmp(word, "DEATH_ANIMATION")) {
		pos = loadSingleEnemyTypeAnimation(pos, &enemyType->deathAnimation, enemyType->deathTextures);
	} else if(!strcmp(word, "WALKING_ANIMATION")) {
		pos = loadSingleEnemyTypeAnimation(pos, &enemyType->walkingAnimation, enemyType->walkingTextures);
	} else if(!strcmp(word, "HIT_ANIMATION")) {
		pos = loadSingleEnemyTypeAnimation(pos, &enemyType->hitAnimation, enemyType->hitTextures);
	}  else if(!strcmp(word, "PUNCH_ANIMATION")) {
		pos = loadSingleEnemyTypeAnimation(pos, &enemyType->punchAnimation, enemyType->punchTextures);
	} else if(!strcmp(word, "PUNCH_COLLISION_DATA")) {
		int strength;
		Acceleration force;
		pos = getNextScriptInteger(pos, &strength);
		pos = getNextScriptDouble(pos, &force.x);
		pos = getNextScriptDouble(pos, &force.y);
		force.z = 0;
		enemyType->punchCollisionData = makeJusticePunchCollisionData(strength, force);
	} else if(!strcmp(word, "PUNCH_COLLISION_ANIMATION")) {
		pos = loadJusticePunchCollisionAnimation(pos, &enemyType->punchCollisionAnimation, enemyType->punchAnimation);
	} else if(!strcmp(word, "HEALTH")) {
		pos = getNextScriptInteger(pos, &enemyType->health);
	} else if(!strcmp(word, "COLLISION")) {
		CollisionRect rect;
		pos = getNextScriptDouble(pos, &rect.mTopLeft.x);
		pos = getNextScriptDouble(pos, &rect.mTopLeft.y);
		pos = getNextScriptDouble(pos, &rect.mTopLeft.z);
		pos = getNextScriptDouble(pos, &rect.mBottomRight.x);
		pos = getNextScriptDouble(pos, &rect.mBottomRight.y);
		pos = getNextScriptDouble(pos, &rect.mBottomRight.z);
		enemyType->col = makeColliderFromRect(rect);
	} else if(!strcmp(word, "MAX_VELOCITY")) {
		pos = getNextScriptDouble(pos, &enemyType->maxVelocity);
	} else if(!strcmp(word, "DRAG_COEFFICIENT")) {
		pos = getNextScriptDouble(pos, &enemyType->dragCoefficient.x);
		pos = getNextScriptDouble(pos, &enemyType->dragCoefficient.y);
		enemyType->dragCoefficient.z = 0;
	} else if(!strcmp(word, "CENTER_X")) {
		pos = getNextScriptDouble(pos, &enemyType->center.x);
		enemyType->center.y = 0;
		enemyType->center.z = 0;
	}  else if(!strcmp(word, "SPEED")) {
		pos = getNextScriptDouble(pos, &enemyType->speed);
	} else if(!strcmp(word, "FALL_THROUGH_DEATH")) {
		enemyType->hasFallThroughDeath = 1;
	} else {
		logError("Unrecognized token");
		logErrorString(word);
		abortSystem();
	}

	return pos;
}

static ScriptPosition loader(void* caller, ScriptPosition pos) {
	

	char word[100];
	pos = getNextScriptString(pos, word);

	if(!strcmp(word, "ENEMY_TYPE")) {
		ScriptRegion enemyRegion = getScriptRegionAtPosition(pos);
		EnemyType* enemyType = allocMemory(sizeof(EnemyType));
		enemyType->hasFallThroughDeath = 0;
		executeOnScriptRegion(enemyRegion, loadSingleEnemyType, enemyType);
		vector_push_back_owned(&gData.enemyTypes, enemyType);
		pos = getPositionAfterScriptRegion(pos.mRegion, enemyRegion);
	} else {
		logError("Unrecognized token");
		logErrorString(word);
		abortSystem();
	}


	return pos;
}

static void loadEnemyTypes() {
	Script s = loadScript("scripts/enemies.txt");
	ScriptRegion r = getScriptRegion(s, "LOAD");

	setWorkingDirectory("/assets/main/justice/enemies/"); // TODO: fix this whole cwd mess
	executeOnScriptRegion(r, loader, NULL);
}


void loadJusticeEnemies() {
	gData.enemyTypes = new_vector();
	gData.activeEnemies = new_list();

	gData.areFrozen = 0;

	loadEnemyTypes();
}

static void invert(ActiveEnemy* enemy) {
	enemy->direction *= -1;
	inverseAnimationVertical(enemy->animationID);
	if(enemy->collisionAnimationID != -1) {
		invertCollisionAnimationVertical(enemy->collisionAnimationID);
	}
}

static void setIdle(ActiveEnemy* enemy) {
	EnemyType* enemyType = vector_get(&gData.enemyTypes, enemy->type);

	changeAnimation(enemy->animationID, enemyType->idleTextures, enemyType->idleAnimation, makeRectangleFromTexture(enemyType->idleTextures[0]));
	removeAnimationCB(enemy->animationID);
	enemy->state = STATE_IDLE;
}

static void setWalking(ActiveEnemy* enemy) {
	EnemyType* enemyType = vector_get(&gData.enemyTypes, enemy->type);

	changeAnimation(enemy->animationID, enemyType->walkingTextures, enemyType->walkingAnimation, makeRectangleFromTexture(enemyType->walkingTextures[0]));
	removeAnimationCB(enemy->animationID);
	enemy->state = STATE_WALKING;
}

static void checkStartRandomWalk(ActiveEnemy* enemy) {
	Position targetDelta = makePosition(randfrom(-100, 100), randfrom(-100, 100), 0);
	Position pPosition = getJusticePlayerPosition();
	Position playerDelta = vecNormalize(vecAdd(pPosition, vecScale(enemy->static128Position, -1)));
	Position totalDelta = vecAdd(targetDelta, vecScale(playerDelta, 10));

	enemy->target = vecAdd(enemy->static128Position, totalDelta);

	setWalking(enemy);
}

static double positionDistance(Position p1, Position p2) {
		p1.z = p2.z;
		return vecLength(vecAdd(p1, vecScale(p2, -1)));
}

static void checkWalking(ActiveEnemy* enemy) {
	EnemyType* enemyType = vector_get(&gData.enemyTypes, enemy->type);

	if(positionDistance(enemy->static128Position, enemy->target) < 3) {
		setIdle(enemy);
	} else {
		Position delta = vecNormalize(vecAdd(enemy->target, vecScale(enemy->static128Position, -1)));
		delta.z = 0;
		addAccelerationToHandledPhysics(enemy->physicsID, vecScale(delta, enemyType->speed));
	}
}

static void checkInversion(ActiveEnemy* enemy) {
	if(enemy->direction == 1 && enemy->velocity->x < 0) invert(enemy);
	else if(enemy->direction == -1 && enemy->velocity->x > 0) invert(enemy);
}

static void checkRandomWalk(ActiveEnemy* enemy) {
	if(enemy->state == STATE_IDLE) {
		checkStartRandomWalk(enemy);
	} else if(enemy->state == STATE_WALKING) {
		checkWalking(enemy);
		checkInversion(enemy);
	}

}

static void setPunchingAllowed(void* caller) {
	ActiveEnemy* enemy = caller;
	enemy->isAllowedToPunch = 1;
}

static void punchFinished(void* caller){	
	ActiveEnemy* enemy = caller;
	if(enemy->state != STATE_PUNCH) return;
	enemy->collisionAnimationID = -1;
	setIdle(enemy);

	enemy->isAllowedToPunch = 0;
	addTimerCB(60, setPunchingAllowed, enemy);
}

static void punchHitSomething(void* caller, void* collisionData) {

}

static void setPunch(ActiveEnemy* enemy) {
	EnemyType* enemyType = vector_get(&gData.enemyTypes, enemy->type);

	enemy->state = STATE_PUNCH;
	changeAnimation(enemy->animationID, enemyType->punchTextures, enemyType->punchAnimation, makeRectangleFromTexture(enemyType->punchTextures[0]));
	setAnimationCB(enemy->animationID, punchFinished, enemy);
	enemy->punchCollisionData = enemyType->punchCollisionData;
	updateJusticeCollisionDataID(&enemy->punchCollisionData);
	enemy->collisionAnimationID = addHandledCollisionAnimation(getJusticeEnemyAttackCollisionListID(), enemy->position, enemyType->punchCollisionAnimation, punchHitSomething, enemy, &enemy->punchCollisionData);
	setCollisionAnimationCenter(enemy->collisionAnimationID, enemyType->center);
	if(enemy->direction == -1) {
		invertCollisionAnimationVertical(enemy->collisionAnimationID);
	}
}

static int isInRangeToHitPlayer(ActiveEnemy* enemy) {
	Position p = getJusticePlayerPosition();
	Position e = enemy->static128Position;

	if(fabs(p.x - e.x) > 100) return 0;
	if(fabs(p.y - e.y) > 40) return 0;
	if(p.x > e.x && enemy->direction == -1) return 0;
	if(p.x < e.x && enemy->direction == 1) return 0;

	return 1;
	
}

static void checkPunch(ActiveEnemy* enemy) {
	if(!enemy->isAllowedToPunch) return;
	if(enemy->state != STATE_IDLE && enemy->state != STATE_WALKING) return;	

	if(isInRangeToHitPlayer(enemy)) {
		setPunch(enemy);
	}
}

static void updatePositionConstraints(ActiveEnemy* enemy) {
	EnemyType* enemyType = vector_get(&gData.enemyTypes, enemy->type);

	enemy->static128Position = *enemy->position;
	enemy->static128Position.y += enemyType->idleTextures[0].mTextureSize.y - 256;
	adjustJusticeZ(&enemy->static128Position);
	enemy->position->z = enemy->static128Position.z;

	constraintJusticeIntoLevel(&enemy->target, gData.screenPositionReference);	
}

static void updateSingleEnemy(void* caller, void* data) {
	(void) caller;
	ActiveEnemy* enemy = data;
	updatePositionConstraints(enemy);	
	if(gData.areFrozen) return;

	checkRandomWalk(enemy);
	checkPunch(enemy); 
	
}

void updateJusticeEnemies() {
	list_map(&gData.activeEnemies, updateSingleEnemy, NULL);
}



int getActiveJusticeEnemyAmount() {
	return list_size(&gData.activeEnemies);
}


static void removeActiveEnemy(ActiveEnemy* enemy) {
	
	removeHandledAnimation(enemy->animationID);
	removeFromCollisionHandler(getJusticeEnemyCollisionListID(), enemy->collisionID);
	removeFromPhysicsHandler(enemy->physicsID);
	removeJusticeShadow(enemy->shadowID);
	list_remove(&gData.activeEnemies, enemy->id);
}

static void dyingOver(void* tCaller) {
	ActiveEnemy* enemy = tCaller;
	removeActiveEnemy(enemy);
}

static void die(ActiveEnemy* enemy) {
	if(enemy->state == STATE_DEATH) return;
	
	EnemyType* enemyType = vector_get(&gData.enemyTypes, enemy->type);

	changeAnimation(enemy->animationID, enemyType->deathTextures, enemyType->deathAnimation, makeRectangleFromTexture(enemyType->deathTextures[0]));
	setAnimationCB(enemy->animationID, dyingOver, enemy);

	if(enemyType->hasFallThroughDeath) {
		setHandledPhysicsGravity(enemy->physicsID, makePosition(0, 1, 0));
		setHandledPhysicsMaxVelocity(enemy->physicsID, INF);
		setHandledPhysicsDragCoefficient(enemy->physicsID, makePosition(0,0,0));
		addAccelerationToHandledPhysics(enemy->physicsID, makePosition(0, -20, 0));
	}

	enemy->state = STATE_DEATH;
}

static void gettingHitOver(void* tCaller) {
	ActiveEnemy* enemy = tCaller;
	setIdle(enemy);
}

static void getHit(ActiveEnemy* enemy) {
	EnemyType* enemyType = vector_get(&gData.enemyTypes, enemy->type);

	changeAnimation(enemy->animationID, enemyType->hitTextures, enemyType->hitAnimation, makeRectangleFromTexture(enemyType->hitTextures[0]));
	setAnimationCB(enemy->animationID, gettingHitOver, enemy);
	enemy->state = STATE_HIT;
}

static void enemyHitCB(void* tCaller, void* tCollisionData) {
	ActiveEnemy* enemy = tCaller;
	JusticeCollisionData* cData = tCollisionData;

	if(enemy->getHitFromID == cData->id) return;

	if(enemy->state != STATE_DEATH) playJusticeHitSoundEffect();	

	enemy->getHitFromID = cData->id;
	enemy->health -= cData->strength;

	if(enemy->health <= 0) {
		die(enemy);
	} else {

		addAccelerationToHandledPhysics(enemy->physicsID, cData->force);
		getHit(enemy);
	}

	if(enemy->collisionAnimationID != -1) {
		removeHandledCollisionAnimation(enemy->collisionAnimationID);
		enemy->collisionAnimationID = -1;
	}
}


void spawnJusticeEnemy(int type, Position pos) {
	EnemyType* enemyType = vector_get(&gData.enemyTypes, type);

	ActiveEnemy* enemy = allocMemory(sizeof(ActiveEnemy));

	enemy->health = enemyType->health;
	enemy->physicsID = addToPhysicsHandler(pos);
	enemy->type = type;
	enemy->getHitFromID = -1;

	enemy->direction = 1;
	enemy->collisionAnimationID = -1;
	enemy->isAllowedToPunch = 1;
	
	setHandledPhysicsMaxVelocity(enemy->physicsID, enemyType->maxVelocity);
	setHandledPhysicsDragCoefficient(enemy->physicsID, enemyType->dragCoefficient);

	PhysicsObject* physics = getPhysicsFromHandler(enemy->physicsID);
	enemy->position = &physics->mPosition;
	enemy->velocity = &physics->mVelocity;
	enemy->target = *enemy->position;
	enemy->static128Position = *enemy->position;

	enemy->collisionData = makeHittableJusticeCollisionData();
	enemy->collisionID = addColliderToCollisionHandler(getJusticeEnemyCollisionListID(), enemy->position, enemyType->col, enemyHitCB, enemy, &enemy->collisionData);
	enemy->punchCollisionData = enemyType->punchCollisionData;
	double shadowScaleX = 1;	
	if(enemyType->idleTextures[0].mTextureSize.x == 512) shadowScaleX = 3;
	enemy->shadowID = addJusticeShadow(&enemy->static128Position, enemyType->center, shadowScaleX);

	enemy->state = STATE_IDLE;
	enemy->animationID = playAnimationLoop(makePosition(0,0,0), enemyType->idleTextures, enemyType->idleAnimation, makeRectangleFromTexture(enemyType->idleTextures[0]));
	setAnimationBasePositionReference(enemy->animationID, enemy->position);
	setAnimationScreenPositionReference(enemy->animationID, gData.screenPositionReference);
	setAnimationCenter(enemy->animationID, enemyType->center);

	Position playerPosition = getJusticePlayerPosition();
	if(playerPosition.x < enemy->position->x) invert(enemy);

	enemy->id = list_push_front_owned(&gData.activeEnemies, enemy);
}

void setJusticeEnemiesScreenPositionReference(Position* p) {
	gData.screenPositionReference = p;
}


void freezeJusticeEnemies() {
	gData.areFrozen = 1;
}

void unfreezeJusticeEnemies() {
	gData.areFrozen = 0;
}

