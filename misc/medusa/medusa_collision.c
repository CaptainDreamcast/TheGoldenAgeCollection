#include "medusa_collision.h"

#include <prism/collisionhandler.h>

static struct {
	int mBadassList;
	int mShotList;

} gData;

void setupMedusaCollisions()
{
	gData.mBadassList = addCollisionListToHandler();
	gData.mShotList = addCollisionListToHandler();
	addCollisionHandlerCheck(gData.mBadassList, gData.mShotList);
}

int getMedusaBadassCollisionList()
{
	return gData.mBadassList;
}

int getMedusaShotCollisionList()
{
	return gData.mShotList;
}
