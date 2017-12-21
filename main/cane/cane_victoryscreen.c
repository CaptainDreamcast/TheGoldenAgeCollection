#include "cane_victoryscreen.h"

#include <tari/input.h>
#include <tari/mugenanimationhandler.h>
#include <tari/physicshandler.h>
#include <tari/timer.h>
#include <tari/screeneffect.h>

#include "cane_resources.h"
#include "cane_stage.h"
#include "cane_titlescreen.h"
#include "cane_gamescreen.h"


static struct {
	int mPlayerID;
	int mKidnapperID;

	int mBadGuyPhysicsID;
	int mVictoryTextID;

} gData;

static void gotoGameScreen(void* tCaller) {
	setNewScreen(&CaneGameScreen);
}

static void fadeToGameAgain(void* tCaller) {
	addFadeOut(30, gotoGameScreen, NULL);
}

static void setGirlfriendStolen(void* tCaller) {
	(void)tCaller;

	changeMugenAnimation(gData.mPlayerID, getMugenAnimation(getCaneAnimations(), 17));
	changeMugenAnimation(gData.mKidnapperID, getMugenAnimation(getCaneAnimations(), 3));
	
	changeMugenAnimation(gData.mVictoryTextID, getMugenAnimation(getCaneAnimations(), 19));
}

static void loadVictoryScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());

	loadCaneResources();

	loadCaneStage();
	stopCaneStageScrolling();

	gData.mPlayerID = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 16), getCaneSprites(), makePosition(100, 350, 5));
	addMugenAnimation(getMugenAnimation(getCaneAnimations(), 4), getCaneSprites(), makePosition(100, 382, 3));

	gData.mBadGuyPhysicsID = addToPhysicsHandler(makePosition(-200, 350, 0));
	addAccelerationToHandledPhysics(gData.mBadGuyPhysicsID, makePosition(5, 0, 0));

	gData.mKidnapperID = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 15), getCaneSprites(), makePosition(0, 0, 4));
	setMugenAnimationBasePosition(gData.mKidnapperID, getHandledPhysicsPositionReference(gData.mBadGuyPhysicsID));

	gData.mVictoryTextID = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 18), getCaneSprites(), makePosition(320, 240, 20));

	addTimerCB(60, setGirlfriendStolen, NULL);
	addTimerCB(300, fadeToGameAgain, NULL);
}


static void updateVictoryScreen() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&CaneTitleScreen);
	}

	if (hasPressedStartFlank()) {
		fadeToGameAgain(NULL);
	}
}

Screen CaneVictoryScreen = {
	.mLoad = loadVictoryScreen,
	.mUpdate = updateVictoryScreen,
};