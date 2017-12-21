#pragma once

typedef struct {
	int mList;
} LuckCollisionData;

void setupLuckGameCollisions();

int getLuckHankCollisionList();
int getLuckPirateCollisionList();
int getLuckHankAttackCollisionList();
int getLuckNPCCollisionList();
