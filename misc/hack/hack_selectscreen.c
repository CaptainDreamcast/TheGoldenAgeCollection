#include "hack_selectscreen.h"

#include <prism/mugenanimationhandler.h>
#include <prism/input.h>
#include <prism/screeneffect.h>
#include <prism/sound.h>

#include "hack_titlescreen.h"
#include "hack_performancehandler.h"
#include "hack_hackscreen.h"
#include "hack_projectlogic.h"

static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;
	int mCurrentCharacter;
	int mCharacterID;
	int mPerformanceID;

	Vector3DI mPerformances[4];
} gData;

static void addCharacter();

static void loadTitleScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());
	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/misc/hack/sprites/SELECT.sff");
	gData.mAnimations = loadMugenAnimationFile("assets/misc/hack/sprites/SELECT.air");
	addMugenAnimation(createOneFrameMugenAnimationForSprite(0, 0), &gData.mSprites, makePosition(0, 0, 1));
	addMugenAnimation(createOneFrameMugenAnimationForSprite(0, 1), &gData.mSprites, makePosition(0, 0, 3));

	gData.mPerformances[0] = makeVector3DI(5, 3, 1);
	gData.mPerformances[1] = makeVector3DI(3, 5, 1);
	gData.mPerformances[2] = makeVector3DI(20, 20, 20);

	gData.mCurrentCharacter = 0;
	addCharacter();


	addFadeIn(30, NULL, NULL);

	playTrack(24);
}

static void addCharacter() {
	gData.mCharacterID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, gData.mCurrentCharacter+10), &gData.mSprites, makePosition(320, 480, 2));
	addHackPerformance(makePosition(175, 384, 4), gData.mPerformances[gData.mCurrentCharacter], &gData.mSprites, &gData.mAnimations);
}

static void removeCharacter() {
	removeMugenAnimation(gData.mCharacterID);
	removeHackPerformance();
}

static void changeCharacter(int tDelta) {
	removeCharacter();
	gData.mCurrentCharacter += tDelta;
	if (gData.mCurrentCharacter < 0) gData.mCurrentCharacter += 3;
	if (gData.mCurrentCharacter >= 3) gData.mCurrentCharacter -= 3;
	addCharacter();
}

static void gotoHackScreen(void* tCaller) {
	resetHackProject();
	setNewScreen(&HackHackScreen);
}

static void updateTitleScreen() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&HackTitleScreen);
	}

	if (hasPressedLeftFlank()) {
		changeCharacter(-1);
	}

	if (hasPressedRightFlank()) {
		changeCharacter(1);
	}

	if ((gData.mCurrentCharacter != 2) && (hasPressedAFlank() || hasPressedStartFlank())) {
		addFadeOut(30, gotoHackScreen, NULL);
	}
}

Screen HackSelectScreen = {
	.mLoad = loadTitleScreen,
	.mUpdate = updateTitleScreen,
};