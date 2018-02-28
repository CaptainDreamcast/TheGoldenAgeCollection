#include "beyond_itemhandler.h"

#include <prism/mugenspritefilereader.h>
#include <prism/mugenanimationhandler.h>
#include <prism/physicshandler.h>
#include <prism/math.h>

#include "beyond_collision.h"
#include "beyond_main.h"

typedef struct {
	BeyondItemType mType;

	int mAnimationID;
	int mPhysicsID;
	int mListID;

	BeyondCollisionData mCollisionData;
} Item;

static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;

	List mItems;
} gData;

static void loadItemHandler(void* tData) {
	(void)tData;

	gData.mItems = new_list();

	char path[1024];
	sprintf(path, "assets/main/%s/items/ITEMS.sff", getBeyondDirectory());
	gData.mSprites = loadMugenSpriteFileWithoutPalette(path);
	sprintf(path, "assets/main/%s/items/ITEMS.air", getBeyondDirectory());
	gData.mAnimations = loadMugenAnimationFile(path);
}

static void unloadItem(Item* e) {
	removeMugenAnimation(e->mAnimationID);
	removeFromPhysicsHandler(e->mPhysicsID);
}

static int updateSingleItem(void* tCaller, void* tData) {
	(void)tData;
	Item* e = tData;
	Position p = *getHandledPhysicsPositionReference(e->mPhysicsID);
	
	if (p.x < -100) {
		unloadItem(e);
		return 1;
	}
	
	return 0;
}



static void updateItemHandler(void* tData) {
	(void)tData;
	list_remove_predicate(&gData.mItems, updateSingleItem, NULL);
}

ActorBlueprint BeyondItemHandler = {
	.mLoad = loadItemHandler,
	.mUpdate = updateItemHandler,
};

static void itemHitCB(void* tCaller, void* tCollisionData) {
	(void)tCollisionData;
	Item* e = tCaller;
	unloadItem(e);
	list_remove(&gData.mItems, e->mListID);
}

static void addSingleItem(Position tPosition, BeyondItemType tType, int tAnimationNumber) {
	Item* e = allocMemory(sizeof(Item));

	e->mPhysicsID = addToPhysicsHandler(tPosition);
	addAccelerationToHandledPhysics(e->mPhysicsID, makePosition(-2, 0, 0));
	MugenAnimation* animation = getMugenAnimation(&gData.mAnimations, tAnimationNumber);
	e->mAnimationID = addMugenAnimation(animation, &gData.mSprites, makePosition(0, 0, 30));
	setMugenAnimationBasePosition(e->mAnimationID, getHandledPhysicsPositionReference(e->mPhysicsID));

	e->mCollisionData.mCollisionList = getBeyondItemCollisionList();
	e->mCollisionData.mIsItem = 1;
	e->mCollisionData.mItemType = tType;

	setMugenAnimationCollisionActive(e->mAnimationID, getBeyondItemCollisionList(), itemHitCB, e, &e->mCollisionData);

	e->mListID = list_push_back_owned(&gData.mItems, e);
}

static void addItems(Position tPosition, int tAmount, BeyondItemType tType, int tAnimationNumber) {
	int i = 0;
	for (i = 0; i < tAmount; i++) {
		Position p = vecAdd(tPosition, makePosition(randfrom(-10, 10), randfrom(-10, 10), 0));
		addSingleItem(p, tType, tAnimationNumber);
	}
}

void addBeyondSmallPowerItems(Position tPosition, int tAmount)
{
	addItems(tPosition, tAmount, ITEM_TYPE_SMALL_POWER, 1);
}

void addBeyondLifeItems(Position tPosition, int tAmount)
{
	addItems(tPosition, tAmount, ITEM_TYPE_LIFE, 2);
}

void addBeyondBombItems(Position tPosition, int tAmount)
{
	addItems(tPosition, tAmount, ITEM_TYPE_BOMB, 3);
}


