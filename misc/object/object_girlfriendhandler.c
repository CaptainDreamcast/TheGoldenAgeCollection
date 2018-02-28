#include "object_girlfriendhandler.h"

#include <prism/math.h>

#include "object_girlbarhandler.h"

#define MAX_CONVERSATIONS 6
#define MAX_GIRLS 5

typedef struct {
	int mIsActive;

	int mGirlID;
	int mConversationUsed[20];
	int mConversationAmount;

	int mHealth;
	int mMaxHealth;
} Girlfriend;

typedef struct {
	int mIsUsed;
} Girl;

static struct {
	int mGirlfriendAmount;
	Girlfriend mGirlfriends[10];
	Girl mGirls[20];

	MugenSpriteFile mPortraitSprites;
	MugenAnimations mPortraitAnimations;

	MugenSpriteFile mGirlfriendSprites;
	MugenAnimations mGirlfriendAnimations;
} gData;


static char gGirlNames[20][1024] = {
"Rihanna",
"Candice",
"Guiselle",
"Panela",
"Shoeanna",
};

void initObjectGirlfriends()
{
	gData.mGirlfriendAmount = 0;
	int i;
	for (i = 0; i < 10; i++) {
		gData.mGirlfriends[i].mIsActive = 0;
	}

	for (i = 0; i < 20; i++) {
		gData.mGirls[i].mIsUsed = 0;
	}
}


static void loadGirlfriendHandler(void* tCaller) {
	gData.mPortraitSprites = loadMugenSpriteFileWithoutPalette("assets/misc/object/portraits/PORTRAITS.sff");
	gData.mPortraitAnimations = loadMugenAnimationFile("assets/misc/object/portraits/PORTRAITS.air");
}

ActorBlueprint ObjectGirlfriendHandler = {
	.mLoad = loadGirlfriendHandler,
};

int getNewObjectGirlID()
{
	int id;
	
	int i;
	for (i = 0; i < 300; i++) {
		id = randfromInteger(0, MAX_GIRLS - 1);
		if (!gData.mGirls[id].mIsUsed) break;
	}

	return id;
}

void addNewObjectGirlfriend(int tID)
{
	int index = gData.mGirlfriendAmount;
	gData.mGirlfriends[index].mIsActive = 1;
	gData.mGirlfriends[index].mGirlID = tID;
	gData.mGirlfriends[index].mConversationAmount = 0;
	int i;
	for (i = 0; i < MAX_CONVERSATIONS; i++) {
		gData.mGirlfriends[index].mConversationUsed[i] = 0;
	}

	gData.mGirlfriends[index].mMaxHealth = 5;
	gData.mGirlfriends[index].mHealth = gData.mGirlfriends[index].mMaxHealth;

	gData.mGirls[tID].mIsUsed = 1;

	gData.mGirlfriendAmount++;
}

void removeObjectGirlfriend(int tIndex)
{
	gData.mGirls[gData.mGirlfriends[tIndex].mGirlID].mIsUsed = 0;

	int i;
	for (i = tIndex; i < gData.mGirlfriendAmount - 1; i++) {
		gData.mGirlfriends[i] = gData.mGirlfriends[i + 1];
	}
	gData.mGirlfriendAmount--;

	
	gData.mGirlfriends[gData.mGirlfriendAmount - 1].mIsActive = 0;
}

int hasEnoughObjectGirlfriends()
{
	return gData.mGirlfriendAmount == 5;
}

MugenSpriteFile * getObjectGirlfriendPortraitSprites()
{
	return &gData.mPortraitSprites;
}

MugenAnimations * getObjectGirlfriendPortraitAnimations()
{
	return &gData.mPortraitAnimations;
}

MugenAnimation * getObjectGirlfriendPortraitAnimation(int tIndex)
{
	return getMugenAnimation(&gData.mPortraitAnimations, gData.mGirlfriends[tIndex].mGirlID);
}

void loadSpecificObjectGirlfriend(int tGirlID)
{
	char path[1000];
	sprintf(path, "assets/misc/object/girls/%s/%s.sff", gGirlNames[tGirlID], gGirlNames[tGirlID]);
	gData.mGirlfriendSprites = loadMugenSpriteFileWithoutPalette(path);

	sprintf(path, "assets/misc/object/girls/%s/%s.air", gGirlNames[tGirlID], gGirlNames[tGirlID]);
	gData.mGirlfriendAnimations = loadMugenAnimationFile(path);
}

