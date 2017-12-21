#include "hack_titlescreen.h"

#include <tari/mugenanimationhandler.h>
#include <tari/input.h>
#include <tari/screeneffect.h>

#include "../../miscgamemenu.h"

#include "hack_selectscreen.h"

static struct {
	MugenSpriteFile mSprites;

} gData;

static void loadTitleScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());
	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/misc/hack/sprites/TITLE.sff");
	addMugenAnimation(createOneFrameMugenAnimationForSprite(0, 0), &gData.mSprites, makePosition(0, 0, 1));

	addFadeIn(30, NULL, NULL);
}

static void gotoSelectScreen(void* tCaller) {
	
	setNewScreen(&HackSelectScreen);
}

static void updateTitleScreen() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&MiscGameMenu);
	}

	if (hasPressedStartFlank()) {
		addFadeOut(30, gotoSelectScreen, NULL);
	}
}

Screen HackTitleScreen = {
	.mLoad = loadTitleScreen,
	.mUpdate = updateTitleScreen,
};