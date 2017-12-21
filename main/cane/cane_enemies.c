#include "cane_enemies.h"

#include <math.h>

#include <tari/texture.h>
#include <tari/file.h>
#include <tari/datastructures.h>
#include <tari/math.h>
#include <tari/memoryhandler.h>
#include <tari/physicshandler.h>
#include <tari/collisionhandler.h>
#include <tari/mugenanimationhandler.h>

#include "cane_collision.h"
#include "cane_manualenemy.h"
#include "cane_resources.h"

typedef struct {
	int physicsID;
} DeadEnemy;

typedef struct {
	int animationID;
	int physicsID;
	int collisionID;

	Position* pos;
	Velocity* vel;

	double scale;
	double rotation;

	CaneCollisionData colData;

} Enemy;

static struct {
	List enemies;

	double z;

	double enemyCreationProb;
	double enemyMaxScale;
} gData;

void loadCaneEnemies() {

	gData.z = 4;
	gData.enemyCreationProb = 2;
	gData.enemyMaxScale = 1;

	gData.enemies = new_list();
}

static void removeEnemy(Enemy* e) {
	removeFromPhysicsHandler(e->physicsID);
	removeMugenAnimation(e->animationID);
	removeFromCollisionHandler(getCaneEnemyCollisionListID(), e->collisionID);
}

static void enemyHitCB(void* caller, void* collisionData);

void addCaneEnemy(Position pos, Velocity vel, double scale) {
	Enemy* e = allocMemory(sizeof(Enemy));

	e->physicsID = addToPhysicsHandler(pos);
	PhysicsObject* physics = getPhysicsFromHandler(e->physicsID);
	e->pos = &physics->mPosition;
	e->vel = &physics->mVelocity;
	addAccelerationToHandledPhysics(e->physicsID, vel);
	setHandledPhysicsGravity(e->physicsID, makePosition(0, 0.07, 0));

	e->animationID = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 7), getCaneSprites(), makePosition(0, 0, 0));
	setMugenAnimationBasePosition(e->animationID, e->pos);

	e->scale = scale;
	e->rotation = randfrom(0, M_PI);

	Position center = makePosition(0, 0, 0);
	setMugenAnimationBaseDrawScale(e->animationID, scale);
	setMugenAnimationDrawAngle(e->animationID, e->rotation);

	e->colData.listID = getCaneEnemyCollisionListID();
	e->colData.direction = e->vel;
	e->collisionID = addColliderToCollisionHandler(getCaneEnemyCollisionListID(), e->pos, makeColliderFromCirc(makeCollisionCirc(center, 64 * e->scale)), enemyHitCB, e, &e->colData);

	list_push_back_owned(&gData.enemies, e);
}

static void splitupEnemy(Enemy* e, CaneCollisionData* c) {
	Position baseP = *e->pos;
	Velocity baseEnemyShot = normalizeVelocity(*e->vel);
	Velocity basePlayerShot = normalizeVelocity(*c->direction);
	Velocity baseV = normalizeVelocity(vecAdd(vecScale(baseEnemyShot, 1), vecScale(basePlayerShot, 0)));

	double speed = vecLength(*e->vel)*0.5;
	baseV = vecScale(baseV, speed);

	Velocity v1 = vecRotateZ(baseV, degreesToRadians(90));
	Velocity v2 = vecRotateZ(baseV, degreesToRadians(-90));

	if (e->scale < 0.4) return;

	addCaneEnemy(baseP, v1, e->scale / 2);
	addCaneEnemy(baseP, v2, e->scale / 2);
}

static void enemyHitCB(void* caller, void* collisionData) {
	Enemy* e = caller;
	CaneCollisionData* c = collisionData;

	if (c->listID == getCanePlayerCollisionListID()) return;

	splitupEnemy(e, c);
	e->pos->x = -1000;
	e->pos->y = 1000;
}

static void createNewEnemy() {
	
	double x = randfrom(-10, 700);

	double scale = randfrom(0.5, gData.enemyMaxScale);
	double y = -150*scale;

	double dx = randfrom(-3, 3);
	double dy = randfrom(0.1, 5);

	Position pos = makePosition(x, y, gData.z);
	Velocity vel = makePosition(dx, dy, 0);

	addCaneEnemy(pos, vel, scale);
}

static void handleEnemyCreation() {
	if (isCaneManual()) return;

	double prob = randfrom(0,100);

	if (prob > gData.enemyCreationProb) {
		gData.enemyCreationProb += 0.0005;
		gData.enemyCreationProb = min(gData.enemyCreationProb, 5);
		gData.enemyMaxScale += 0.0001;
		gData.enemyMaxScale = min(gData.enemyMaxScale, 4);
		return;
	}
	createNewEnemy();
}

static void removeDeadEnemy(void* caller) {
	DeadEnemy* e = caller;
	removeFromPhysicsHandler(e->physicsID);
	freeMemory(e);
}

static void playEnemyDeathAnimation(Position p, double scale) {
	DeadEnemy* e = allocMemory(sizeof(DeadEnemy));

	e->physicsID = addToPhysicsHandler(p);
	addAccelerationToHandledPhysics(e->physicsID, makePosition(-20, 0, 0));
	Position* pos = &getPhysicsFromHandler(e->physicsID)->mPosition;

	int id = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 8), getCaneSprites(), makePosition(0, 0, 0));
	setMugenAnimationBaseDrawScale(id, scale);
	setMugenAnimationBasePosition(id, pos);
	setMugenAnimationNoLoop(id);
	setMugenAnimationCallback(id, removeDeadEnemy, e);
}

static int handleSingleEnemyUpdate(void* caller, void* data) {
	Enemy* e = data;
	
	e->rotation += (1 / e->scale) * 0.1;
	setMugenAnimationDrawAngle(e->animationID, e->rotation);

	Position p = *e->pos;
	double scale = e->scale;

	if (p.x < -200 || p.y < -700 || p.x > 700 || p.y > 428) {
		removeEnemy(e);
		if (p.y > 428) {
			playEnemyDeathAnimation(p, scale);
		}
		return 1;
	}

	return 0;
}

static void handleEnemyUpdate() {
	list_remove_predicate(&gData.enemies, handleSingleEnemyUpdate, NULL);
}

void updateCaneEnemies() {
	handleEnemyCreation();

	handleEnemyUpdate();
}
