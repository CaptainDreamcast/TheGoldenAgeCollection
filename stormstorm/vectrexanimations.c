#include "vectrexanimations.h"

#include <prism/datastructures.h>
#include <prism/animation.h>
#include <prism/math.h>

typedef struct {
	TextureData mTexture;
	Position mPosition;

	int mHasBasePosition;
	Position* mBasePosition;

} RegisteredAnimation;

typedef struct {
	TextureData* mTexture;
	Position mPosition;
	double mAlpha;
	int mListID;

	int mAnimationID;

	int mHasBasePosition;
	Position* mBasePosition;

	double mAngle;
	Position mCenter;
} ActiveAnimation;

static struct {
	IntMap mRegisteredAnimations;
	List mActiveAnimations;

	TextureData mLineTexture;
	TextureData mDotTexture;
} gData;

static void loadVectrexAnimationHandler(void* tData) {
	(void)tData;
	gData.mRegisteredAnimations = new_int_map();
	gData.mActiveAnimations = new_list();

	gData.mLineTexture = loadTexture("assets/stormstorm/animations/LINE.pkg");
	gData.mDotTexture = loadTexture("assets/stormstorm/animations/DOT.pkg");

}

static void addActiveAnimation(TextureData* tTexture, Position tPosition, double tBaseAlpha, int tHasBasePosition, Position* tBasePosition, double tAngle, Position tCenter) {
	if (tBaseAlpha < 0.1) return;

	ActiveAnimation* e = allocMemory(sizeof(ActiveAnimation));
	e->mTexture = tTexture;
	e->mPosition = tPosition;
	e->mAlpha = randfrom(tBaseAlpha, tBaseAlpha*0.8);
	e->mHasBasePosition = tHasBasePosition;
	e->mBasePosition = tBasePosition;
	e->mAngle = tAngle;
	e->mCenter = tCenter;

	e->mAnimationID = playOneFrameAnimationLoop(e->mPosition, e->mTexture);
	setAnimationTransparency(e->mAnimationID, e->mAlpha);
	if (e->mHasBasePosition) {
		setAnimationBasePositionReference(e->mAnimationID, e->mBasePosition);
	}
	setAnimationRotationZ(e->mAnimationID, e->mAngle, e->mCenter);

	e->mListID = list_push_front_owned(&gData.mActiveAnimations, e);
}

static void updateSingleRegisteredAnimation(void* tCaller, void* tData) {
	(void)tCaller;
	RegisteredAnimation* e = tData;

	addActiveAnimation(&e->mTexture, e->mPosition, 1, e->mHasBasePosition, e->mBasePosition, 0, makePosition(0, 0, 0));
}

static void updateRegisteredAnimations() {
	int_map_map(&gData.mRegisteredAnimations, updateSingleRegisteredAnimation, NULL);
}

static void addUpdatedSingleActiveAnimation(ActiveAnimation* e) {
	addActiveAnimation(e->mTexture, e->mPosition, e->mAlpha*0.2, e->mHasBasePosition, e->mBasePosition, e->mAngle, e->mCenter);
}

static void removeSingleActiveAnimation(ActiveAnimation* e) {
	removeHandledAnimation(e->mAnimationID);
}

static int updateSingleActiveAnimation(void* tCaller, void* tData) {
	(void)tCaller;

	ActiveAnimation* e = tData;
	addUpdatedSingleActiveAnimation(e);
	removeSingleActiveAnimation(e);

	return 1;
}

static void updateActiveAnimations() {
	list_remove_predicate(&gData.mActiveAnimations, updateSingleActiveAnimation, NULL);
}

static void updateVectrexAnimationHandler(void* tData) {
	(void)tData;
	updateActiveAnimations();
	updateRegisteredAnimations();
}

ActorBlueprint VectrexAnimationHandler = {
	.mLoad = loadVectrexAnimationHandler,
	.mUpdate = updateVectrexAnimationHandler,
};

int addVectrexImage(char * tPath, Position tPosition)
{
	RegisteredAnimation* e = allocMemory(sizeof(RegisteredAnimation));
	e->mTexture = loadTexture(tPath);
	e->mPosition = tPosition;
	e->mHasBasePosition = 0;
	return int_map_push_back_owned(&gData.mRegisteredAnimations, e);
}

void setVectrexImageBasePosition(int tID, Position * tBasePosition)
{
	RegisteredAnimation* e = int_map_get(&gData.mRegisteredAnimations, tID);
	e->mHasBasePosition = 1;
	e->mBasePosition = tBasePosition;
}

void addVectrexOneFrameLine(Position tPosition, double tAngle)
{
	addActiveAnimation(&gData.mLineTexture, tPosition, 10, 0, NULL, tAngle, makePosition(0, 5, 0));
}

void addVectrexOneFrameDot(Position tPosition)
{
	addActiveAnimation(&gData.mDotTexture, tPosition, 10, 0, NULL, randfrom(0, 2*M_PI), makePosition(10, 10, 0));
}

void addVectrexOneFrameImage(TextureData * tTextures, Position tPosition)
{
	addActiveAnimation(tTextures, tPosition, 10, 0, NULL, 0, makePosition(0, 0, 0));
}
