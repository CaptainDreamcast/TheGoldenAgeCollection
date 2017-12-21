#pragma once

#include <tari/animation.h>


typedef struct {
	int mActive;

	double* mAlpha;
	double mAlphaDelta;

	int mAnimationID;
	int mPhysicsID;

} VersusAnimationFlickering;

VersusAnimationFlickering* setVersusAnimationFlickering(int tAnimationID, Duration tFlickerDuration);
void setVersusAnimationFlickeringDuration(VersusAnimationFlickering* e, Duration tDuration);
void stopVersusAnimationFlickering(VersusAnimationFlickering* e);