#include "cane_bullet.h"

#include <stdlib.h>

#include <tari/datastructures.h>
#include <tari/geometry.h>
#include <tari/physicshandler.h>
#include <tari/memoryhandler.h>
#include <tari/collisionhandler.h>
#include <tari/log.h>
#include <tari/math.h>
#include <tari/mugenanimationhandler.h>

#include "cane_collision.h"
#include "cane_resources.h"

typedef struct {
	Position* pos;
	int physicsID;
	int animationID;
	int collisionID;

	CaneCollisionData colData;
} Bullet;

static struct {
	List bullets;

	int bulletsLeft;
	int bulletsLeftDisplayIDs[6];
} gData;



void setupCaneBulletHandling() {
	gData.bullets = new_list();
	
	int id = addMugenAnimation(createOneFrameMugenAnimationForSprite(7, 0), getCaneSprites(), makePosition(20, 396, 13));
	setMugenAnimationBaseDrawScale(id, 0.75);
	
	gData.bulletsLeft = 0;

	int i;
	for (i = 0; i < 6; i++) {
		double x = 20 + 0.75*32 * i;
		gData.bulletsLeftDisplayIDs[i] = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 5), getCaneSprites(), makePosition(x, 428, 13));
		setMugenAnimationDrawAngle(gData.bulletsLeftDisplayIDs[i], M_PI / 4);
		reloadCaneBullet();
	}
}

void reloadCaneBullet() {
	if (gData.bulletsLeft == 6) return;
	
	int i = gData.bulletsLeft;
	setMugenAnimationBaseDrawScale(gData.bulletsLeftDisplayIDs[i], 0.75);

	gData.bulletsLeft++;
}

int getCaneBulletAmount() {
	return gData.bulletsLeft;
}

static void removeBulletFromPistol() {
	if (!gData.bulletsLeft) {
		logWarning("Trying to remove bullet from pistol without bullets.");
		return;
	}

	int i = gData.bulletsLeft - 1;
	setMugenAnimationBaseDrawScale(gData.bulletsLeftDisplayIDs[i], 0);

	gData.bulletsLeft--;
}

static void removeBullet(Bullet* e) {
	removeMugenAnimation(e->animationID);
	removeFromPhysicsHandler(e->physicsID);
	removeFromCollisionHandler(getCaneBulletCollisionListID(), e->collisionID);
}

static int deleteAllBullets(void* caller, void* data) {
	Bullet* e = data;
	removeBullet(e);

	return 1;
}

void shutdownCaneBulletHandling() {
	list_remove_predicate(&gData.bullets, deleteAllBullets, NULL);
}

static int updateBullet(void* caller, void* data) {
	Bullet* e = data;

	Position p = *(e->pos);
	if (p.x < -200 || p.x > 700 || p.y < -200 || p.y > 700) {
		removeBullet(e);
		return 1;
	}

	return 0;
}

void updateCaneBulletHandling() {
	list_remove_predicate(&gData.bullets, updateBullet, NULL);

}

static void bulletHit(void* caller, void* collisionData) {
	Bullet* e = caller;
	
	e->pos->x = -300;
}

void addCaneBullet(Position pos, double angle) {
	Bullet* e = allocMemory(sizeof(Bullet));

	e->physicsID = addToPhysicsHandler(pos);
	e->pos = &getPhysicsFromHandler(e->physicsID)->mPosition;
	addAccelerationToHandledPhysics(e->physicsID, vecRotateZ(makePosition(10,0,0), -angle));
	e->animationID = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 5), getCaneSprites(), makePosition(0, 0, 0));
	setMugenAnimationBasePosition(e->animationID, e->pos);
	setMugenAnimationDrawAngle(e->animationID, angle);

	e->colData.listID = getCaneBulletCollisionListID();
	e->colData.direction = &getPhysicsFromHandler(e->physicsID)->mVelocity;

	e->collisionID = addColliderToCollisionHandler(getCaneBulletCollisionListID(), e->pos, makeColliderFromCirc(makeCollisionCirc(makePosition(0, 0, 0), 16)), bulletHit, e, &e->colData);
	list_push_back_owned(&gData.bullets, e);

	removeBulletFromPistol();
}
