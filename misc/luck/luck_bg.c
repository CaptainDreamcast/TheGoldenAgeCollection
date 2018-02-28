#include "luck_bg.h"

#include <prism/animation.h>

static struct {
	TextureData mTexture;
	TextureData mFrontTexture;
	int mID;
	int mFrontID;

} gData;

static void loadBackground(void* tData) {
	(void)tData;

	gData.mTexture = loadTexture("assets/misc/luck/sprites/BG.pkg");
	gData.mFrontTexture = loadTexture("assets/misc/luck/sprites/BG_FRONT.pkg");

	gData.mID = playOneFrameAnimationLoop(makePosition(0,0,1), &gData.mTexture);
	gData.mFrontID = playOneFrameAnimationLoop(makePosition(0, 0, 4), &gData.mFrontTexture);
}

ActorBlueprint LuckBackgroundBP = {
	.mLoad = loadBackground,

};