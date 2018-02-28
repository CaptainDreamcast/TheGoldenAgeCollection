#pragma once

#include <prism/physics.h>

typedef struct {
	int listID;
	Velocity* direction;
} CaneCollisionData;

void loadCaneCollisions();
int getCaneEnemyCollisionListID();
int getCaneBulletCollisionListID();
int getCanePlayerCollisionListID();