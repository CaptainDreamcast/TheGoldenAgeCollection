#include "mirk_player.h"

#include <prism/input.h>
#include <prism/datastructures.h>
#include <prism/memoryhandler.h>
#include <prism/timer.h>
#include <prism/collisionhandler.h>
#include <prism/soundeffect.h>
#include <prism/stagehandler.h>
#include <prism/wrapper.h>

#include "mirk_collision.h"
#include "mirk_stage.h"
#include "mirk_preciouspeople.h"
#include "mirk_explosion.h"
#include "mirk_mirkling.h"

typedef struct {
	int mCollisionID;
	Position mPosition;
	Collider mCollider;
	MirkCollisionData mCollisionData;
} Shot;

static struct {
	int mCanShoot;

	double mRotationAngle;
	Position mRotationCenter;
} gData;


static void loadPlayer(void* tData) {
	(void)tData;
	gData.mCanShoot = 1;
	gData.mRotationAngle = 0;
	gData.mRotationCenter = makePosition(0, 0, 0);
}

static void shotFinished(void* tCaller) {
	Shot* s = tCaller;
	removeFromCollisionHandler(getMirkShotCollisionList(), s->mCollisionID);
	destroyCollider(&s->mCollider);
	freeMemory(s);
}

static void addShot(Position p, double r) {
	Shot* s = allocMemory(sizeof(Shot));
	s->mPosition = p;
	s->mCollider = makeColliderFromCirc(makeCollisionCirc(makePosition(0,0,0), r));
	s->mCollisionData = makeMirkCollisionData(getMirkShotCollisionList());
	s->mCollisionID = addColliderToCollisionHandler(getMirkShotCollisionList(), &s->mPosition, s->mCollider, NULL, NULL, &s->mCollisionData);
	p.z = 3;
	
	addMirkExplosion(p, r);
	addMirkMirklingDetraction(p, r*0.6);
	addTimerCB(2, shotFinished, s);
}

static Position getAdvancedShotPosition() {
	Position p = getShotPosition();
	p = vecAdd(p, *getMirkStagePositionReference());
	p = vecRotateZAroundCenter(p, gData.mRotationAngle, gData.mRotationCenter);
	return p;
}

static void shoot() {
	Position p = getAdvancedShotPosition();
	addShot(p, 40 * getMirkPreciousPeopleAmount());	
}

static void updatePlayer(void* tData) {
	(void)tData;
	if (!gData.mCanShoot || isWrapperPaused()) return;
	
	if (hasShotGunFlank()) {
		shoot();
	}

}

ActorBlueprint MirkPlayerBP = {
	.mLoad = loadPlayer,
	.mUpdate = updatePlayer
};

void pauseMirkPlayerShooting()
{
	gData.mCanShoot = 0;
}

void unpauseMirkPlayerShooting()
{
	gData.mCanShoot = 1;
}

void setMirkPlayerShotRotation(double tAngle, Vector3D tCenter)
{
	gData.mRotationAngle = tAngle;
	gData.mRotationCenter = tCenter;
}

void addMirkPlayerShotManual(Position p, double r)
{
	addShot(p, r);
}
