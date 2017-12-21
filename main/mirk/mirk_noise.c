#include "mirk_noise.h"

#include <tari/soundeffect.h>
#include <tari/timer.h>
#include <tari/math.h>

typedef struct {
	Duration mNow;
	Duration mDuration;

	int mSFX;
	int mAnimation;
} Noise;

static struct {
	TextureData mTexture;
	int mSoundEffect;
	int mIsActive;
} gData;

void loadMirkScreenNoise() {
	gData.mTexture = loadTexture("assets/main/mirk/effects/NOISE.pkg");
	gData.mSoundEffect = loadSoundEffect("assets/main/mirk/sfx/noise/noise.wav");
	gData.mIsActive = 0;
}

static void stopScreenNoise(Noise* e) {
	stopSoundEffect(e->mSFX);
	removeHandledAnimation(e->mAnimation);
	freeMemory(e);
	gData.mIsActive = 0;
}

static void updateScreenNoise(void* tCaller) {
	Noise* e = tCaller;
	int dx = randfromInteger(-100, 100);
	int dy = randfromInteger(-100, 100);
	Position p = makePosition(-200 + dx, -200 + dy, 17);
	setAnimationPosition(e->mAnimation, p);

	if (handleDurationAndCheckIfOver(&e->mNow, e->mDuration)) {
		stopScreenNoise(e);
	}
	else {
		addTimerCB(0, updateScreenNoise, e);
	}
}


void addMirkScreenNoise(Duration d)
{
	if (gData.mIsActive) return;

	Noise* e = allocMemory(sizeof(Noise));
	e->mNow = 0;
	e->mDuration = d;
	e->mAnimation = playAnimationLoop(makePosition(-100, -100, 15), &gData.mTexture, createOneFrameAnimation(), makeRectangleFromTexture(gData.mTexture));
	setAnimationSize(e->mAnimation, makePosition(1024, 1024, 1), makePosition(0, 0, 0));
	e->mSFX = playSoundEffect(gData.mSoundEffect);

	addTimerCB(0, updateScreenNoise, e);
	gData.mIsActive = 1;
}
