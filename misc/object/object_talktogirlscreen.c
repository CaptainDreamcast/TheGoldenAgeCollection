#include "object_talktogirlscreen.h"

#include <tari/input.h>
#include <tari/mugendefreader.h>
#include <tari/log.h>
#include <tari/system.h>
#include <tari/texthandler.h>
#include <tari/math.h>
#include <tari/optionhandler.h>
#include <tari/mugenanimationhandler.h>
#include <tari/timer.h>
#include <tari/screeneffect.h>
#include <tari/animation.h>
#include <tari/sound.h>

#include "object_titlescreen.h"
#include "object_backgroundhandler.h"
#include "object_girlfriendhandler.h"
#include "object_girlbarhandler.h"
#include "object_gameoverscreen.h"
#include "object_congratsgirlfriendscreen.h"
#include "object_clockhandler.h"
#include "object_selectgirlscreen.h"

static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;

	int mGirlfriendID;
	int mGirlID;
	int mStage;

	MugenDefScriptGroup* mCurrentGroup;

	MugenDefScript mConversationScript;
	MugenDefScript mQuestionScript;

	int mHasConversationText;
	int mSpeakerTextID;
	int mMainTextID;

	int mHasPortrait;
	int mPortraitID;

	int mOptionHandlerID;
	int mQuestionText;
	

	int mAnswerID;
	int mAnswerTextID;

	int mWasAnswerCorrect;

	TextureData mWhiteTexture;
	int mBlackBGID;
} gData;

static void startConversation();

static void loadTalkToGirlScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());
	instantiateActor(ObjectBackgroundHandler);
	instantiateActor(ObjectGirlfriendHandler);
	instantiateActor(ObjectClockHandler);
	
	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/misc/object/talk/TALK.sff");
	gData.mAnimations = loadMugenAnimationFile("assets/misc/object/talk/TALK.air");
	
	loadSpecificObjectGirlfriend(gData.mGirlID);
	gData.mConversationScript = getObjectGirlfriendConversation(gData.mGirlfriendID);
	gData.mQuestionScript = getObjectGirlfriendQuestion(gData.mGirlfriendID);
	gData.mHasConversationText = 0;
	gData.mHasPortrait = 0;

	gData.mWhiteTexture = loadTexture("$/rd/effects/white.pkg");

	startConversation();

	playTrack(25);
}

static int isTextGroup() {
	return !strcmp(gData.mCurrentGroup->mName, "Text");
}

static void handleTextGroup() {
	char* speaker = getAllocatedMugenDefStringVariableAsGroup(gData.mCurrentGroup, "speaker");
	char* text = getAllocatedMugenDefStringVariableAsGroup(gData.mCurrentGroup, "text");

	if (gData.mHasConversationText) {
		removeHandledText(gData.mSpeakerTextID);
		removeHandledText(gData.mMainTextID);
	}

	int length = strlen(text);
	gData.mSpeakerTextID = addHandledText(makePosition(20, 320, 50), speaker, 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, -5, 0), makePosition(INF, INF, INF), INF);
	gData.mMainTextID = addHandledTextWithBuildup(makePosition(20, 350, 50), text, 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, 0, 0), makePosition(620, INF, INF), INF, length);
	
	gData.mHasConversationText = 1;

	freeMemory(speaker);
	freeMemory(text);

	gData.mCurrentGroup = gData.mCurrentGroup->mNext;
}

static int isPortraitGroup() {
	return !strcmp(gData.mCurrentGroup->mName, "Portrait");
}

static void handlePortraitGroup() {
	
	if (gData.mHasPortrait) {
		removeMugenAnimation(gData.mPortraitID);
	}

	int animationID = getMugenDefIntegerOrDefaultAsGroup(gData.mCurrentGroup, "animation", 0);
	gData.mPortraitID = addMugenAnimation(getObjectGirlfriendSpecificAnimation(gData.mGirlID, animationID), getObjectGirlfriendSpecificSprites(gData.mGirlID), getMugenDefVectorOrDefaultAsGroup(gData.mCurrentGroup, "position", makePosition(200, 200, 30)));

	gData.mCurrentGroup = gData.mCurrentGroup->mNext;
}

static int isQuestionGroup() {
	return !strcmp(gData.mCurrentGroup->mName, "Question");
}

static void unloadConversation() {
	if (gData.mHasConversationText) {
		removeHandledText(gData.mSpeakerTextID);
		removeHandledText(gData.mMainTextID);
	}

	if (gData.mHasPortrait) {
		removeMugenAnimation(gData.mPortraitID);
	}
}

static void setAnswerStage(int tAnimation, int tTextAnimation) {
	int mAnswerID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, tAnimation), &gData.mSprites, makePosition(320, 150, 50));
	int mAnswerTextID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, tTextAnimation), &gData.mSprites, makePosition(320, 280, 50));

	removeHandledText(gData.mQuestionText);
	removeActor(gData.mOptionHandlerID);

	setObjectGirlBarHandlerSingleGirl(gData.mGirlfriendID);
	setObjectGirlBarHandlerBasePosition(makePosition(300, 350, 50));
	instantiateActor(ObjectGirlBarHandler);
}

static void increaseGirlHealthCB(void* tCaller) {
	increaseObjectGirlfriendHealth(gData.mGirlfriendID);
	gData.mStage = 3;
}



static void decreaseGirlHealthCB(void* tCaller) {
	decreaseObjectGirlfriendHealth(gData.mGirlfriendID);
	gData.mStage = 3;
}

