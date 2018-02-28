#include "cane_collision.h"

#include <prism/collisionhandler.h>

static struct {
	int enemies;
	int bullets;
	int players;
} gData;

void loadCaneCollisions() {
	gData.enemies = addCollisionListToHandler();
	gData.bullets = addCollisionListToHandler();
	gData.players = addCollisionListToHandler();

	addCollisionHandlerCheck(gData.bullets, gData.enemies);
	addCollisionHandlerCheck(gData.players, gData.enemies);
	setCollisionHandlerOwningColliders();
}

int getCaneEnemyCollisionListID() {
	return gData.enemies;
}

int getCaneBulletCollisionListID() {
	return gData.bullets;
}

int getCanePlayerCollisionListID() {
	return gData.players;
}