#include "octo_bg.h"

#include <tari/mugenanimationhandler.h>
#include <tari/physicshandler.h>

#include "octo_assethandler.h"

static struct {
	int mBGAnimationIDs[3];
	int mPhysicsID;

	MugenAnimation mAnimations[3];

	int mStageSize;
} gData;

static void loadOctoBackground(void* tData) {
	(void)tData;

	gData.mPhysicsID = addToPhysicsHandler(makePosition(0, 0, 0));
	addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(-5, 0, 0));
	
	gData.mStageSize = 853 * 2;

	gData.mBGAnimationIDs[0] = addMugenAnimation(createOneFrameMugenAnimationForSprite(9000, 0), getOctoSprites(), makePosition(0, 0, 1));
	setMugenAnimationBasePosition(gData.mBGAnimationIDs[0], getHandledPhysicsPositionReference(gData.mPhysicsID));
	gData.mBGAnimationIDs[1] = addMugenAnimation(createOneFrameMugenAnimationForSprite(9000, 1), getOctoSprites(), makePosition(gData.mStageSize / 2, 0, 1));
	setMugenAnimationBasePosition(gData.mBGAnimationIDs[1], getHandledPhysicsPositionReference(gData.mPhysicsID));
	gData.mBGAnimationIDs[2] = addMugenAnimation(createOneFrameMugenAnimationForSprite(9000, 0), getOctoSprites(), makePosition(gData.mStageSize, 0, 1));
	setMugenAnimationBasePosition(gData.mBGAnimationIDs[2], getHandledPhysicsPositionReference(gData.mPhysicsID));

}

static void updateOctoBackground(void* tData) {
	Position* pos = getHandledPhysicsPositionReference(gData.mPhysicsID);

	if (pos->x <= -gData.mStageSize) {
		pos->x += gData.mStageSize;
	}
}

ActorBlueprint OctoBackgroundHandler = {
	.mLoad = loadOctoBackground,
	.mUpdate = updateOctoBackground,
};