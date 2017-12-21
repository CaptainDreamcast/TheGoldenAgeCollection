#include "norm_title.h"

#include <tari/animation.h>
#include <tari/physicshandler.h>
#include <tari/input.h>
#include <tari/sound.h>

#include "../../maingamemenu.h"

#include "norm_introscreen.h"

static struct {
	TextureData mTitleTexture;
	int mTitleID;

	int mBGPhysics;
	TextureData mBGTexture;
	int mBGID;

} gData;


static void loadTitleScreen() {
	gData.mTitleTexture = loadTexture("assets/main/norm/sprites/TITLE.pkg");
	gData.mTitleID = playOneFrameAnimationLoop(makePosition(0,0,2),&gData.mTitleTexture);

	gData.mBGPhysics = addToPhysicsHandler(makePosition(0, 0, 1));
	addAccelerationToHandledPhysics(gData.mBGPhysics, makePosition(-1, 0, 0));
	gData.mBGTexture = loadTexture("assets/main/norm/sprites/BG.pkg");
	gData.mBGID = playOneFrameAnimationLoop(makePosition(0, 0, 0), &gData.mBGTexture);
	setAnimationBasePositionReference(gData.mBGID, getHandledPhysicsPositionReference(gData.mBGPhysics));

	playTrack(4);
}

static void updateTitleScreen() {
	Position p = *getHandledPhysicsPositionReference(gData.mBGPhysics);

	Velocity* v = getHandledPhysicsVelocityReference(gData.mBGPhysics);
	if (p.x > -10) v->x = -1;
	if (p.x < -(1000 - 640)) v->x = 1;


	if (hasPressedAbortFlank()) {
		setNewScreen(&MainGameMenu);
	}

	if (hasPressedStartFlank()) {
		setNewScreen(&NormIntroScreen);
	}
}

Screen NormTitleScreen = {
	.mLoad = loadTitleScreen,
	.mUpdate = updateTitleScreen,
};
