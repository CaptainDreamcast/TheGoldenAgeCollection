#include "bombx_titlescreen.h"

#include <tari/animation.h>
#include <tari/input.h>
#include <tari/screeneffect.h>

#include "../maingamemenu.h"

#include "bombx_gamescreen.h"
#include "bombx_levelhandler.h"

static struct {
	TextureData mBGTexture;
	int mBGID;

	TextureData mFlameTextures[10];
	Animation mFlameAnimation;
	int mFlameAnimationID;

} gData;

static void loadBombxTitleScreen() {
	gData.mBGTexture = loadTexture("assets/bombx/TITLE.pkg");
	gData.mBGID = playOneFrameAnimationLoop(makePosition(0,0,1), &gData.mBGTexture);

	gData.mFlameAnimation = createEmptyAnimation();
	gData.mFlameAnimation.mFrameAmount = 4;
	gData.mFlameAnimation.mDuration = 5;
	loadConsecutiveTextures(gData.mFlameTextures, "assets/bombx/flame/TITLEFLAME.pkg", gData.mFlameAnimation.mFrameAmount);

	gData.mFlameAnimationID = playAnimationLoop(makePosition(436, 270, 2), gData.mFlameTextures, gData.mFlameAnimation, makeRectangleFromTexture(gData.mFlameTextures[0]));

	addFadeIn(30, NULL, NULL);
}

static void gotoGameScreen(void* tCaller) {
	(void)tCaller;
	resetBombxLevels();
	setNewScreen(&BombxGameScreen);
}

static void gotoMainMenuCB(void* tCaller) {
	(void)tCaller;
	setNewScreen(&MainGameMenu);
}

static void updateBombxTitleScreen() {

	if (hasPressedBFlank()) {
		addFadeOut(30, gotoMainMenuCB, NULL);
	}

	if (hasPressedAbortFlank()) {
		setNewScreen(&MainGameMenu);
	}
	else if (hasPressedStartFlank()) {
		addFadeOut(30, gotoGameScreen, NULL);
	}
}

Screen BombxTitleScreen = {
	.mLoad = loadBombxTitleScreen,
	.mUpdate = updateBombxTitleScreen,
};