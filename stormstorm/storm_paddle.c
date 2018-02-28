#include "storm_paddle.h"

#include <stdio.h>

#include <prism/physicshandler.h>
#include <prism/input.h>
#include <prism/collisionhandler.h>

#include "vectrexanimations.h"
#include "storm_collision.h"

static struct {
	int mAnimationID;
	int mPhysicsID;
	int mCollisionID;

	JusticeCollisionData mCollisionData;

	int mIsJumping;
} gData;

static void loadStormPaddle(void* tData) {
	(void)tData;
	gData.mPhysicsID = addToPhysicsHandler(makePosition(320, 400, 0));
	setHandledPhysicsDragCoefficient(gData.mPhysicsID, makePosition(0.2, 0, 0));

	gData.mAnimationID = addVectrexImage("assets/stormstorm/player/PADDLE.pkg", makePosition(-56, -10, 3));
	setVectrexImageBasePosition(gData.mAnimationID, getHandledPhysicsPositionReference(gData.mPhysicsID));
	
	gData.mCollisionData.mListID = getStormPaddleCollisionList();
	CollisionRect rect = makeCollisionRect(makePosition(-48, -1, 0), makePosition(45, 1, 0));
	gData.mCollisionID = addCollisionRectangleToCollisionHandler(getStormPaddleCollisionList(), getHandledPhysicsPositionReference(gData.mPhysicsID), rect, NULL, NULL,  &gData.mCollisionData);

	gData.mIsJumping = 0;
}


static void updateGround() {
	Position* p = getHandledPhysicsPositionReference(gData.mPhysicsID);
	Velocity* v = getHandledPhysicsVelocityReference(gData.mPhysicsID);

	double groundY = 400;
	if (p->y >= groundY && v->y > 0) {
		setHandledPhysicsGravity(gData.mPhysicsID, makePosition(0, 0, 0));
		v->y = 0;
		p->y = groundY;
		gData.mIsJumping = 0;
	}

}

static void updateJumping() {
	if (gData.mIsJumping) return;

	if (hasPressedAFlank()) {
		gData.mIsJumping = 1;
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(0, -15, 0));
		setHandledPhysicsGravity(gData.mPhysicsID, makePosition(0, 1, 0));
	}
}

static void updateMovementInput() {
	if (hasPressedLeft()) {
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(-2, 0, 0));
	}
	else if (hasPressedRight()) {
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(2, 0, 0));
	}
}

static void updateMovementConstraints() {
	Velocity* vel = getHandledPhysicsVelocityReference(gData.mPhysicsID);
	double speedX = fabs(vel->x);

	double maxSpeed = 2;
	if (speedX > maxSpeed) {
		vel->x = (vel->x / speedX) * maxSpeed;
	}

	Position* pos = getHandledPhysicsPositionReference(gData.mPhysicsID);
	if (pos->x < 240) pos->x = 240;
	if (pos->x > 420) pos->x = 420;
}


static void updateStormPaddle(void* tData) {
	(void)tData;

	updateJumping();
	updateGround();
	updateMovementInput();
	updateMovementConstraints();
}


static ActorBlueprint StormPaddle = {
	.mLoad = loadStormPaddle,
	.mUpdate = updateStormPaddle,
};

ActorBlueprint getStormPaddleBP()
{
	return StormPaddle;
}

Velocity getStormPaddleVelocity()
{
	return *getHandledPhysicsVelocityReference(gData.mPhysicsID);
}
