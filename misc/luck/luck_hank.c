#include "luck_hank.h"

#include <string.h>

#include <prism/animation.h>
#include <prism/physicshandler.h>
#include <prism/input.h>
#include <prism/collisionhandler.h>
#include <prism/wrapper.h>
#include <prism/collisionanimation.h>
#include <prism/math.h>

#include "luck_gameover.h"
#include "luck_collision.h"
#include "luck_gamelogic.h"

static struct {
	char mCurrentName[100];

	TextureData mIdle;
	Animation mIdleAnimation;
	TextureData mWalking[10];
	Animation mWalkingAnimation;
	TextureData mAttacking[10];
	Animation mAttackingAnimation;
	CollisionAnimation mAttack;
	Collider mAttackHitboxes[3];
	LuckCollisionData mAttackCollisionData;

	int mAnimation;
	int mPhysics;
	int mCollision;
	LuckCollisionData mCollisionData;



	int mIsWalking;
	int mIsAttacking;
	int mFaceDirection;
	int mIsBigTexture;

	int mIsSupposedToGoRight;
	int mIsEvasionLevel;
} gData;

static void hasBeenHit(void* tCaller, void* tCollisionData);

static void loadHank(void* tData) {
	(void)tData;
	char path[1024];

	sprintf(path, "assets/misc/luck/sprites/HANK_%s.pkg", gData.mCurrentName);
	
	gData.mIdleAnimation = createOneFrameAnimation();
	gData.mIdle = loadTexture(path);

	gData.mIsBigTexture = gData.mIdle.mTextureSize.x == 256;

	gData.mPhysics = addToPhysicsHandler(makePosition(100, 200, 2));
	setHandledPhysicsDragCoefficient(gData.mPhysics, makePosition(0.2, 0.2, 0));
	setHandledPhysicsMaxVelocity(gData.mPhysics, 3);

	sprintf(path, "assets/misc/luck/sprites/HANK_%s_WALKING.pkg", gData.mCurrentName);
	gData.mWalkingAnimation = createEmptyAnimation();
	gData.mWalkingAnimation.mFrameAmount = 4;
	gData.mWalkingAnimation.mDuration = 3;
	loadConsecutiveTextures(gData.mWalking, path, gData.mWalkingAnimation.mFrameAmount);

	sprintf(path, "assets/misc/luck/sprites/HANK_%s_ATTACKING.pkg", gData.mCurrentName);
	gData.mAttackingAnimation = createEmptyAnimation();
	gData.mAttackingAnimation.mFrameAmount = 3;
	gData.mAttackingAnimation.mDuration = 7;
	loadConsecutiveTextures(gData.mAttacking, path, gData.mAttackingAnimation.mFrameAmount);

	gData.mAttack = makeEmptyCollisionAnimation();
	gData.mAttackHitboxes[0] = makeColliderFromRect(makeCollisionRect(makePosition(0, 0, 0), makePosition(0, 0, 0)));
	gData.mAttackHitboxes[1] = makeColliderFromRect(makeCollisionRect(makePosition(0, 0, 0), makePosition(0, 0, 0)));
	if(gData.mIsBigTexture) gData.mAttackHitboxes[2] = makeColliderFromRect(makeCollisionRect(makePosition(128+64, 32, 0), makePosition(256, 32+64, 0)));
	else gData.mAttackHitboxes[2] = makeColliderFromRect(makeCollisionRect(makePosition(64, 32, 0), makePosition(128, 32+64, 0)));
	gData.mAttack.mAnimation = gData.mAttackingAnimation;
	vector_push_back(&gData.mAttack.mFrames, &gData.mAttackHitboxes[0]);
	vector_push_back(&gData.mAttack.mFrames, &gData.mAttackHitboxes[1]);
	vector_push_back(&gData.mAttack.mFrames, &gData.mAttackHitboxes[2]);
	gData.mAttackCollisionData.mList = getLuckHankAttackCollisionList();

	gData.mIsWalking = 0;
	gData.mIsAttacking = 0;
	gData.mFaceDirection = 1;
	gData.mIsEvasionLevel = 0;

	gData.mAnimation = playAnimationLoop(makePosition(0,0,0), &gData.mIdle, gData.mIdleAnimation, makeRectangleFromTexture(gData.mIdle));
	setAnimationBasePositionReference(gData.mAnimation, getHandledPhysicsPositionReference(gData.mPhysics));
	if(gData.mIsBigTexture) setAnimationCenter(gData.mAnimation, makePosition(128, 64, 0));
		else setAnimationCenter(gData.mAnimation, makePosition(64, 64, 0));

	Position mColliderOffset;
	if (gData.mIsBigTexture) mColliderOffset = makePosition(128 - 16, 128 - 32, 0);
	else mColliderOffset = makePosition(64 - 16, 128 - 32, 0);
	
	Collider c = makeColliderFromRect(makeCollisionRect(vecAdd(makePosition(0,0,0), mColliderOffset), vecAdd(makePosition(32, 32, 0), mColliderOffset)));
	gData.mCollisionData.mList = getLuckHankCollisionList();
	gData.mCollision = addColliderToCollisionHandler(getLuckHankCollisionList(), getHandledPhysicsPositionReference(gData.mPhysics), c, hasBeenHit, NULL, &gData.mCollisionData);

	gData.mIsSupposedToGoRight = 0;
}

