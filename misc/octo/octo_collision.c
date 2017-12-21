#include "octo_collision.h"

#include <tari/collisionhandler.h>

static struct {
	int mPlayerList;
	int mEnemyList;
} gData;

void setupOctoCollisions()
{
	gData.mPlayerList = addCollisionListToHandler();
	gData.mEnemyList = addCollisionListToHandler();
	addCollisionHandlerCheck(gData.mPlayerList, gData.mEnemyList);
}

int getOctoPlayerCollisionList()
{
	return gData.mPlayerList;
}

int getOctoEnemyCollisionList()
{
	return gData.mEnemyList;
}
