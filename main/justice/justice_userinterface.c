#include "justice_userinterface.h"

#include <prism/animation.h>
#include <prism/input.h>
#include <prism/wrapper.h>

#include "justice_gamestate.h"
#include "justice_stage.h"

static int UI_Z_BACK = 10;
static int UI_Z_FRONT = 11;
static int UI_Z_PAUSE = 12;

static struct {
	Position* screenPositionReference;

	TextureData shadowTexture;
	TextureData healthBackground;
	TextureData health;
	TextureData pause;
	
	int pauseID;

	int healthID;
} gData;

static void showHealth() {
	Position p = makePosition(28, 46, UI_Z_BACK);
	playAnimationLoop(p, &gData.healthBackground, createOneFrameAnimation(), makeRectangleFromTexture(gData.healthBackground));

	p = makePosition(28+74, 46+6, UI_Z_FRONT);
	gData.healthID = playAnimationLoop(p, &gData.health, createOneFrameAnimation(), makeRectangleFromTexture(gData.health));
	setJusticeHealthBarPercentage(getRemainingJusticeHealth() / 1000.0);
}

void loadJusticeUserInterface() {
	gData.screenPositionReference = NULL;
	gData.shadowTexture = loadTexture("sprites/SHADOW.pkg");
	gData.healthBackground = loadTexture("sprites/UI_BACK.pkg");
	gData.health = loadTexture("sprites/LIFEBAR.pkg");
	gData.pause = loadTexture("sprites/PAUSE.pkg");
	gData.pauseID = -1;

	showHealth();
}

static void pauseGame() {
	pauseWrapper();
	gData.pauseID = playAnimationLoop(makePosition(140, 140, UI_Z_PAUSE), &gData.pause, createOneFrameAnimation(), makeRectangleFromTexture(gData.pause));
}

void resumeJusticeGame() {
	if(gData.pauseID == -1) return;
	removeHandledAnimation(gData.pauseID);
	gData.pauseID = -1;
	resumeWrapper();
}

void updateJusticeUserInterface() {
	if (isJusticePlayingBlockingAnimation()) return;

	if(hasPressedStartFlank()) {
		if(gData.pauseID == -1) pauseGame();
		else resumeJusticeGame();
	}
}

int addJusticeShadow(Position* pos, Position center, double scaleX) {
	Position p = makePosition(-(gData.shadowTexture.mTextureSize.x / 2) + 20, 252-(gData.shadowTexture.mTextureSize.y / 2), -1);
	p = vecAdd(p, center);

	int id = playAnimationLoop(p, &gData.shadowTexture, createOneFrameAnimation(), makeRectangleFromTexture(gData.shadowTexture));
	setAnimationScreenPositionReference(id, gData.screenPositionReference);
	setAnimationBasePositionReference(id, pos);
	setAnimationScale(id, makePosition(scaleX, 1, 1), makePosition(32, 0, 0));
	return id;
}

void removeJusticeShadow(int id) {
	removeHandledAnimation(id);

}



void setJusticeUserInterfaceScreenPositionReference(Position* pos) {
	gData.screenPositionReference = pos;
}


void setJusticeHealthBarPercentage(double t) {
	double sizeX = 164*t;
	setAnimationScale(gData.healthID, makePosition(sizeX / 16.0, 16 / 16.0, 1), makePosition(0,0,0));
}

int isJusticeGamePaused()
{
	return gData.pauseID != -1;
}


