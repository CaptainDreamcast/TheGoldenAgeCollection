#include "cane_player.h"

#include <math.h>

#include <tari/animationtree.h>
#include <tari/input.h>
#include <tari/physicshandler.h>
#include <tari/file.h>
#include <tari/math.h>
#include <tari/collisionhandler.h>
#include <tari/system.h>
#include <tari/timer.h>
#include <tari/math.h>
#include <tari/mugenanimationhandler.h>

#include "cane_resources.h"
#include "cane_bullet.h"
#include "cane_collision.h"
#include "cane_gamelogic.h"
#include "cane_shadow.h"

static struct {
	int animationID;
	int physicsID;
	int collisionID;
	int shadowID;

	int armAnimationID;

	double armAngle;
	Position armRotationPoint;
	Position armPosition;

	int isInShotCooldown;
	Duration shotCooldownNow;
	Duration shotCooldown;

	int isInReloadCooldown;
	Duration reloadCooldownNow;
	Duration reloadCooldown;

	Position* pos;

	CaneCollisionData colData;

	double accel;

	int isDying;

} gData;

static void playerHit(void* caller, void* collisionData);

void loadCanePlayer() {

	gData.physicsID = addToPhysicsHandler(makePosition(100,350,6));
	setHandledPhysicsDragCoefficient(gData.physicsID, makePosition(0.2, 0.2, 0));
	setHandledPhysicsMaxVelocity(gData.physicsID, 4);
	gData.accel = 1;
	gData.pos = &getPhysicsFromHandler(gData.physicsID)->mPosition;

	gData.animationID = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 1), getCaneSprites(), makePosition(0, 0, 0));
	setMugenAnimationBasePosition(gData.animationID, gData.pos);

	gData.armAnimationID = -1;
	gData.armAngle = 0;
	gData.armRotationPoint = makePosition(0, 7, 0);
	gData.armPosition = makePosition(70, 27, -1);

	gData.shadowID = addCaneShadow(gData.pos);

	gData.isInShotCooldown = 0;
	gData.shotCooldownNow = 0;
	gData.shotCooldown = 20;

	gData.isInReloadCooldown = 0;
	gData.reloadCooldownNow = 0;
	gData.reloadCooldown = 20;

	gData.colData.listID = getCanePlayerCollisionListID();
	gData.colData.direction = &getPhysicsFromHandler(gData.physicsID)->mVelocity;
	CollisionRect rect = makeCollisionRect(makePosition(48, 32, 0), makePosition(48+40, 32+18, 1));
	Collider collider = makeColliderFromRect(rect);
	gData.collisionID = addColliderToCollisionHandler(getCanePlayerCollisionListID(), gData.pos, collider, playerHit, NULL, &gData.colData);	

	gData.isDying = 0;
}

static void playerDied(void* caller) {
	removeFromPhysicsHandler(gData.physicsID);
	setCaneLevelLost();
}

static void setArmActive(double angle) {
	if (gData.armAnimationID == -1) {
		gData.armAnimationID = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 6), getCaneSprites(), gData.armPosition);
		setMugenAnimationBasePosition(gData.armAnimationID, gData.pos);
	}

	setMugenAnimationDrawAngle(gData.armAnimationID, angle);
}

static void setArmInactive() {
	if (gData.armAnimationID == -1) return;
	removeMugenAnimation(gData.armAnimationID);
	gData.armAnimationID = -1;
}

static void playerHit(void* caller, void* collisionData) {
	
	removeMugenAnimation(gData.animationID);
	removeFromCollisionHandler(getCanePlayerCollisionListID(), gData.collisionID);
	setArmInactive();

	int id = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 2), getCaneSprites(), makePosition(0, 0, 0));
	setMugenAnimationBasePosition(id, gData.pos);
	addAccelerationToHandledPhysics(gData.physicsID, makePosition(-20, 0, 0));
	setHandledPhysicsDragCoefficient(gData.physicsID, makePosition(0, 0, 0));
	setHandledPhysicsMaxVelocity(gData.physicsID, INF);
	addTimerCB(120, playerDied, NULL);
	gData.isDying = 1;
}

static void handleArmDirection() {
	double dx = getLeftStickNormalizedX();
	double dy = getLeftStickNormalizedY();
	double l = fstsqrt(dx*dx + dy*dy);

	if (l < 0.5) {
		setArmInactive();
		return;
	}

	gData.armAngle = getAngleFromDirection(makePosition(-dx, -dy, 0)); // TODO: fix this with tests
	setArmActive(gData.armAngle);
}

static void handleCooldown(int* flag, Duration* now, Duration duration) {
	if (!*flag) return;

	if (handleDurationAndCheckIfOver(now, duration)) {
		*flag = 0;
	}
}

static void handleShotCooldown() {
	handleCooldown(&gData.isInShotCooldown, &gData.shotCooldownNow, gData.shotCooldown);
}

static void handleReloadCooldown() {
	handleCooldown(&gData.isInReloadCooldown, &gData.reloadCooldownNow, gData.reloadCooldown);
}


static void handleShotFireAttempt() {
	if (gData.isInShotCooldown || gData.armAnimationID == -1 || !getCaneBulletAmount()) return;

	Position bulletPos = vecAdd(*gData.pos, vecRotateZ(makePosition(35, 0, 0), -gData.armAngle));
	bulletPos = vecAdd(vecAdd(bulletPos, gData.armRotationPoint), gData.armPosition);

	addCaneBullet(bulletPos, gData.armAngle);

	gData.shotCooldownNow = 0;
	gData.isInShotCooldown = 1;
}

static void handleReloadAttempt() {
	if (gData.isInReloadCooldown || gData.armAnimationID != -1 || getCaneBulletAmount() == 6) return;

	reloadCaneBullet();

	gData.reloadCooldownNow = 0;
	gData.isInReloadCooldown = 1;
}

static void handleShots() {

	handleShotCooldown();
	handleReloadCooldown();
	
	if (hasPressedR()) {
		handleShotFireAttempt();
	}


	handleReloadAttempt();



}

static void handleMovement() {

	if (hasPressedX()) {
		addAccelerationToHandledPhysics(gData.physicsID, makePosition(-gData.accel, 0, 0));
	} else if (hasPressedB()) {
		addAccelerationToHandledPhysics(gData.physicsID, makePosition(gData.accel, 0, 0));
	}

}

static void constraintPlayerPosition() {
	gData.pos->x = fmax(-64, gData.pos->x);
	gData.pos->x = fmin(576, gData.pos->x);

}
	


void updateCanePlayer() {
	if (gData.isDying) return;

	constraintPlayerPosition();
	handleArmDirection();

	handleShots();

	handleMovement();
}
