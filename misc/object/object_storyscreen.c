#include "object_storyscreen.h"

#include <assert.h>

#include <prism/log.h>
#include <prism/system.h>
#include <prism/animation.h>
#include <prism/texthandler.h>
#include <prism/math.h>
#include <prism/input.h>
#include <prism/screeneffect.h>
#include <prism/mugendefreader.h>
#include <prism/mugenspritefilereader.h>
#include <prism/mugenanimationreader.h>
#include <prism/mugenanimationhandler.h>
#include <prism/sound.h>

#include "object_titlescreen.h"


static struct {
	MugenDefScript mScript;
	MugenDefScriptGroup* mCurrentGroup;
	MugenSpriteFile mSprites;

	MugenAnimation* mOldAnimation;
	MugenAnimation* mAnimation;
	int mAnimationID;
	int mOldAnimationID;

	Position mOldAnimationBasePosition;
	Position mAnimationBasePosition;

	int mSpeakerID;
	int mTextID;

	int mIsStoryOver;

	char mDefinitionPath[1024];
} gData;

static int isImageGroup() {
	char* name = gData.mCurrentGroup->mName;
	char firstW[100];
	sscanf(name, "%s", firstW);

	return !strcmp("Image", firstW);
}

static void increaseGroup() {
	gData.mCurrentGroup = gData.mCurrentGroup->mNext;
}

static void loadImageGroup() {
	if (gData.mOldAnimationID != -1) {
		removeMugenAnimation(gData.mOldAnimationID);
		destroyMugenAnimation(gData.mOldAnimation);
	}

	if (gData.mAnimationID != -1) {
		setMugenAnimationBasePosition(gData.mAnimationID, &gData.mOldAnimationBasePosition);
	}

	gData.mOldAnimationID = gData.mAnimationID;
	gData.mOldAnimation = gData.mAnimation;
	

	int group = getMugenDefNumberVariableAsGroup(gData.mCurrentGroup, "group");
	int item =  getMugenDefNumberVariableAsGroup(gData.mCurrentGroup, "item");
	gData.mAnimation = createOneFrameMugenAnimationForSprite(group, item);
	gData.mAnimationID = addMugenAnimation(gData.mAnimation, &gData.mSprites, makePosition(0, 0, 0));
	setMugenAnimationBasePosition(gData.mAnimationID, &gData.mAnimationBasePosition);

	increaseGroup();
}

static int isTextGroup() {
	char* name = gData.mCurrentGroup->mName;
	char firstW[100];
	sscanf(name, "%s", firstW);

	return !strcmp("Text", firstW);
}

static void loadTextGroup() {
	if (gData.mTextID != -1) {
		removeHandledText(gData.mTextID);
		removeHandledText(gData.mSpeakerID);
	}

	char* speaker = getAllocatedMugenDefStringVariableAsGroup(gData.mCurrentGroup, "speaker");
	char* text = getAllocatedMugenDefStringVariableAsGroup(gData.mCurrentGroup, "text");

	gData.mSpeakerID = addHandledText(makePosition(40, 340, 3), speaker, 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, 0, 0), makePosition(INF, INF, 1), INF);

	int dur = strlen(text);
	gData.mTextID = addHandledTextWithBuildup(makePosition(50, 360, 3), text, 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, 0, 0), makePosition(540, 480, 1), INF, dur);
	
	freeMemory(speaker);
	freeMemory(text);

	increaseGroup();
}

static int isTitleGroup() {
	char* name = gData.mCurrentGroup->mName;
	char firstW[100];
	sscanf(name, "%s", firstW);

	return !strcmp("Title", firstW);
}

static void goToTitle(void* tCaller) {
	(void)tCaller;
	setNewScreen(&ObjectTitleScreen);
}

static void loadTitleGroup() {
	gData.mIsStoryOver = 1;
	addFadeOut(30, goToTitle, NULL);
}

static void loadNextStoryGroup() {
	int isRunning = 1;
	while (isRunning) {
		if (isImageGroup()) {
			loadImageGroup();
		}
		else if (isTextGroup()) {
			loadTextGroup();
			break;
		}
		else if (isTitleGroup()) {
			loadTitleGroup();
			break;
		}
		else {
			logError("Unidentified group type.");
			logErrorString(gData.mCurrentGroup->mName);
			abortSystem();
		}
	}
}

static void findStartOfStoryBoard() {
	gData.mCurrentGroup = gData.mScript.mFirstGroup;

	while (gData.mCurrentGroup && strcmp("STORYSTART", gData.mCurrentGroup->mName)) {
		gData.mCurrentGroup = gData.mCurrentGroup->mNext;
	}

	assert(gData.mCurrentGroup);
	gData.mCurrentGroup = gData.mCurrentGroup->mNext;
	assert(gData.mCurrentGroup);

	gData.mAnimationID = -1;
	gData.mOldAnimationID = -1;
	gData.mTextID = -1;

	gData.mOldAnimationBasePosition = makePosition(0, 0, 1);
	gData.mAnimationBasePosition = makePosition(0, 0, 2);

	loadNextStoryGroup();
}



static void loadStoryScreen() {
	gData.mIsStoryOver = 0;
	
	instantiateActor(getMugenAnimationHandlerActorBlueprint());

	gData.mScript = loadMugenDefScript(gData.mDefinitionPath);

	char* spritePath = getAllocatedMugenDefStringVariable(&gData.mScript, "Header", "sprites");
	gData.mSprites = loadMugenSpriteFileWithoutPalette(spritePath);
	freeMemory(spritePath);

	findStartOfStoryBoard();

	playTrack(25);
}


static void updateText() {
	if (gData.mIsStoryOver) return;
	if (gData.mTextID == -1) return;

	if (hasPressedAFlankSingle(0) || hasPressedAFlankSingle(1) || hasPressedStartSingle(0) || hasPressedStartSingle(1)) {
		if (isHandledTextBuiltUp(gData.mTextID)) {
			loadNextStoryGroup();
		}
		else {
			setHandledTextBuiltUp(gData.mTextID);
		}
	}
}

static void updateStoryScreen() {

	updateText();

	if (hasPressedAbortFlank()) {
		setNewScreen(&ObjectTitleScreen);
	}

}


Screen ObjectStoryScreen = {
	.mLoad = loadStoryScreen,
	.mUpdate = updateStoryScreen,
};


void setCurrentObjectStoryDefinitionFile(char* tPath) {
	strcpy(gData.mDefinitionPath, tPath);
}