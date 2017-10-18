#include "vectrexanimations.h"

#include <tari/datastructures.h>
#include <tari/animation.h>
#include <tari/math.h>

typedef struct {
	TextureData mTexture;
	Position mPosition;

} RegisteredAnimation;

typedef struct {
	TextureData* mTexture;
	Position mPosition;
	double mAlpha;
	int mListID;

	int mAnimationID;
} ActiveAnimation;

static struct {
	IntMap mRegisteredAnimations;

	List mActiveAnimations;
} gData;

static void loadVectrexAnimationHandler(void* tData) {
	(void)tData;
	gData.mRegisteredAnimations = new_int_map();
	gData.mActiveAnimations = new_list();
}

static void addActiveAnimation(TextureData* tTexture, Position tPosition, double tBaseAlpha) {
	if (tBaseAlpha < 0.1) return;

	ActiveAnimation* e = allocMemory(sizeof(ActiveAnimation));
	e->mTexture = tTexture;
	e->mPosition = tPosition;
	e->mAlpha = randfrom(tBaseAlpha, tBaseAlpha*0.8);
	
	e->mAnimationID = playOneFrameAnimationLoop(e->mPosition, e->mTexture);
	setAnimationTransparency(e->mAnimationID, e->mAlpha);

	e->mListID = list_push_front_owned(&gData.mActiveAnimations, e);
}

static void updateSingleRegisteredAnimation(void* tCaller, void* tData) {
	(void)tCaller;
	RegisteredAnimation* e = tData;

	addActiveAnimation(&e->mTexture, e->mPosition, 1);
}

static void updateRegisteredAnimations() {
	int_map_map(&gData.mRegisteredAnimations, updateSingleRegisteredAnimation, NULL);
}

static void addUpdatedSingleActiveAnimation(ActiveAnimation* e) {
	addActiveAnimation(e->mTexture, e->mPosition, e->mAlpha*0.2);
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
	return int_map_push_back_owned(&gData.mRegisteredAnimations, e);
}
