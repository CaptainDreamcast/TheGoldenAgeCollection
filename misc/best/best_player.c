#include "best_player.h"

#include <prism/animation.h>
#include <prism/math.h>
#include <prism/input.h>

#include "best_endscreen.h"

static struct {
	TextureData mPlayerTextures[4];

	Animation mAnimation;
	Duration mAnimationDurations[4];
	Duration mNow;

	Position mPosition;

	int mAnimationID;

} gData;

static void loadPlayer(void* tData) {
	(void)tData;

	gData.mAnimation = createAnimation(4, INF);
	loadConsecutiveTextures(gData.mPlayerTextures, "assets/misc/best/PLAYER.pkg", gData.mAnimation.mFrameAmount);
	gData.mAnimationDurations[0] = gData.mAnimationDurations[2] = 30;
	gData.mAnimationDurations[1] = gData.mAnimationDurations[3] = 1;
	gData.mNow = 0;

	gData.mPosition = makePosition(320, 207, 0);
	gData.mAnimationID = playOneFrameAnimationLoop(makePosition(-15, 0, 2), &gData.mPlayerTextures[gData.mAnimation.mFrame]);
	setAnimationBasePositionReference(gData.mAnimationID, &gData.mPosition);
}

static void updateAnimation() {
	if(handleDurationAndCheckIfOver(&gData.mNow, gData.mAnimationDurations[gData.mAnimation.mFrame])) {
		gData.mNow = 0;
		gData.mAnimation.mFrame = (gData.mAnimation.mFrame + 1) % gData.mAnimation.mFrameAmount;
		changeAnimation(gData.mAnimationID, &gData.mPlayerTextures[gData.mAnimation.mFrame], createOneFrameAnimation(), makeRectangleFromTexture(gData.mPlayerTextures[gData.mAnimation.mFrame]));
	}
}

static void updateMovement() {
	if (hasPressedLeft()) {
		gData.mPosition.x--;
		updateAnimation();
	}
	else if (hasPressedRight()) {
		gData.mPosition.x++;
		updateAnimation();
	}

}

static void updateVictoryConditions() {
	if (gData.mPosition.x < 140) {
		setBestEndScreenLose();
		setNewScreen(&BestEndScreen);
	}

	if (gData.mPosition.x > 500) {
		setBestEndScreenWin();
		setNewScreen(&BestEndScreen);
	}
}

static void updatePlayer(void* tData) {
	(void)tData;

	updateMovement();
	updateVictoryConditions();
}

ActorBlueprint BestPlayer = {
	.mLoad = loadPlayer,
	.mUpdate = updatePlayer,
};