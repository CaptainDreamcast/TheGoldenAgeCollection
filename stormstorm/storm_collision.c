#include "storm_collision.h"

#include <prism/collisionhandler.h>

static struct {
	int mPaddleList;
	int mBallList;

	int mEnemyList;
} gData;

void loadStormCollisions()
{
	gData.mPaddleList = addCollisionListToHandler();
	gData.mBallList = addCollisionListToHandler();
	gData.mEnemyList = addCollisionListToHandler();
	addCollisionHandlerCheck(gData.mPaddleList, gData.mBallList);
	addCollisionHandlerCheck(gData.mBallList, gData.mEnemyList);
}

int getBallCollisionList()
{
	return gData.mBallList;
}

int getStormPaddleCollisionList()
{
	return gData.mPaddleList;
}

int getStormEnemyCollisionList()
{
	return gData.mEnemyList;
}
