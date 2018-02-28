#include "best_bg.h"

#include <prism/animation.h>

static struct {
	TextureData mBackgroundTexture;

} gData;

static void loadBackgroundHandler(void* tData) {
	gData.mBackgroundTexture = loadTexture("assets/misc/best/BG.pkg");
	playOneFrameAnimationLoop(makePosition(0, 0, 1), &gData.mBackgroundTexture);
}

ActorBlueprint BestBackgroundHandler = {
	.mLoad = loadBackgroundHandler,
};