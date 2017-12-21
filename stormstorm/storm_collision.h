#pragma once

typedef struct {
	int mListID;

} JusticeCollisionData;

void loadStormCollisions();
int getBallCollisionList();
int getStormPaddleCollisionList();
int getStormEnemyCollisionList();