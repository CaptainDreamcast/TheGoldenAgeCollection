#pragma once

typedef enum {
	ITEM_TYPE_SMALL_POWER,
	ITEM_TYPE_LIFE,
	ITEM_TYPE_BOMB,

} BeyondItemType;

typedef struct {
	int mCollisionList;

	int mIsItem;
	BeyondItemType mItemType;
} BeyondCollisionData;

void loadBeyondCollisions();

int getBeyondPlayerCollisionList();
int getBeyondPlayerShotCollisionList();
int getBeyondEnemyCollisionList();
int getBeyondEnemyShotCollisionList();
int getBeyondItemCollisionList();
int getBeyondPlayerItemCollisionList();