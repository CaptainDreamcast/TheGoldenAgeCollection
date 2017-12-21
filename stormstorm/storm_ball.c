#include "storm_ball.h"

#include <stdio.h>

#include <tari/physicshandler.h>
#include <tari/collisionhandler.h>

#include "vectrexanimations.h"
#include "storm_collision.h"
#include "storm_paddle.h"

static struct {
	int mPhysicsID;
	int mAnimationID;

	int mCollisionID;

	JusticeCollisionData mCollisionData;

} gData;

static void ballHitCB(void* tCaller, void* tCollisionData);

static void loadStormBall(void* tData) {
	(void)tData;

	gData.mPhysicsID = addToPhysicsHandler(makePosition(320, 380, 0));
	gData.mAnimationID = addVectrexImage("assets/stormstorm/player/BALL.pkg", makePosition(-22, -21, 2));
	setVectrexImageBasePosition(gData.mAnimationID, getHandledPhysicsPositionReference(gData.mPhysicsID));

	gData.mCollisionData.mListID = getBallCollisionList();
	CollisionRect rect = makeCollisionRect(makePosition(-22, -21, 0), makePosition(22, 21, 0));
	gData.mCollisionID = addCollisionRectangleToCollisionHandler(getBallCollisionList(), getHandledPhysicsPositionReference(gData.mPhysicsID), rect, ballHitCB, NULL, &gData.mCollisionData);	
}

static void updatePaddleBallContact() {
	Velocity paddleVel = getStormPaddleVelocity();
	if (paddleVel.y >= 0) return;

	setHandledPhysicsGravity(gData.mPhysicsID, makePosition(0, 1, 0));

	Velocity* vel = getHandledPhysicsVelocityReference(gData.mPhysicsID);
	vel->x = paddleVel.x;
	vel->y = paddleVel.y - 8;
}

static void ballHitCB(void* tCaller, void* tCollisionData) {
	(void)tCaller;
	JusticeCollisionData* collisionData = tCollisionData;

	if (collisionData->mListID == getStormPaddleCollisionList()) {
		updatePaddleBallContact();
	}

}

static void updateGround() {
	Position* p = getHandledPhysicsPositionReference(gData.mPhysicsID);
	Velocity* v = getHandledPhysicsVelocityReference(gData.mPhysicsID);

	double groundY = 380;
	if (p->y >= groundY && v->y > 0) {
		setHandledPhysicsGravity(gData.mPhysicsID, makePosition(0, 0, 0));
		v->y = 0;
		v->x = 0;
		p->y = groundY;
	}
}

static void updatePositionConstraints() {
	Position* pos = getHandledPhysicsPositionReference(gData.mPhysicsID);
	if (pos->x < 240) pos->x = 240;
	if (pos->x > 420) pos->x = 420;
}


static void updateStormBall(void* tData) {
	(void)tData;

	updateGround();
	updatePositionConstraints();
}

ActorBlueprint StormBall = {
	.mLoad = loadStormBall,
	.mUpdate = updateStormBall,
};
