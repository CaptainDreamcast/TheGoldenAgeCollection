#pragma once

#include <tari/mugenanimationhandler.h>
#include <tari/actorhandler.h>
#include <tari/mugendefreader.h>

void initObjectGirlfriends();
int getNewObjectGirlID();
void addNewObjectGirlfriend(int tID);
void removeObjectGirlfriend(int tIndex);
int hasEnoughObjectGirlfriends();

MugenSpriteFile* getObjectGirlfriendPortraitSprites();
MugenAnimations* getObjectGirlfriendPortraitAnimations();
MugenAnimation* getObjectGirlfriendPortraitAnimation(int tIndex);

MugenDefScript getObjectGirlfriendConversation(int tGirlfriendID);
MugenDefScript getObjectGirlfriendQuestion(int tGirlfriendID);

void loadSpecificObjectGirlfriend(int tGirlID);

MugenSpriteFile* getObjectGirlfriendSpecificSprites(int tGirlID);
MugenAnimation* getObjectGirlfriendSpecificAnimation(int tGirlID, int tAnimationID);

int getObjectGirlfriendAmount();
int getObjectGirlfriendHealth(int tIndex);
void increaseObjectGirlfriendHealth(int tIndex);
void decreaseObjectGirlfriendHealth(int tIndex);
void decreaseObjectGirlfriendHealthExcept(int tIndex);
void increaseObjectGirlfriendMaxHealthExcept(int tIndex);
void removeDeadObjectGirlfriends();
int getObjectGirlfriendMaxHealth(int tIndex);
int getObjectGirlIDFromGirlfriendID(int tIndex);
void getObjectGirlfriendName(char* tDst, int tIndex);


extern ActorBlueprint ObjectGirlfriendHandler;