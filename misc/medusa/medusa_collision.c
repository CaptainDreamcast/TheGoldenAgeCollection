#include "medusa_collision.h"

#include <tari/collisionhandler.h>

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
