#include "storm_particlehandler.h"

#include <tari/animation.h>
#include <tari/datastructures.h>
#include <tari/physicshandler.h>
#include <tari/math.h>

#include "vectrexanimations.h"

typedef struct {
	int mPhysicsID;

	Duration mNow;
	Duration mDuration;
} ActiveParticle;

static struct {
	IntMap mParticles;
} gData;

static void loadParticleHandler(void* tData) {
	gData.mParticles = new_int_map();
}

static void unloadParticle(ActiveParticle* e) {
	removeFromPhysicsHandler(e->mPhysicsID);
}

static int updateParticle(void* tCaller, void* tData) {
	(void)tCaller;
	ActiveParticle* e = tData;

	Position p = *getHandledPhysicsPositionReference(e->mPhysicsID);
	addVectrexOneFrameDot(p);

	if (handleDurationAndCheckIfOver(&e->mNow, e->mDuration)) {
		unloadParticle(e);
		return 1;
	}

	return 0;
}

static void updateParticleHandler(void* tData) {
	int_map_remove_predicate(&gData.mParticles, updateParticle, NULL);
}

ActorBlueprint StormParticleHandler = {
	.mLoad = loadParticleHandler,
	.mUpdate = updateParticleHandler,
};

static void addSingleParticle(Position tPosition, Velocity tVelocity) {
	ActiveParticle* e = allocMemory(sizeof(ActiveParticle));
	e->mNow = 0;
	e->mDuration = randfrom(15, 20);

	e->mPhysicsID = addToPhysicsHandler(tPosition);
	addAccelerationToHandledPhysics(e->mPhysicsID, tVelocity);

	int_map_push_back_owned(&gData.mParticles, e);
}

void addStormParticleSpread(Position tPosition)
{
	int i;
	for (i = 0; i < 20; i++) {
		addSingleParticle(tPosition, makePosition(randfrom(-2, 2), randfrom(-2, 2), 0));
	}
}

void addStormTitleSparcleParticle() {
	addSingleParticle(makePosition(randfrom(0, 640), randfrom(0, 480), 2), makePosition(0, 0, 0));
}

int getStormTitleParticleAmount() {
	return int_map_size(&gData.mParticles);
}
