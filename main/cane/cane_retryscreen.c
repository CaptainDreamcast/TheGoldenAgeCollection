#include "cane_retryscreen.h"

#include <stdlib.h>
#include <math.h>

#include <tari/input.h>
#include <tari/mugenanimationhandler.h>
#include <tari/file.h>
#include <tari/sound.h>

#include "cane_stage.h"
#include "cane_gamescreen.h"
#include "cane_titlescreen.h"
#include "cane_resources.h"

static struct {

	int isTimerFinished;

	Position distancePosition;
	double distanceTravelled;

} gData;

static void timerFinished(void* caller) {
	gData.isTimerFinished = 1;
}

static void addTravelledDistance();

static void loadRetryScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());

	loadCaneResources();

	gData.distanceTravelled = getCaneDistanceTravelledDuringLastScreen();
	

	loadCaneStage();

	addMugenAnimation(getMugenAnimation(getCaneAnimations(), 11), getCaneSprites(), makePosition(60, 210, 3));

	int animationID = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 13), getCaneSprites(), makePosition(300, 300, 3));
	setMugenAnimationNoLoop(animationID);
	setMugenAnimationCallback(animationID, timerFinished, NULL);

	gData.distancePosition = makePosition(170, 100, 10);

	addMugenAnimation(getMugenAnimation(getCaneAnimations(), 12), getCaneSprites(), makePosition(140, 30, 10));
	addTravelledDistance();

	gData.isTimerFinished = 0;

	playTrack(27);
}


static void unloadRetryScreen() {

}


static void updateRetryScreen() {
	updateCaneStage();
}

static void addTravelledDistance() { // TODO: import playbill font properly (will never ever happen though, but hey)
	gData.distanceTravelled = fmin(gData.distanceTravelled, 99.99);
	
	int dist = (int)gData.distanceTravelled;

	int firstDig = (dist / 10) % 10;
	int secondDig = dist % 10;

	double afterDigs = gData.distanceTravelled - dist;

	int firstCommaDig = (int)(afterDigs * 10);
	int secondCommaDig = ((int)(afterDigs * 100)) % 10;
	
	Position p = gData.distancePosition;
	double dx = 64 - 35;

	if (firstDig > 0) {
		addMugenAnimation(createOneFrameMugenAnimationForSprite(11, firstDig), getCaneSprites(), p);
		p.x += dx;
	}
	else {
		p.x += dx / 2;
	}

	addMugenAnimation(createOneFrameMugenAnimationForSprite(11, secondDig), getCaneSprites(), p);
	p.x += dx;

	addMugenAnimation(createOneFrameMugenAnimationForSprite(12, 0), getCaneSprites(), p);
	p.x += dx;

	addMugenAnimation(createOneFrameMugenAnimationForSprite(11, firstCommaDig), getCaneSprites(), p);
	p.x += dx;

	addMugenAnimation(createOneFrameMugenAnimationForSprite(11, secondCommaDig), getCaneSprites(), p);
	p.x += dx;

	p.x += dx;

	addMugenAnimation(getMugenAnimation(getCaneAnimations(), 14), getCaneSprites(), p);
} 

static Screen* getNextRetryScreenScreen() {
	if (hasPressedAbortFlank()) {
		return &CaneTitleScreen;
	}

	else if (hasPressedStartFlank()) {
		resetCaneDistanceTravelled();
		return &CaneGameScreen;
	}
	else if (gData.isTimerFinished) {
		return &CaneTitleScreen;
	}

	return NULL;
}


Screen CaneRetryScreen = {
	.mLoad = loadRetryScreen,
	.mUnload = unloadRetryScreen,
	.mUpdate = updateRetryScreen,
	.mGetNextScreen = getNextRetryScreenScreen
};