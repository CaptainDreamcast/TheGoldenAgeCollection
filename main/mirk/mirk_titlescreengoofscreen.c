#include "mirk_titlescreengoofscreen.h"

#include <stdio.h>
#include <prism/wrapper.h>
#include <prism/animation.h>
#include <prism/input.h>
#include <prism/soundeffect.h>
#include <prism/sound.h>

#include "mirk_titlescreen.h"

static struct {
	TextureData mBGTextureData;

} gData;

static void loadTitleScreenGoofScreen() {
	gData.mBGTextureData = loadTexture("assets/main/mirk/title/GOOF.pkg");
	playOneFrameAnimationLoop(makePosition(0, 0, 1), &gData.mBGTextureData);

	int sfx = loadSoundEffect("assets/main/mirk/sfx/noise/noise.wav");
	playSoundEffect(sfx);
	pauseTrack();
}

static Screen* getTitleScreenGoofScreenNextScreen() {
	if (hasPressedAbortFlank() || hasPressedStartFlank() || hasPressedAFlank() || hasPressedBFlank()) {
		return &MirkTitleScreen;
	}

	return NULL;
}


Screen MirkTitleScreenGoofScreen = {
	.mLoad = loadTitleScreenGoofScreen,
	.mGetNextScreen = getTitleScreenGoofScreenNextScreen,
};