MugenDefScript getObjectGirlfriendConversation(int tGirlfriendID)
{
	int index;
	int i;

	if (gData.mGirlfriends[tGirlfriendID].mConversationAmount >= MAX_CONVERSATIONS) {
		index = randfromInteger(0, MAX_CONVERSATIONS - 1);
	}
	else {
		for (i = 0; i < 100; i++) {
			index = randfromInteger(0, MAX_CONVERSATIONS - 1);
			if (!gData.mGirlfriends[tGirlfriendID].mConversationUsed[index]) break;
		}
	}

	gData.mGirlfriends[tGirlfriendID].mConversationUsed[index] = 1;
	gData.mGirlfriends[tGirlfriendID].mConversationAmount++;

	char path[1000];
	sprintf(path, "assets/misc/object/girls/%s/conversations/%d.def", gGirlNames[gData.mGirlfriends[tGirlfriendID].mGirlID], index);
	return loadMugenDefScript(path);
}

MugenDefScript getObjectGirlfriendQuestion(int tGirlfriendID)
{
	int index;
	int i;

	for (i = 0; i < 100; i++) {
		index = randfromInteger(0, MAX_CONVERSATIONS - 1);
		if (gData.mGirlfriends[tGirlfriendID].mConversationUsed[index]) break;
	}	

	char path[1000];
	sprintf(path, "assets/misc/object/girls/%s/questions/%d.def", gGirlNames[gData.mGirlfriends[tGirlfriendID].mGirlID], index);
	return loadMugenDefScript(path);
}

MugenSpriteFile * getObjectGirlfriendSpecificSprites(int tGirlID)
{
	(void)tGirlID;
	return &gData.mGirlfriendSprites;
}

MugenAnimation * getObjectGirlfriendSpecificAnimation(int tGirlID, int tAnimationID)
{
	(void)tGirlID;
	return getMugenAnimation(&gData.mGirlfriendAnimations, tAnimationID);
}

int getObjectGirlfriendAmount()
{
	return gData.mGirlfriendAmount;
}

int getObjectGirlfriendHealth(int tIndex)
{
	return gData.mGirlfriends[tIndex].mHealth;
}

void increaseObjectGirlfriendHealth(int tIndex)
{
	gData.mGirlfriends[tIndex].mHealth = gData.mGirlfriends[tIndex].mMaxHealth;
	updateObjectBarSizeCall(tIndex);
}

void decreaseObjectGirlfriendHealth(int tIndex)
{
	gData.mGirlfriends[tIndex].mHealth = max(0, gData.mGirlfriends[tIndex].mHealth - 2);
	updateObjectBarSizeCall(tIndex);
}

typedef struct {
	int mHealthMaxDelta;
	int mHealthDelta;
	int mExceptID;
} HealthChangeCaller;


void decreaseObjectGirlfriendHealthExcept(int tIndex)
{
	int i;
	for (i = 0; i < gData.mGirlfriendAmount; i++) {
		if (i == tIndex) continue;
		gData.mGirlfriends[i].mHealth--;
	}
}

void increaseObjectGirlfriendMaxHealthExcept(int tIndex)
{
	int i;
	for (i = 0; i < gData.mGirlfriendAmount; i++) {
		if (i == tIndex) continue;
		gData.mGirlfriends[i].mMaxHealth++;
	}
}

void removeDeadObjectGirlfriends() {
	int i;
	for (i = gData.mGirlfriendAmount - 1; i >= 0; i--) {
		if (gData.mGirlfriends[i].mHealth <= 0) {
			removeObjectGirlfriend(i);
		}
	}
}

int getObjectGirlfriendMaxHealth(int tIndex)
{
	return gData.mGirlfriends[tIndex].mMaxHealth;
}

int getObjectGirlIDFromGirlfriendID(int tIndex)
{
	return gData.mGirlfriends[tIndex].mGirlID;
}

void getObjectGirlfriendName(char * tDst, int tIndex)
{
	strcpy(tDst, gGirlNames[gData.mGirlfriends[tIndex].mGirlID]);
}
