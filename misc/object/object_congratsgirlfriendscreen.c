#include "object_congratsgirlfriendscreen.h"

#include <tari/mugenanimationhandler.h>
#include <tari/timer.h>
#include <tari/screeneffect.h>
#include <tari/input.h>

#include "object_girlfriendhandler.h"
#include "object_congratsscreen.h"
#include "object_titlescreen.h"
#include "object_selectgirlscreen.h"

static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;
	int mGirlID;

	int mBGID;
	int mGirlAnimationID;
	int mCongratsID;
	int mYouGotItID;
} gData;

static void goToNextScreen(void* tCaller) {
	if (hasEnoughObjectGirlfriends()) {
		setNewScreen(&ObjectCongratsScreen);
	}
	else {
		setNewScreen(&ObjectSelectGirlScreen);
	}
}

static void goToNextScreenCB(void* tCaller) {
	addFadeOut(30, goToNextScreen, NULL);
}

static void loadCongratsScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());
	instantiateActor(ObjectGirlfriendHandler);
	addNewObjectGirlfriend(gData.mGirlID);

	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/misc/object/congratsgirl/SPRITES.sff");
	gData.mAnimations = loadMugenAnimationFile("assets/misc/object/congratsgirl/ANIM.air");

	gData.mBGID = addMugenAnimation(createOneFrameMugenAnimationForSprite(0, 1), &gData.mSprites, makePosition(0, 0, 1));
	gData.mGirlAnimationID = addMugenAnimation(getMugenAnimation(getObjectGirlfriendPortraitAnimations(), gData.mGirlID), getObjectGirlfriendPortraitSprites(), makePosition(320, 240, 20));
	setMugenAnimationDrawSize(gData.mGirlAnimationID, makePosition(200, 200, 1));

	gData.mCongratsID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 1), &gData.mSprites, makePosition(320, 240, 10));
	setMugenAnimationDrawScale(gData.mCongratsID, makePosition(5, 5, 1));

	gData.mYouGotItID = addMugenAnimation(createOneFrameMugenAnimationForSprite(2, 1), &gData.mSprites, makePosition(320, 400, 10));

	addTimerCB(400, goToNextScreenCB, NULL);
}

static void updateCongratsScreen() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&ObjectTitleScreen);
	}

	if (hasPressedAFlank() || hasPressedStartFlank()) {
		goToNextScreenCB(NULL);
	}
}

Screen ObjectCongratsGirlfriendScreen = {
	.mLoad = loadCongratsScreen,
	.mUpdate = updateCongratsScreen,
};

void setObjectCongratsGirlfriendID(int tID)
{
	gData.mGirlID = tID;
}
