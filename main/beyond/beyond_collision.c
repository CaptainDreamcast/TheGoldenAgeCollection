#include "beyond_collision.h"

#include <prism/collisionhandler.h>

static struct {
	int mPlayerCollisionList;
	int mPlayerShotCollisionList;

	int mEnemyCollisionList;
	int mEnemyShotCollisionList;

	int mItemCollisionList;
	int mPlayerItemCollisionList;

} gData;

void loadBeyondCollisions()
{
	gData.mPlayerCollisionList = addCollisionListToHandler();
	gData.mPlayerShotCollisionList = addCollisionListToHandler();
	gData.mEnemyCollisionList = addCollisionListToHandler();
	gData.mEnemyShotCollisionList = addCollisionListToHandler();
	gData.mItemCollisionList = addCollisionListToHandler();
	gData.mPlayerItemCollisionList = addCollisionListToHandler();

	addCollisionHandlerCheck(gData.mPlayerCollisionList, gData.mEnemyShotCollisionList);
	addCollisionHandlerCheck(gData.mPlayerCollisionList, gData.mEnemyCollisionList);
	addCollisionHandlerCheck(gData.mPlayerItemCollisionList, gData.mItemCollisionList);
	addCollisionHandlerCheck(gData.mEnemyCollisionList, gData.mPlayerShotCollisionList);
}

int getBeyondPlayerCollisionList()
{
	return gData.mPlayerCollisionList;
}

int getBeyondPlayerShotCollisionList()
{
	return gData.mPlayerShotCollisionList;
}

int getBeyondEnemyCollisionList()
{
	return gData.mEnemyCollisionList;
}

int getBeyondEnemyShotCollisionList()
{
	return gData.mEnemyShotCollisionList;
}

int getBeyondItemCollisionList()
{
	return gData.mItemCollisionList;
}

int getBeyondPlayerItemCollisionList()
{
	return gData.mPlayerItemCollisionList;
}
