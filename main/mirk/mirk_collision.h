#pragma once

typedef struct {
	int mType;

} MirkCollisionData;

void loadMirkMirklingsCollisions();

int getMirkShotCollisionList();
int getMirkMirklingsCollisionList();
int getMirkPreciousPeopleCollisionList();

MirkCollisionData makeMirkCollisionData(int tType);