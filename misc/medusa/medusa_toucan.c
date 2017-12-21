#include "medusa_toucan.h"

#include <tari/animation.h>
#include <tari/physicshandler.h>
#include <tari/input.h>

#include "medusa_shothandler.h"

static struct {
	int mPhysicsID;
	int mWhiteBackgroundID;
	int mAnimationID;

	Vector3D mSize;

	TextureData mTextures[4];
	TextureData mWhiteBackground;

	Duration mCooldownNow;
	Duration mCooldownDuration;
	int mIsInCooldown;

} gData;

static void loadToucan(void* tData) {
	(void)tData;

	loadConsecutiveTextures(gData.mTextures, "assets/misc/medusa/TOUCAN.pkg", 3);
	gData.mWhiteBackground = loadTexture("$/rd/effects/white.pkg");

	gData.mPhysicsID = addToPhysicsHandler(makePosition(20, 200, 0));
	setHandledPhysicsDragCoefficient(gData.mPhysicsID, makePosition(0.2, 0.2, 0));
	setHandledPhysicsMaxVelocity(gData.mPhysicsID, 1);

	gData.mSize = makePosition(107, 200, 1);
	gData.mWhiteBackgroundID = playOneFrameAnimationLoop(makePosition(-4, -4, 4), &gData.mWhiteBackground);
	setAnimationBasePositionReference(gData.mWhiteBackgroundID, getHandledPhysicsPositionReference(gData.mPhysicsID));
	setAnimationSize(gData.mWhiteBackgroundID, vecAdd(gData.mSize, makePosition(8, 8, 0)), makePosition(0, 0, 0));

	gData.mAnimationID = playOneFrameAnimationLoop(makePosition(0, 0, 5), &gData.mTextures[0]);
	setAnimationBasePositionReference(gData.mAnimationID, getHandledPhysicsPositionReference(gData.mPhysicsID));
	setAnimationSize(gData.mAnimationID, gData.mSize, makePosition(0, 0, 0));

	gData.mIsInCooldown = 0;
	gData.mCooldownDuration = 30;
}

static void updateShot() {
	if (gData.mIsInCooldown) {
		if (handleDurationAndCheckIfOver(&gData.mCooldownNow, gData.mCooldownDuration)) {
			gData.mIsInCooldown = 0;
		}
		return;
	}

	if (hasPressedA()) {
		Position p = *getHandledPhysicsPositionReference(gData.mPhysicsID);
		addMedusaShot(vecAdd(p, makePosition(gData.mSize.x, gData.mSize.y / 2, 0)));

		gData.mCooldownNow = 0;
		gData.mIsInCooldown = 1;
	}
}

static void updateMovement() {
	if (hasPressedLeft()) {
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(-1, 0, 0));
	}
	else if (hasPressedRight()) {
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(1, 0, 0));
	}
	
	if (hasPressedUp()) {
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(0, -1, 0));
	}
	else if (hasPressedDown()) {
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(0, 1, 0));
	}
}

static void transformToucan(TextureData* tNewTexture) {
	changeAnimation(gData.mAnimationID, tNewTexture, createOneFrameAnimation(), makeRectangleFromTexture(*tNewTexture));
	setAnimationSize(gData.mAnimationID, gData.mSize, makePosition(0, 0, 0));
}

static void updateTransform() {
	if (hasPressedXFlank()) {
		transformToucan(&gData.mTextures[0]);
	}

	if (hasPressedYFlank()) {
		transformToucan(&gData.mTextures[1]);
	}

	if (hasPressedBFlank()) {
		transformToucan(&gData.mTextures[2]);
	}
}

static void updateToucan(void* tData) {
	(void)tData;

	updateShot();
	updateMovement();
	updateTransform();
}

ActorBlueprint MedusaToucan = {
	.mLoad = loadToucan,
	.mUpdate = updateToucan,
};