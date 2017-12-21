#include "sfmk_animation.h"

#include <math.h>

#include <tari/memoryhandler.h>
#include <tari/timer.h>
#include <tari/physicshandler.h>
#include <tari/math.h>


static void updateAnimationFlickering(void* tCaller) {
	VersusAnimationFlickering* e = tCaller;
	
	if (!e->mActive) {
		removeFromPhysicsHandler(e->mPhysicsID);
		freeMemory(e);
		return;
	}

	setAnimationTransparency(e->mAnimationID, *e->mAlpha);
	if (*e->mAlpha <= 0 || *e->mAlpha >= 1) {
		*e->mAlpha = fmax(0.0, fmin(1.0, *e->mAlpha));
		stopHandledPhysics(e->mPhysicsID);
		e->mAlphaDelta *= -1;
		addAccelerationToHandledPhysics(e->mPhysicsID, makePosition(e->mAlphaDelta, 0, 0));
		*e->mAlpha += e->mAlphaDelta;
	}

	addTimerCB(0, updateAnimationFlickering, e);
}

VersusAnimationFlickering* setVersusAnimationFlickering(int tAnimationID, Duration tFlickerDuration) {
	VersusAnimationFlickering* e = allocMemory(sizeof(VersusAnimationFlickering));
	e->mAnimationID = tAnimationID;
	e->mPhysicsID = addToPhysicsHandler(makePosition(1, 0, 0));
	e->mAlpha = &getPhysicsFromHandler(e->mPhysicsID)->mPosition.x;
	e->mAlphaDelta = -1 / (double)tFlickerDuration;
	addAccelerationToHandledPhysics(e->mPhysicsID, makePosition(e->mAlphaDelta, 0, 0));
	e->mActive = 1;
	addTimerCB(0, updateAnimationFlickering, e);
	return e;
}

void setVersusAnimationFlickeringDuration(VersusAnimationFlickering* e, Duration tDuration) {
	stopHandledPhysics(e->mPhysicsID);
	*e->mAlpha = 1;
	e->mAlphaDelta = -1 / (double)tDuration;
	addAccelerationToHandledPhysics(e->mPhysicsID, makePosition(e->mAlphaDelta, 0, 0));
}

void stopVersusAnimationFlickering(VersusAnimationFlickering* e) {
	e->mActive = 0;
}
