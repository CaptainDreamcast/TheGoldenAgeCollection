#include "storm_enemyhandler.h"

#include <stdio.h>

#include <prism/datastructures.h>
#include <prism/physicshandler.h>
#include <prism/collisionhandler.h>
#include <prism/memoryhandler.h>
#include <prism/math.h>
#include <prism/timer.h>

#include "storm_collision.h"
#include "vectrexanimations.h"
#include "storm_particlehandler.h"
#include "storm_ui.h"

typedef struct {
	int mPhysicsID;
	int mCollisionID;
	int mListID;
} ActiveEnemy;

static struct {
	List mEnemies;
	JusticeCollisionData mCollisionData;

	double mEnemySpeedX;
} gData;

static void finalIncreaseSpeedCB(void* tCaller) {
	(void)tCaller;
	gData.mEnemySpeedX++;
	addTimerCB(60*30, finalIncreaseSpeedCB, NULL);
}

static void increaseSpeedCB(void* tCaller) {
	(void)tCaller;
	gData.mEnemySpeedX++;
}

static void loadEnemyHandler(void* tData) {
	(void)tData;
	gData.mEnemies = new_list();
	gData.mCollisionData.mListID = getStormEnemyCollisionList();
	gData.mEnemySpeedX = 1;

	addTimerCB(60 * 5, increaseSpeedCB, NULL);
	addTimerCB(60 * 10, increaseSpeedCB, NULL);
	addTimerCB(60 * 30, finalIncreaseSpeedCB, NULL);
}

static void unloadEnemy(ActiveEnemy* e) {
	removeFromCollisionHandler(getStormEnemyCollisionList(), e->mCollisionID);
	removeFromPhysicsHandler(e->mPhysicsID);
}

static void addEnemyLines(ActiveEnemy* e) {
	Position p = *getHandledPhysicsPositionReference(e->mPhysicsID);
	
	int i;
	for (i = 0; i < 2; i++) {
		addVectrexOneFrameLine(p, randfrom(M_PI / 4, (M_PI * 3) / 4));
	}

}

static int updateSingleEnemy(void* tCaller, void* tData) {
	(void)tCaller;
	ActiveEnemy* e = tData;

	addEnemyLines(e);

	Position pos = *getHandledPhysicsPositionReference(e->mPhysicsID);
	if (pos.x <= 150) {
		increaseStormEnemiesMissed();
		unloadEnemy(e);
		return 1;
	}

	return 0;
}

static void enemyHitCB(void* tCaller, void* tCollisionData) {
	(void)tCollisionData;
	ActiveEnemy* e = tCaller;

	Position pos = *getHandledPhysicsPositionReference(e->mPhysicsID);
	addStormParticleSpread(pos);
	increaseStormScore();

	unloadEnemy(e);
	list_remove(&gData.mEnemies, e->mListID);
}

static void addNewEnemy() {
	ActiveEnemy* e = allocMemory(sizeof(ActiveEnemy));
	e->mPhysicsID = addToPhysicsHandler(makePosition(500, randfrom(150, 175), 3));
	addAccelerationToHandledPhysics(e->mPhysicsID, makePosition(-gData.mEnemySpeedX, 0, 0));

	CollisionRect rect = makeCollisionRect(makePosition(-8, -20, 0), makePosition(8, 0, 0));
	e->mCollisionID = addCollisionRectangleToCollisionHandler(getStormEnemyCollisionList(), getHandledPhysicsPositionReference(e->mPhysicsID), rect, enemyHitCB, e, &gData.mCollisionData);

	e->mListID = list_push_back_owned(&gData.mEnemies, e);
}

static void updateEnemies() {
	list_remove_predicate(&gData.mEnemies, updateSingleEnemy, NULL);
}

static void updateAddingEnemies() {
	double r = randfrom(0, 1);

	if (r < 0.02) {	
		addNewEnemy();
	}
}

static void updateEnemyHandler(void* tData) {
	(void)tData;

	updateEnemies();
	updateAddingEnemies();
}

ActorBlueprint StormEnemyHandler = {
	.mLoad = loadEnemyHandler,
	.mUpdate = updateEnemyHandler,
};