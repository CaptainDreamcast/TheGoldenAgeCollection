#include "beyond_titlescreen.h"

#include <stdio.h>

#include <prism/mugenanimationhandler.h>
#include <prism/input.h>
#include <prism/screeneffect.h>

#include "../../maingamemenu.h"

#include "beyond_gamescreen.h"
#include "beyond_bg.h"
#include "beyond_level.h"
#include "beyond_player.h"
#include "beyond_main.h"

static struct {
	MugenSpriteFile mSprites;
	MugenAnimation* mTitleAnimation;
	int mTitleAnimationID;

} gData;

static void loadTitleScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());
	
	char path[1024];
	sprintf(path, "assets/main/%s/title/TITLE.sff", getBeyondDirectory());
	gData.mSprites = loadMugenSpriteFileWithoutPalette(path);
	 
	gData.mTitleAnimation = createOneFrameMugenAnimationForSprite(1, 0);
	gData.mTitleAnimationID = addMugenAnimation(gData.mTitleAnimation, &gData.mSprites, makePosition(0, 0, 10));

	instantiateActor(BeyondBackgroundHandler);
	sprintf(path, "assets/main/%s/title/BG.def", getBeyondDirectory());
	setBeyondBackground(path, &gData.mSprites, NULL);

	addFadeIn(30, NULL, NULL);
}

static void goToGame(void* tCaller) {
	(void)tCaller;
	setBeyondLevelToStart();
	resetBeyondPlayerState();
	setNewScreen(&BeyondGameScreen);
}

static void gotoMainMenuCB(void* tCaller) {
	(void)tCaller;
	setNewScreen(&MainGameMenu);
}

static void updateTitleScreen() {
	if (hasPressedStartFlank()) {
		addFadeOut(30, goToGame, NULL);
	}

	if (hasPressedBFlank()) {
		addFadeOut(30, gotoMainMenuCB, NULL);
	}

	if (hasPressedAbortFlank()) {
		setNewScreen(&MainGameMenu);
	}
}

Screen BeyondTitleScreen = {
	.mLoad = loadTitleScreen,
	.mUpdate = updateTitleScreen,
};
