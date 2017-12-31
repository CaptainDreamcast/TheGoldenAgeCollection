#include "object_titlescreen.h"

#include <tari/input.h>
#include <tari/mugenanimationhandler.h>
#include <tari/screeneffect.h>
#include <tari/sound.h>

#include "../../miscgamemenu.h"

#include "object_explanationscreen.h"
#include "object_gamelogic.h"

static struct {
	MugenSpriteFile mSprites;
	int mBGID;

} gData;

static void loadTitleScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());
	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/misc/object/title/TITLE.sff");
	gData.mBGID = addMugenAnimation(createOneFrameMugenAnimationForSprite(1, 0), &gData.mSprites, makePosition(0, 0, 1));
	addFadeIn(30, NULL, NULL);
}


static void gotoExplanationScreen(void* tCaller) {
	(void)tCaller;
	startObjectificationGame();
}

static void gotoMiscMenuCB(void* tCaller) {
	(void)tCaller;
	setNewScreen(&MiscGameMenu);
}

static void updateTitleScreen() {
	if (hasPressedBFlank()) {
		addFadeOut(30, gotoMiscMenuCB, NULL);
	}

	if (hasPressedStartFlank()) {
		addFadeOut(30, gotoExplanationScreen, NULL);
	}

	if (hasPressedAbortFlank()) {
		setNewScreen(&MiscGameMenu);
	}
}

Screen ObjectTitleScreen = {
	.mLoad = loadTitleScreen,
	.mUpdate = updateTitleScreen,
};