#include "octo_enemyhandler.h"

#include <prism/datastructures.h>
#include <prism/mugenanimationhandler.h>
#include <prism/physicshandler.h>
#include <prism/math.h>

#include "octo_assethandler.h"
#include "octo_player.h"
#include "octo_collision.h"

typedef struct {
	int mPhysicsID;
	int mAnimationID;
	int mListID;
} ActiveEnemy;

static struct {
	IntMap mEnemies;
} gData;

static void loadEnemyHandler(void* tData) {
	(void)tData;
	gData.mEnemies = new_int_map();
}

static void enemyHitCB(void* tCaller, void* tCollisionData) {
	(void)tCaller;
	(void)tCollisionData;
}

static void addEnemy(Position p, Vector3D tDirection) {
	ActiveEnemy* e = allocMemory(sizeof(ActiveEnemy));
	e->mPhysicsID = addToPhysicsHandler(p);
	addAccelerationToHandledPhysics(e->mPhysicsID, vecScale(tDirection, 2));
	e->mAnimationID = addMugenAnimation(getMugenAnimation(getOctoAnimations(), 50), getOctoSprites(), makePosition(0, 0, 3));
	setMugenAnimationBasePosition(e->mAnimationID, getHandledPhysicsPositionReference(e->mPhysicsID));
	setMugenAnimationCollisionActive(e->mAnimationID, getOctoEnemyCollisionList(), enemyHitCB, e, NULL);

	e->mListID = int_map_push_back_owned(&gData.mEnemies, e);
}

static void unloadEnemy(ActiveEnemy* e) {
	removeMugenAnimation(e->mAnimationID);
	removeFromPhysicsHandler(e->mPhysicsID);
}

static int updateSingleEnemy(void* tCaller, void* tData) {
	(void)tCaller;
	ActiveEnemy* e = tData;

	Position p = *getHandledPhysicsPositionReference(e->mPhysicsID);

	if (p.x < -200 || p.x > 840 || p.y < -200 || p.y >= 680) {
		unloadEnemy(e);
		return 1;
	}

	return 0;
}

static void updateEnemies() {
	int_map_remove_predicate(&gData.mEnemies, updateSingleEnemy, NULL);
}

static void updateAddingEnemies() {
	double poss = randfrom(0, 1);

	if (poss < 0.01)
	{
		int val = randfromInteger(0, 3);
		double other = randfrom(0, 1);

		double x, y;

		if (val == 0 || val == 2)
		{
			y = other * 480;
			if (val == 0) x = 740;
			else x = -100;
		}
		else
		{
			x = other * 640;
			if (val == 0) y = 580;
			else y = -100;
		}

		Position pos = makePosition(x, y, 0);
		Position aim = getOctoPlayerPosition();
		Vector3D direction = vecSub(aim, pos);
		
		double l = vecLength(direction);
		if (l == 0) { l = 1; direction.x = 1; }
		direction = vecNormalize(direction);

		if (l > 200) {
			addEnemy(pos, direction);
		}
	}
}

static void updateEnemyHandler(void* tData) {
	(void)tData;

	updateEnemies();
	updateAddingEnemies();
}


ActorBlueprint OctoEnemyHandler = {
	.mLoad = loadEnemyHandler,
	.mUpdate = updateEnemyHandler,
};