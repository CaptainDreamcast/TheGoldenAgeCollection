#include "norm_congratsscreen.h"

#include <tari/animation.h>
#include <tari/input.h>
#include <tari/sound.h>

#include "norm_title.h"

static struct {
	TextureData mBGTexture;
	int mBGID;
} gData;


static void loadGameOverScreen() {
	gData.mBGTexture = loadTexture("assets/main/norm/sprites/GAMEOVER.pkg");
	gData.mBGID = playOneFrameAnimationLoop(makePosition(0,0,1), &gData.mBGTexture);	
	playTrack(4);
}

static Screen* getNextGameOverScreenScreen() {
	if (hasPressedAbortFlank()) {
		return &NormTitleScreen;
	}

	if (hasPressedStartFlank()) {
		return &NormTitleScreen;
	}

	return NULL;
}

Screen NormGameOverScreen = {
	.mLoad = loadGameOverScreen,
	.mGetNextScreen = getNextGameOverScreenScreen,
};