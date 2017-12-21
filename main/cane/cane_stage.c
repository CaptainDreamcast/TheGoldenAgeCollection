#include "cane_stage.h"

#include <stdio.h>

#include <tari/mugenanimationhandler.h>
#include <tari/physicshandler.h>

#include "cane_resources.h"

static struct {

	PhysicsObject backPhysics;
	PhysicsObject frontPhysics;

	int backPhysicsID;
	int frontPhysicsID;

	int hasSavedPhysics;

	double distanceTravelled;
	int mIsScrolling;

} gData;

void loadCaneStage() {

	gData.backPhysicsID = addToPhysicsHandler(makePosition(0, 0, 0));
	gData.frontPhysicsID = addToPhysicsHandler(makePosition(0, 0, 0));

	int animationID = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 100), getCaneSprites(), makePosition(0, 0, 1));
	setMugenAnimationBasePosition(animationID, getHandledPhysicsPositionReference(gData.backPhysicsID));

	animationID = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 100), getCaneSprites(), makePosition(640, 0, 1));
	setMugenAnimationBasePosition(animationID, getHandledPhysicsPositionReference(gData.backPhysicsID));

	animationID = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 101), getCaneSprites(), makePosition(0, 0, 2));
	setMugenAnimationBasePosition(animationID, getHandledPhysicsPositionReference(gData.frontPhysicsID));

	animationID = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 101), getCaneSprites(), makePosition(640, 0, 2));
	setMugenAnimationBasePosition(animationID, getHandledPhysicsPositionReference(gData.frontPhysicsID));

	addAccelerationToHandledPhysics(gData.backPhysicsID, makePosition(-0.1, 0, 0));
	addAccelerationToHandledPhysics(gData.frontPhysicsID, makePosition(-10, 0, 0));
	
	if (gData.hasSavedPhysics) {
		*getPhysicsFromHandler(gData.backPhysicsID) = gData.backPhysics;
		*getPhysicsFromHandler(gData.frontPhysicsID) = gData.frontPhysics;
	}

	gData.mIsScrolling = 1;
}

static void savePhysics() {
	if (!gData.mIsScrolling) return;

	gData.backPhysics = *getPhysicsFromHandler(gData.backPhysicsID);
	gData.frontPhysics = *getPhysicsFromHandler(gData.frontPhysicsID);
	gData.hasSavedPhysics = 1;
}

static void updateSingleScrollingBackground(int physicsID) {
	Position* p = getHandledPhysicsPositionReference(physicsID);

	if (p->x > -640) return;

	p->x += 640;
}

static void updateDistance() {
	if (!gData.mIsScrolling) return;

	double horseSpeedMilesPerTick = 2 * 29.83 / (60 *  60 * 60);
	gData.distanceTravelled += horseSpeedMilesPerTick;
}

void updateCaneStage() {
	updateSingleScrollingBackground(gData.frontPhysicsID);
	updateSingleScrollingBackground(gData.backPhysicsID);

	savePhysics();
	
	updateDistance();
}

double getCaneDistanceTravelledDuringLastScreen() {
	return gData.distanceTravelled;
}

void stopCaneStageScrolling()
{
	stopHandledPhysics(gData.frontPhysicsID);
	stopHandledPhysics(gData.backPhysicsID);

	gData.mIsScrolling = 0;
}

void resetCaneDistanceTravelled() {
	gData.distanceTravelled = 0;
}