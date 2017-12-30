#include "cane_kidnapper.h"

#include <tari/file.h>
#include <tari/physicshandler.h>
#include <tari/mugenanimationhandler.h>
#include <tari/collisionhandler.h>
#include <tari/screeneffect.h>
#include <tari/math.h>

#include "cane_shadow.h"
#include "cane_resources.h"
#include "cane_collision.h"
#include "cane_titlescreen.h"
#include "cane_victoryscreen.h"

static struct {
	int physicsID;
	int animationID;
	int shadowID;
	int collisionID;
	int mHealth;
	int mMaxHealth;

	int mHealthBarBackID;
	int mHealthBarFrontID;

	Position* pos;

	int isActive;
} gData;

static void kidnapperHitCB(void* tCaller, void* tCollisionData);

void loadCaneKidnapper() {
	
	gData.physicsID = addToPhysicsHandler(makePosition(450, 350, 5));
	gData.pos = &getPhysicsFromHandler(gData.physicsID)->mPosition;
	addAccelerationToHandledPhysics(gData.physicsID, makePosition(2, 0, 0));

	gData.animationID = addMugenAnimation(getMugenAnimation(getCaneAnimations(), 3), getCaneSprites(), makePosition(0, 0, 0));
	setMugenAnimationBasePosition(gData.animationID, gData.pos);
	
	gData.collisionID = addCollisionRectangleToCollisionHandler(getCaneEnemyCollisionListID(), getHandledPhysicsPositionReference(gData.physicsID), makeCollisionRect(makePosition(61, 5, 0), makePosition(109, 53, 1)), kidnapperHitCB, NULL, NULL);

	gData.shadowID = addCaneShadow(gData.pos);
	gData.mMaxHealth = 50;
	gData.mHealth = gData.mMaxHealth;

	gData.mHealthBarBackID = addMugenAnimation(createOneFrameMugenAnimationForSprite(19, 0), getCaneSprites(), makePosition(500, 20, 5));
	setMugenAnimationDrawAngle(gData.mHealthBarBackID, M_PI);
	gData.mHealthBarFrontID = addMugenAnimation(createOneFrameMugenAnimationForSprite(19, 1), getCaneSprites(), makePosition(500, 20, 6));
	setMugenAnimationDrawAngle(gData.mHealthBarFrontID, M_PI);

	gData.isActive = 1;
}

static void stopKidnapper() {
	stopHandledPhysics(gData.physicsID);

	gData.isActive = 0;
}

static void goToVictoryScreenCB(void* tCaller) {
	(void)tCaller;
	setNewScreen(&CaneVictoryScreen);
}

static void kidnapperHitCB(void* tCaller, void* tCollisionData){
	(void)tCaller;
	(void)tCollisionData;

	gData.mHealth--;
	gData.mHealth = max(0, gData.mHealth);
	if (gData.mHealth == 0) {
		addFadeOut(30, goToVictoryScreenCB, NULL);
	}

	int start = 1;
	int end = 300 - 75;

	double factor = gData.mHealth / (double)gData.mMaxHealth;
	int current = (int)(start + (end - start)*factor);

	setMugenAnimationRectangleHeight(gData.mHealthBarFrontID, current);
}

void updateCaneKidnapper() {
	if (!gData.isActive) return;

	if (gData.pos->x > 500) {
		stopKidnapper();
	}


}