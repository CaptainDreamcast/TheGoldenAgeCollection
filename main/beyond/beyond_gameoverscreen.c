#include "beyond_gameoverscreen.h"

#include <prism/screeneffect.h>
#include <prism/animation.h>
#include <prism/input.h>

#include "beyond_titlescreen.h"
#include "beyond_main.h"

static struct {
	TextureData mTexture;
	int mAnimationID;

} gData;

static void loadGameOverScreen() {
	char path[1024];

	sprintf(path, "assets/main/%s/gameover/GAMEOVER.pkg", getBeyondDirectory());
	gData.mTexture = loadTexture(path);
	gData.mAnimationID = playOneFrameAnimationLoop(makePosition(0,0,1), &gData.mTexture);
	addFadeIn(30, NULL, NULL);
}

static void goToTitleScreen(void* tCaller) {
	(void)tCaller;
	setNewScreen(&BeyondTitleScreen);
}

static void updateGameOverScreen() {
	if (hasPressedStartFlank()) {
		addFadeOut(30, goToTitleScreen, NULL);
	}

	if (hasPressedAbortFlank()) {
		setNewScreen(&BeyondTitleScreen);
	}
}

Screen BeyondGameOverScreen = {
	.mLoad = loadGameOverScreen,
	.mUpdate = updateGameOverScreen,
};
