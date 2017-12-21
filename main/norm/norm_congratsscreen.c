#include "norm_congratsscreen.h"

#include <tari/animation.h>
#include <tari/input.h>
#include <tari/sound.h>

#include "norm_title.h"

static struct {
	TextureData mBGTexture;
	int mBGID;
} gData;


static void loadCongratsScreen() {
	gData.mBGTexture = loadTexture("assets/main/norm/sprites/CONGRATS.pkg");
	gData.mBGID = playOneFrameAnimationLoop(makePosition(0,0,1), &gData.mBGTexture);	
	playTrack(4);
}

static Screen* getNextCongratsScreenScreen() {
	if (hasPressedAbortFlank()) {
		return &NormTitleScreen;
	}

	if (hasPressedStartFlank()) {
		return &NormTitleScreen;
	}

	return NULL;
}

Screen NormCongratsScreen = {
	.mLoad = loadCongratsScreen,
	.mGetNextScreen = getNextCongratsScreenScreen,
};