static void selectCorrectAnswerCB(void* tCaller) {
	setAnswerStage(2, 4);

	int healthChange =  abs(getObjectGirlfriendMaxHealth(gData.mGirlfriendID) - getObjectGirlfriendHealth(gData.mGirlfriendID));

	addTimerCB(60, increaseGirlHealthCB, NULL);
	gData.mWasAnswerCorrect = 0;
	gData.mStage = 2;


}

static void selectFalseAnswerCB(void* tCaller) {
	setAnswerStage(3, 5);

	addTimerCB(60, decreaseGirlHealthCB, NULL);
	gData.mWasAnswerCorrect = 0;
	gData.mStage = 2;
}

static void getAnswerIndexArray(int* tIndexArray) {
	int isActive[10];
	int i;
	for (i = 0; i < 4; i++) {
		isActive[i] = 0;
 	}

	for (i = 0; i < 4; i++) {
		while (1) {
			int id = randfromInteger(0, 3);
			if (!isActive[id]) {
				tIndexArray[i] = id;
				isActive[id] = 1;
				break;
			}
		}
	}
}

static void handleQuestionGroup() {

	unloadConversation();

	char* text = getAllocatedMugenDefStringVariable(&gData.mQuestionScript, "Header", "question");
	gData.mQuestionText = addHandledText(makePosition(50, 100, 50), text, 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, -5, 0), makePosition(590, INF, INF), INF);

	freeMemory(text);

	gData.mOptionHandlerID = instantiateActor(getOptionHandlerBlueprint());
	setOptionButtonA();
	setOptionTextSize(20);
	setOptionTextBreakSize(-5);

	int indexArray[10];
	getAnswerIndexArray(indexArray);

	char* answers[4];
	answers[0] = getAllocatedMugenDefStringVariable(&gData.mQuestionScript, "Header", "answer1");
	answers[1] = getAllocatedMugenDefStringVariable(&gData.mQuestionScript, "Header", "answer2");
	answers[2] = getAllocatedMugenDefStringVariable(&gData.mQuestionScript, "Header", "answer3");
	answers[3] = getAllocatedMugenDefStringVariable(&gData.mQuestionScript, "Header", "answer4");	

	double y = 200;
	int i;
	for (i = 0; i < 4; i++) {
		if (indexArray[i] == 0) {
			addOption(makePosition(50, y, 50), answers[indexArray[i]], selectCorrectAnswerCB, NULL);
		}
		else {
			addOption(makePosition(50, y, 50), answers[indexArray[i]], selectFalseAnswerCB, NULL);
		}
		y += 30;
	}

	for (i = 0; i < 4; i++) {
		freeMemory(answers[i]);
	}

	setAnimationPosition(gData.mBlackBGID, makePosition(0, 0, 40));
	setAnimationSize(gData.mBlackBGID, makePosition(640, 480, 1), makePosition(0, 0, 0));

	gData.mStage = 1;
}

static void setNextConversation() {
	while (gData.mCurrentGroup != NULL) {
		if (isTextGroup()) {
			handleTextGroup();
			break;
		}
		else if (isPortraitGroup()) {
			handlePortraitGroup();
		}
		else if (isQuestionGroup()) {
			handleQuestionGroup();
			break;
		}
		else {
			logError("Unrecognized group.");
			logErrorString(gData.mCurrentGroup->mName);
			abortSystem();
		}
	}
}

static void startConversation() {
	gData.mBlackBGID = playOneFrameAnimationLoop(makePosition(0, 300, 40), &gData.mWhiteTexture);
	setAnimationSize(gData.mBlackBGID, makePosition(640, 180, 1), makePosition(0, 0, 0));
	setAnimationColorType(gData.mBlackBGID, COLOR_BLACK);
	setAnimationTransparency(gData.mBlackBGID, 0.6);

	gData.mCurrentGroup = gData.mConversationScript.mFirstGroup;
	gData.mStage = 0;

	setNextConversation();
}

static void updateScreenInput() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&ObjectTitleScreen);
	}
}

static void updateConversation() {
	if (gData.mStage != 0) return;
	
	if (hasPressedAFlank()) {
		if (isHandledTextBuiltUp(gData.mMainTextID)) {
			setNextConversation();
		}
		else {
			setHandledTextBuiltUp(gData.mMainTextID);
		}
	}
}

static void gotoSelectCB(void* tCaller) {
	decreaseObjectGirlfriendHealthExcept(gData.mGirlfriendID);
	removeDeadObjectGirlfriends();

	if (!getObjectGirlfriendAmount()) {
		setNewScreen(&ObjectGameOverScreen);
		return;
	}

	if (increaseObjectClockAndReturnIfNewDay()) {
		increaseObjectGirlfriendMaxHealthExcept(-1);
		setObjectCongratsGirlfriendID(getNewObjectGirlID());
		setNewScreen(&ObjectCongratsGirlfriendScreen);
		return;
	}

	setNewScreen(&ObjectSelectGirlScreen);
}

static void updateGoingToSelect() {
	if (gData.mStage != 3) return;

	if (hasPressedAFlank()) {
		addFadeOut(30, gotoSelectCB, NULL);
	}
}

static void updateTalkToGirlScreen() {
	updateConversation();
	updateScreenInput();
	updateGoingToSelect();
}

Screen ObjectTalkToGirlScreen = {
	.mLoad = loadTalkToGirlScreen,
	.mUpdate = updateTalkToGirlScreen,
};

void setObjectTalkToGirlID(int tIndex)
{
	gData.mGirlfriendID = tIndex;
	gData.mGirlID = getObjectGirlIDFromGirlfriendID(tIndex);
}
