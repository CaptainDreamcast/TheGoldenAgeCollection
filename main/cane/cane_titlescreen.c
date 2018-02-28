#include "cane_titlescreen.h"

#include <stdlib.h>

#include <prism/input.h>
#include <prism/animation.h>
#include <prism/file.h>
#include <prism/mugenanimationhandler.h>
#include <prism/screeneffect.h>
#include <prism/sound.h>

#include "../../maingamemenu.h"

#include "cane_stage.h"
#include "cane_gamescreen.h"
#include "cane_resources.h"


static struct {

	TextureData pressStart;

	int mAnimationHandler;
} gData;

static void loadTitleScreen() {
	gData.mAnimationHandler = instantiateActor(getMugenAnimationHandlerActorBlueprint());

	loadCaneResources();
	loadCaneStage();

	addMugenAnimation(getMugenAnimation(getCaneAnimations(), 1), getCaneSprites(), makePosition(100, 350, 5));
	addMugenAnimation(getMugenAnimation(getCaneAnimations(), 4), getCaneSprites(), makePosition(100, 382, 3));

	addMugenAnimation(getMugenAnimation(getCaneAnimations(), 3), getCaneSprites(), makePosition(450, 350, 5));
	addMugenAnimation(getMugenAnimation(getCaneAnimations(), 4), getCaneSprites(), makePosition(450, 382, 3));

	
	addMugenAnimation(createOneFrameMugenAnimationForSprite(17, 1), getCaneSprites(), makePosition(60, 80, 3));
	addMugenAnimation(createOneFrameMugenAnimationForSprite(17, 0), getCaneSprites(), makePosition(60, 150, 3));

	playTrack(27);
}


static void unloadTitleScreen() {
}


static void updateTitleScreen() {
	updateCaneStage();
}

static void drawTitleScreen() {

}

static void stopCane() {
	removeActor(gData.mAnimationHandler);
	shutdownCaneResources();
}

static void gotoMainMenuCB(void* tCaller) {
	(void)tCaller;
	stopCane();
	setNewScreen(&MainGameMenu);
}


static Screen* getNextTitleScreenScreen() {
	if (hasPressedBFlank()) {
		addFadeOut(30, gotoMainMenuCB, NULL);
	}
	
	if (hasPressedAbortFlank()) {
		stopCane();
		return &MainGameMenu;
	}

	else if (hasPressedStartFlank()) {
		resetCaneDistanceTravelled();
		return &CaneGameScreen;
	}

	return NULL;
}


Screen CaneTitleScreen = {
	.mLoad = loadTitleScreen,
	.mUnload = unloadTitleScreen,
	.mUpdate = updateTitleScreen,
	.mDraw = drawTitleScreen,
	.mGetNextScreen = getNextTitleScreenScreen
};