static void hasHit(void* tCaller, void* tCollisionData) {
	(void)tCaller;
	(void)tCollisionData;
}

static void hasBeenHit(void* tCaller, void* tCollisionData) {
	(void)tCaller;
	(void)tCollisionData;
	setNewScreen(&LuckGameOverScreen);
}

static void setIdle() {
	changeAnimation(gData.mAnimation, &gData.mIdle, gData.mIdleAnimation, makeRectangleFromTexture(gData.mIdle));
	gData.mIsWalking = 0;
	gData.mIsAttacking = 0;
}

static void setWalking() {
	changeAnimation(gData.mAnimation, gData.mWalking, gData.mWalkingAnimation, makeRectangleFromTexture(*gData.mWalking));
	gData.mIsWalking = 1;
	gData.mIsAttacking = 0;
}

static void attackOver(void* tCaller) {
	(void)tCaller;

	setAnimationCB(gData.mAnimation, NULL, NULL);
	setIdle();
}

static void setAttacking() {
	changeAnimation(gData.mAnimation, gData.mAttacking, gData.mAttackingAnimation, makeRectangleFromTexture(*gData.mAttacking));
	setAnimationCB(gData.mAnimation, attackOver, NULL);
	int colAnim = addHandledCollisionAnimation(getLuckHankAttackCollisionList(), getHandledPhysicsPositionReference(gData.mPhysics), gData.mAttack, hasHit, NULL, &gData.mAttackCollisionData);
	if (gData.mFaceDirection == -1) invertCollisionAnimationVertical(colAnim);
	if(gData.mIsBigTexture) setCollisionAnimationCenter(colAnim, makePosition(128, 64, 0));
	else setCollisionAnimationCenter(colAnim, makePosition(64, 64, 0));
	gData.mIsWalking = 0;
	gData.mIsAttacking = 1;
}

static void invertFaceDirection() {
	inverseAnimationVertical(gData.mAnimation);
	gData.mFaceDirection = (gData.mFaceDirection == 1) ? -1 : 1;
}

static void updateWalking() {

	if (gData.mIsEvasionLevel) {
		if(!gData.mIsWalking) setWalking();
		return;
	}

	if (gData.mIsWalking && vecLength(*getHandledPhysicsVelocityReference(gData.mPhysics)) < 0.5) setIdle();
	else if (!gData.mIsWalking && vecLength(*getHandledPhysicsVelocityReference(gData.mPhysics)) > 0.5) setWalking();

	if (gData.mFaceDirection == 1 && getHandledPhysicsVelocityReference(gData.mPhysics)->x < -1) invertFaceDirection();
	else if (gData.mFaceDirection == -1 && getHandledPhysicsVelocityReference(gData.mPhysics)->x > 1) invertFaceDirection();
}

static void confineHank() {
	Position* p = getHandledPhysicsPositionReference(gData.mPhysics);

	if (!gData.mIsEvasionLevel) {
		p->x = max(100, p->x);
		p->y = max(100, p->y);
		p->y = min(250, p->y);
		if (!gData.mIsSupposedToGoRight) {
			p->x = min(600, p->x);
		}
	}
	else {
		p->x = max(0, p->x);
		p->x = min(600, p->x);
		p->y = max(0, p->y);
		p->y = min(250, p->y);
	}

	
}

static void checkVictoryCondition() {
	if (gData.mIsSupposedToGoRight) {
		Position* p = getHandledPhysicsPositionReference(gData.mPhysics);
		if (p->x > 640) {
			moveLuckToNextScreen();
		}
	}
}

static void updateZ() {
	Position* p = getHandledPhysicsPositionReference(gData.mPhysics);
	double y = p->y + 64;
	p->z = 2 + (y - (-64)) / (480 - (-64.0));
}

static void updateHank(void* tData) {
	(void)tData;
	confineHank();
	updateZ();
	checkVictoryCondition();

	if (gData.mIsAttacking) return;

	updateWalking();
	

	if (hasPressedUp()) {
		addAccelerationToHandledPhysics(gData.mPhysics, makePosition(0, -1, 0));
	}
	else if (hasPressedDown()) {
		addAccelerationToHandledPhysics(gData.mPhysics, makePosition(0, 1, 0));
	}

	if (hasPressedLeft()) {
		addAccelerationToHandledPhysics(gData.mPhysics, makePosition(-1, 0, 0));
	}
	else if (hasPressedRight()) {
		addAccelerationToHandledPhysics(gData.mPhysics, makePosition(1, 0, 0));
	}

	if (gData.mIsEvasionLevel) return;

	if (hasPressedAFlank()) {
		setAttacking();
	}

}

ActorBlueprint LuckHankBP = {
	.mLoad = loadHank,
	.mUpdate = updateHank,
};

void setLuckHankName(char * tName)
{
	strcpy(gData.mCurrentName, tName);
}

void setLuckHankSupposedToGoRight()
{
	gData.mIsSupposedToGoRight = 1;
}

void setLuckHankPosition(Position tPosition)
{
	Position* p = getHandledPhysicsPositionReference(gData.mPhysics);
	*p = tPosition;
}

void setLuckHankEvasionLevel()
{
	gData.mIsEvasionLevel = 1;
}
