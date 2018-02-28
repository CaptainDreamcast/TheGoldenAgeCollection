#include "object_selectgirlscreen.h"

#include <prism/mugenanimationhandler.h>
#include <prism/input.h>
#include <prism/optionhandler.h>
#include <prism/texthandler.h>
#include <prism/math.h>
#include <prism/screeneffect.h>
#include <prism/animation.h>
#include <prism/sound.h>

#include "object_girlfriendhandler.h"
#include "object_girlbarhandler.h"
#include "object_titlescreen.h"
#include "object_clockhandler.h"
#include "object_talktogirlscreen.h"

static struct {
	MugenSpriteFile mSprites;

	int mBGID;

	TextureData mWhiteTexture;
	int mBlackBGID;
} gData;

static void gotoTalkScreen(void* tCaller) {
	(void)tCaller;
	setNewScreen(&ObjectTalkToGirlScreen);
}

static void selectCB(void* tCaller) {
	int i = (int)tCaller;
	setObjectTalkToGirlID(i);
	addFadeOut(30, gotoTalkScreen, NULL);
}

static void loadSelection() {
	Position pos = makePosition(60, 100, 20);

	gData.mWhiteTexture = loadTexture("$/rd/effects/white.pkg");
	gData.mBlackBGID = playOneFrameAnimationLoop(vecAdd(pos, makePosition(-30, -10, -1)), &gData.mWhiteTexture);
	setAnimationSize(gData.mBlackBGID, makePosition(320, 180, 1), makePosition(0, 0, 0));
	setAnimationColorType(gData.mBlackBGID, COLOR_BLACK);
	setAnimationTransparency(gData.mBlackBGID, 0.6);

	addHandledText(pos, "Spend time with...", 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, -5, 0), makePosition(INF, INF, INF), INF);


	instantiateActor(getOptionHandlerBlueprint());
	setOptionButtonA();
	setOptionTextSize(20);
	setOptionTextBreakSize(-5);

	pos = vecAdd(pos, makePosition(0, 10, 0));

	int i;
	for (i = 0; i < getObjectGirlfriendAmount(); i++) {

		char girlfriendName[100];
		getObjectGirlfriendName(girlfriendName, i);

		addOption(vecAdd(pos, makePosition(0, (i+1)*30, 0)), girlfriendName, selectCB, (void*)i);
	}

	playTrack(25);
}


static void loadSelectScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());

	instantiateActor(ObjectGirlfriendHandler);
	setObjectGirlBarHandlerAllGirls();
	setObjectGirlBarHandlerBasePosition(makePosition(500, 300, 20));
	instantiateActor(ObjectGirlBarHandler);
	instantiateActor(ObjectClockHandler);

	loadSelection();



	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/misc/object/select/SELECT.sff");
	gData.mBGID = addMugenAnimation(createOneFrameMugenAnimationForSprite(0, 1), &gData.mSprites, makePosition(0, 0, 1));
}

static void updateSelectScreen() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&ObjectTitleScreen);
	}
}

Screen ObjectSelectGirlScreen = {
	.mLoad = loadSelectScreen,
	.mUpdate = updateSelectScreen,
};