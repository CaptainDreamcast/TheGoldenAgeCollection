#include "mirk_collision.h"

#include <tari/collisionhandler.h>

static struct {
	int mShotCollisionList;
	int mMirklingCollisionList;
	int mPreciousPeopleCollisionList;
} gData;

void loadMirkMirklingsCollisions()
{
	gData.mShotCollisionList = addCollisionListToHandler();
	gData.mMirklingCollisionList = addCollisionListToHandler();
	gData.mPreciousPeopleCollisionList = addCollisionListToHandler();
	addCollisionHandlerCheck(gData.mShotCollisionList, gData.mMirklingCollisionList);
	addCollisionHandlerCheck(gData.mPreciousPeopleCollisionList, gData.mMirklingCollisionList);
}

int getMirkShotCollisionList()
{
	return gData.mShotCollisionList;
}

int getMirkMirklingsCollisionList()
{
	return gData.mMirklingCollisionList;
}

int getMirkPreciousPeopleCollisionList()
{
	return gData.mPreciousPeopleCollisionList;
}

MirkCollisionData makeMirkCollisionData(int tType) {
	MirkCollisionData ret;
	ret.mType = tType;
	return ret;
}
