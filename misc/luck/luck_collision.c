#include "luck_collision.h"

#include <prism/collisionhandler.h>

static struct {
	int mHankList;
	int mHankAttackList;
	int mPirateList;
	int mNPCCollisionList;
} gData;

void setupLuckGameCollisions()
{
	gData.mHankList = addCollisionListToHandler();
	gData.mHankAttackList = addCollisionListToHandler();
	gData.mPirateList = addCollisionListToHandler();
	gData.mNPCCollisionList = addCollisionListToHandler();

	addCollisionHandlerCheck(gData.mHankList, gData.mPirateList);
	addCollisionHandlerCheck(gData.mHankAttackList, gData.mPirateList);
	addCollisionHandlerCheck(gData.mHankAttackList, gData.mNPCCollisionList);

}

int getLuckHankCollisionList()
{
	return gData.mHankList;
}

int getLuckPirateCollisionList()
{
	return gData.mPirateList;
}

int getLuckHankAttackCollisionList()
{
	return gData.mHankAttackList;
}

int getLuckNPCCollisionList()
{
	return gData.mNPCCollisionList;
}
