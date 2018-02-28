#include "mirk_particles.h"

#include <prism/actorhandler.h>
#include <prism/animation.h>
#include <prism/physicshandler.h>
#include <prism/math.h>
#include <prism/log.h>
#include <prism/system.h>

#include "mirk_stage.h"

// TODO: find a nicer way to write this
enum { MAX_PARTICLE_AMOUNT = (int)(1000*PERFORMANCE_FACTOR) };

typedef struct {
	int mActive;
	int mAnimation;
	int mPhysics;
	Duration mNow;
	Duration mDuration;
	Color mColor;
	double mRotation;
	double mRotationDelta;
	Position mCenter;
} Particle;

static struct {
	TextureData mBlood;
	int mHead;
	int mAmount;
	Particle mParticles[MAX_PARTICLE_AMOUNT];

	int mIsReal;
} gData;

void loadMirkParticles() {
	gData.mBlood = loadTexture("assets/main/mirk/particles/blood.pkg");
	
	gData.mAmount = 0;
	gData.mHead = 0;

	int i;
	for (i = 0; i < MAX_PARTICLE_AMOUNT; i++) {
		gData.mParticles[i].mActive = 0;
	}

	gData.mIsReal = 0;
}


static void unloadParticle(Particle* e) {
	drawMirkBloodOnStage(*getHandledPhysicsPositionReference(e->mPhysics), e->mColor);
	removeHandledAnimation(e->mAnimation);
	removeFromPhysicsHandler(e->mPhysics);
	e->mActive = 0;

	gData.mAmount--;
}

static void updateParticle(Particle* e) {

	e->mRotation += e->mRotationDelta;
	setAnimationRotationZ(e->mAnimation, e->mRotation, e->mCenter);

	if (handleDurationAndCheckIfOver(&e->mNow, e->mDuration)) {
		unloadParticle(e);
	}
	
}

void updateMirkParticles() {
	int i;
	for (i = 0; i < MAX_PARTICLE_AMOUNT; i++) {
		if (!gData.mParticles[i].mActive) continue;
	
		updateParticle(&gData.mParticles[i]);
	}
}

void setMirkParticlesReal() {
	int i;
	for (i = 0; i < MAX_PARTICLE_AMOUNT; i++) {
		if (!gData.mParticles[i].mActive) continue;

		Particle* e = &gData.mParticles[i];
		setAnimationColorType(e->mAnimation, COLOR_WHITE);
	}
	
	gData.mIsReal = 1;
}

void setMirkParticlesUnreal() {
	int i;
	for (i = 0; i < MAX_PARTICLE_AMOUNT; i++) {
		if (!gData.mParticles[i].mActive) continue;

		Particle* e = &gData.mParticles[i];
		setAnimationColorType(e->mAnimation, e->mColor);
	}

	gData.mIsReal = 0;
}

void invertMirkParticleReality()
{
	if (gData.mIsReal) setMirkParticlesUnreal();
	else setMirkParticlesReal();

}




static Particle* getNextFreeParticle() {
	
	int i;
	for (i = 0; i < MAX_PARTICLE_AMOUNT; i++) {
		if (!gData.mParticles[gData.mHead].mActive) return &gData.mParticles[gData.mHead];
		gData.mHead = (gData.mHead + 1) % MAX_PARTICLE_AMOUNT;

	}

	logError("Unable to find free particle.");
	logErrorInteger(gData.mAmount);
	abortSystem();
	return NULL;
}

static void addParticle(int tAmount, Position pos, Color tColor, TextureData* tTexture, GeoRectangle tPosRange, GeoRectangle tVelRange, double tMinRotationDelta, double tMaxRotationDelta, Duration tDuration) {
	
	int i;
	for (i = 0; i < tAmount; i++) {
		if (gData.mAmount >= MAX_PARTICLE_AMOUNT) return;

		double rx = randfrom(tPosRange.mTopLeft.x, tPosRange.mBottomRight.x);
		double ry = randfrom(tPosRange.mTopLeft.y, tPosRange.mBottomRight.y);

		double vx = randfrom(tVelRange.mTopLeft.x, tVelRange.mBottomRight.x);
		double vy = randfrom(tVelRange.mTopLeft.y, tVelRange.mBottomRight.y);

		Particle* p = getNextFreeParticle();
		p->mPhysics = addToPhysicsHandler(vecAdd(pos, makePosition(rx, ry, 0)));
		addAccelerationToHandledPhysics(p->mPhysics, makePosition(vx, vy, 0));
		setHandledPhysicsGravity(p->mPhysics, makePosition(0, 0.1, 0));
		p->mRotation = randfrom(-1, 1);
		p->mRotationDelta = randfrom(tMinRotationDelta, tMaxRotationDelta);

		p->mAnimation = playAnimationLoop(makePosition(0,0,0), tTexture, createOneFrameAnimation(), makeRectangleFromTexture(tTexture[0]));
		setAnimationBasePositionReference(p->mAnimation, getHandledPhysicsPositionReference(p->mPhysics));
		setAnimationScreenPositionReference(p->mAnimation, getMirkStagePositionReference());
		if (gData.mIsReal) {
			setAnimationColorType(p->mAnimation, COLOR_WHITE);
		}
		else {
			setAnimationColorType(p->mAnimation, tColor);
		}
		p->mCenter = makePosition(1, 1, 0);
		setAnimationRotationZ(p->mAnimation, p->mRotation, p->mCenter);
		setAnimationSize(p->mAnimation, makePosition(2, 2, 1), makePosition(0,0,0));

		p->mNow = 0;
		p->mDuration = tDuration + randfromInteger(-5 , 5);
		
		p->mColor = tColor;

		p->mActive = 1;
		gData.mAmount++;
	}

}

void addMirkBloodParticles(Position p, Color c){
	p.z = 5;
	int amount = 5 ;
	addParticle(amount, p, c, &gData.mBlood, makeGeoRectangle(-8 - 4, -8 - 4, -8 + 4, -8 + 4), makeGeoRectangle(-1, -2, 2, 1), -1, 1, 40);
}


