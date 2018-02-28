#include "sfmk_fighterselectscreen.h"

#include <stdlib.h>
#include <stdint.h>

#include <prism/input.h>
#include <prism/file.h>
#include <prism/datastructures.h>
#include <prism/geometry.h>
#include <prism/animation.h>
#include <prism/memoryhandler.h>
#include <prism/screeneffect.h>
#include <prism/timer.h>

#include "sfmk_titlescreen.h"
#include "sfmk_animation.h"
#include "sfmk_modemenu.h"
#include "sfmk_storyrollscreen.h"

typedef struct {
	uint16_t mCharacterAmount;
	uint8_t mPadding[46];
} CharacterListHeader;

typedef struct {

	uint8_t Separator1[4];

	char mName[8];

	uint16_t mUp;
	uint16_t mDown;
	uint16_t mLeft;
	uint16_t mRight; 

	uint16_t mPositionX;
	uint16_t mPositionY;
	uint16_t mSizeX;
	uint16_t mSizeY; 

	uint8_t mSeparator2[4];

	char mBackGroundName[8];

	int mTextureSizeX;
	int mTextureSizeY; 

	uint8_t mReservedSpace[16];

} CharacterListEntry;

typedef struct {
	int mIsActive;
	int mWhichSelected;
	int mHasSelected;

	TextureData mBigPortraitTexture;
	int mAnimationID;

	Position mPosition;
	Vector3D mSize;

	VersusAnimationFlickering* mSmallFrameFlickering;
	TextureData mFrameTexture;
	int mFrameAnimationID;
	int mSmallFrameAnimationID;

} PlayerSelection;

typedef struct {
	char mName[1024];
	
	Position mPosition;
	Vector3D mSize;
	TextureData mSmallPortrait;

	int mLeft;
	int mRight;
	int mUp;
	int mDown;

} FighterSelectCharacter;

typedef struct {

	uint16_t mBigPortraitScreenPositionX;
	uint16_t mBigPortraitScreenPositionY;
	uint16_t mBigPortraitSizeX;
	uint16_t mBigPortraitSizeY;

	uint16_t mNameScreenPositionX;
	uint16_t mNameScreenPositionY;
	uint16_t mNameSizeX;
	uint16_t mNameSizeY; 

	uint8_t mReservedSpace[16];

} FighterSelectHeader;

typedef struct {
	uint8_t mThisBonusIsScrolling;
	uint8_t mPositionLeft;
	uint8_t mSeparator1[2];

	uint16_t mScreenPositionX;
	uint16_t mScreenPositionY;

	uint16_t mSizeX;
	uint16_t mSizeY;

	uint16_t mTextureSizeX;
	uint16_t mTextureSizeY; 

	uint8_t mReservedSpace[16];

} FighterSelectSpecialEffect;

static struct {
	Vector mCharacters;
	double mSmallPortraitZ;

	PlayerSelection mSelection[2];

	TextureData mBothSelectedFrame;
	VersusAnimationFlickering* mBothSelectedFlickering;
	int mBothSelectedFrameID;
	int mHasSameSelectionFlag;
	
	TextureData mBG;

	int mCannotSelectSomething;

	Screen* mNextScreen;
	Screen* mReturnScreen;
} gData;


static void loadFighterSelectCharacters() {
	char path[1024];
	sprintf(path, "assets/misc/sfmk/STORY/CHARACTERS.lst");

	Buffer charListBuffer = fileToBuffer(path);
	char* pointer = charListBuffer.mData;

	CharacterListHeader* header = (CharacterListHeader*)pointer;
	pointer += sizeof(CharacterListHeader);

	gData.mCharacters = new_vector();
	gData.mSmallPortraitZ = 2;

	sprintf(path, "assets/misc/sfmk/STORY/SMALLPORTRAITS.img");
	mountRomdisk(path, "SMALLPORTRAITS");

	int i;
	for (i = 0; i < header->mCharacterAmount; i++) {
		CharacterListEntry* entry;
		entry = (CharacterListEntry*)pointer;
		pointer += sizeof(CharacterListEntry);
		
		FighterSelectCharacter* e = vector_push_back_new_buffer(&gData.mCharacters, sizeof(FighterSelectCharacter));
		sprintf(e->mName, "%.8s", entry->mName);
		
		e->mPosition = makePosition(entry->mPositionX, entry->mPositionY, gData.mSmallPortraitZ);
		e->mSize = makePosition(entry->mSizeX, entry->mSizeY, 1);
		e->mLeft = entry->mLeft;
		e->mRight = entry->mRight;
		e->mUp = entry->mUp;
		e->mDown = entry->mDown;

		sprintf(path, "$/SMALLPORTRAITS/%s.pkg", e->mName);
		if (canLoadTexture(path)) {
			e->mSmallPortrait = loadTexture(path);
		}
		else {
			e->mSmallPortrait = loadTexture("assets/misc/sfmk/FALLBACK/SMALLPORTRAIT.pkg");
		}
		int animation = playAnimationLoop(e->mPosition, &e->mSmallPortrait, createOneFrameAnimation(), makeRectangleFromTexture(e->mSmallPortrait));
		setAnimationSize(animation, e->mSize, makePosition(0, 0, 0));		
	}

	unmountRomdisk("SMALLPORTRAITS");
	freeBuffer(charListBuffer);
}

static int hasSelected(int i) {
	return gData.mSelection[i].mHasSelected;
}

static int hasOtherPlayerSelected(int i) {
	return gData.mSelection[(i + 1) % 2].mHasSelected;
}

static int isPlayerActive(int i) {
	return gData.mSelection[i].mIsActive;
}

static int isOtherPlayerActive(int i) {
	return isPlayerActive((i + 1) % 2);
}

static void loadSmallFrame(int i) {
	if (!isPlayerActive(i)) return;

	FighterSelectCharacter* selected = vector_get(&gData.mCharacters, gData.mSelection[i].mWhichSelected);
	Position p = vecAdd(selected->mPosition, makePosition(0, 0, 1));
	gData.mSelection[i].mSmallFrameAnimationID = playAnimationLoop(p, &gData.mSelection[i].mFrameTexture, createOneFrameAnimation(), makeRectangleFromTexture(gData.mSelection[i].mFrameTexture));
	setAnimationSize(gData.mSelection[i].mSmallFrameAnimationID, selected->mSize, makePosition(0, 0, 0));

	if (!hasSelected(i)) {
		gData.mSelection[i].mSmallFrameFlickering = setVersusAnimationFlickering(gData.mSelection[i].mSmallFrameAnimationID, 100);
	}
}

static void unloadSmallFrame(int i) {
	if (!gData.mHasSameSelectionFlag && gData.mSelection[i].mSmallFrameAnimationID != -1) {
		if (gData.mSelection[i].mSmallFrameFlickering != NULL) {
			stopVersusAnimationFlickering(gData.mSelection[i].mSmallFrameFlickering);
			gData.mSelection[i].mSmallFrameFlickering = NULL;
		}
		removeHandledAnimation(gData.mSelection[i].mSmallFrameAnimationID);
		gData.mSelection[i].mSmallFrameAnimationID = -1;
	}
}

static void loadSelection(int i) {
	FighterSelectCharacter* selected = vector_get(&gData.mCharacters, gData.mSelection[i].mWhichSelected);
	
	gData.mSelection[i].mIsActive = 1;

	char path[1024];
	sprintf(path, "$/BIGPORTRAITS/%s.pkg", selected->mName);
	
	if (canLoadTexture(path)) {
		gData.mSelection[i].mBigPortraitTexture = loadTexture(path);
	}
	else {
		gData.mSelection[i].mBigPortraitTexture = loadTexture("assets/misc/sfmk/FALLBACK/BIGPORTRAIT.pkg");
	}
	gData.mSelection[i].mAnimationID = playAnimationLoop(gData.mSelection[i].mPosition, &gData.mSelection[i].mBigPortraitTexture, createOneFrameAnimation(), makeRectangleFromTexture(gData.mSelection[i].mBigPortraitTexture));
	setAnimationSize(gData.mSelection[i].mAnimationID, gData.mSelection[i].mSize, makePosition(0,0,0));

	Position p = vecAdd(gData.mSelection[i].mPosition, makePosition(0, 0, 1));
	gData.mSelection[i].mFrameAnimationID = playAnimationLoop(p, &gData.mSelection[i].mFrameTexture, createOneFrameAnimation(), makeRectangleFromTexture(gData.mSelection[i].mFrameTexture));
	setAnimationSize(gData.mSelection[i].mFrameAnimationID, gData.mSelection[i].mSize, makePosition(0, 0, 0));

	if (!gData.mHasSameSelectionFlag) {
		loadSmallFrame(i);
	}
	
	
}

static void unloadSelection(int i) {
	if (!gData.mSelection[i].mIsActive) return;
	gData.mSelection[i].mIsActive = 0;
	unloadTexture(gData.mSelection[i].mBigPortraitTexture);
	unloadSmallFrame(i);
	removeHandledAnimation(gData.mSelection[i].mFrameAnimationID);
	removeHandledAnimation(gData.mSelection[i].mAnimationID);
}

static void loadSameSelection(int i) {
	if (gData.mHasSameSelectionFlag) return;

	FighterSelectCharacter* selected = vector_get(&gData.mCharacters, gData.mSelection[0].mWhichSelected);
	Position p = vecAdd(selected->mPosition, makePosition(0, 0, 1));
	gData.mBothSelectedFrameID = playAnimationLoop(p, &gData.mBothSelectedFrame, createOneFrameAnimation(), makeRectangleFromTexture(gData.mBothSelectedFrame));
	setAnimationSize(gData.mBothSelectedFrameID, selected->mSize, makePosition(0, 0, 0));
	gData.mBothSelectedFlickering = setVersusAnimationFlickering(gData.mBothSelectedFrameID, 100);

	int other = (i + 1) % 2;
	unloadSmallFrame(i);
	unloadSmallFrame(other);

	gData.mHasSameSelectionFlag = 1;
}

static void unloadSameSelection(int i) {
	if (!gData.mHasSameSelectionFlag) return;

	stopVersusAnimationFlickering(gData.mBothSelectedFlickering);
	removeHandledAnimation(gData.mBothSelectedFrameID);

	gData.mHasSameSelectionFlag = 0;

	int other = (i + 1) % 2;
	loadSmallFrame(i);	
	loadSmallFrame(other);
}

static void updateSameSelection(int i) {
	if (!gData.mSelection[0].mIsActive || !gData.mSelection[1].mIsActive) {
		unloadSameSelection(i);
		return;
	}

	if (gData.mSelection[0].mWhichSelected == gData.mSelection[1].mWhichSelected) {
		loadSameSelection(i);
	}
	else {
		unloadSameSelection(i);
	}
}

static void updateSelection(int i, int tSelection) {
	gData.mSelection[i].mWhichSelected = tSelection;
	unloadSelection(i);
	if (i == 0) {
		loadSelection(i);
	}

	updateSameSelection(i);
}

static void loadPlayerSelectionHeaders() {
	char path[1024];
	sprintf(path, "assets/misc/sfmk/STORY/FIGHTERSELECT.hdr");

	Buffer b = fileToBuffer(path);
	char* pointer = b.mData;
	int i;
	for (i = 0; i < 2; i++) {
		FighterSelectHeader* header = (FighterSelectHeader*)pointer;
		pointer += sizeof(FighterSelectHeader);

		gData.mSelection[i].mPosition = makePosition(header->mBigPortraitScreenPositionX, header->mBigPortraitScreenPositionY, 3);
		gData.mSelection[i].mSize = makePosition(header->mBigPortraitSizeX, header->mBigPortraitSizeY, 1);
		gData.mSelection[i].mWhichSelected = -1;
		gData.mSelection[i].mHasSelected = 0;
		gData.mSelection[i].mIsActive = 0;
		gData.mSelection[i].mSmallFrameFlickering = NULL;
	}
	gData.mHasSameSelectionFlag = 0;

	freeBuffer(b);
}

static void loadPlayerSelection() {
	loadPlayerSelectionHeaders();

	gData.mHasSameSelectionFlag = 0;
	int i;
	for (i = 0; i < 2; i++) {
		updateSelection(i, 0);
	}
}

static void mountRomdisks() {
	char path[1024];
	
	sprintf(path, "assets/misc/sfmk/STORY/BIGPORTRAITS.img");
	mountRomdisk(path, "BIGPORTRAITS");

	sprintf(path, "assets/misc/sfmk/STORY/NAMES.img");
	mountRomdisk(path, "NAMES");

	sprintf(path, "assets/misc/sfmk/STORY/LOGOS.img");
	mountRomdisk(path, "LOGOS");
}

static void unmountRomdisks() {
	unmountRomdisk("LOGOS");
	unmountRomdisk("NAMES");
	unmountRomdisk("BIGPORTRAITS");
}

static void loadTextures() {
	char path[1024];

	sprintf(path, "assets/misc/sfmk/STORY/TEXTURES.img");
	mountRomdisk(path, "TEXTURES");

	gData.mBG = loadTexture("$/TEXTURES/BACKGROUND.pkg");
	playAnimationLoop(makePosition(0, 0, 1), &gData.mBG, createOneFrameAnimation(), makeRectangleFromTexture(gData.mBG));

	int i;
	for (i = 0; i < 2; i++) {
		sprintf(path, "$/TEXTURES/FRAME%d.pkg", i);
		gData.mSelection[i].mFrameTexture = loadTexture(path);
	}
	gData.mBothSelectedFrame = loadTexture("$/TEXTURES/FRAME2.pkg");

	unmountRomdisk("TEXTURES");
}

static void loadSelectScreen() {
	mountRomdisks();
	loadTextures();
	loadFighterSelectCharacters();
	loadPlayerSelection();

	gData.mNextScreen = NULL;
	gData.mReturnScreen = NULL;
	gData.mCannotSelectSomething = 0;
	addVerticalLineFadeIn(20, NULL, NULL);
}

static void unloadSelectScreen() {
	unmountRomdisks();
}

static void fadeOutFinished(void* tCaller) {
	gData.mReturnScreen = gData.mNextScreen;
	setScreenBlack();
}

static void setFadeOut() {
	addFadeOut(20, fadeOutFinished, NULL);
	gData.mCannotSelectSomething = 1;
}

static void setPlayerInactive(int i) {
	updateSelection(i, gData.mSelection[i].mWhichSelected);
}

static void setPlayerActive(int i) {
	updateSelection(i, gData.mSelection[i].mWhichSelected);
}

typedef struct {
	int mID;
} SelectionBlink;

static void selectionFinished(void* tCaller) {
	SelectionBlink* e = tCaller;
	int i = e->mID;
	freeMemory(e);

	if (!isPlayerActive(i) || !hasSelected(i)) return;

	if (gData.mHasSameSelectionFlag) {
		setVersusAnimationFlickeringDuration(gData.mBothSelectedFlickering, 100);
	}

	updateSelection(i, gData.mSelection[i].mWhichSelected);
	
	if (isOtherPlayerActive(i) && !hasOtherPlayerSelected(i)) return;

	FighterSelectCharacter* character = vector_get(&gData.mCharacters, gData.mSelection[i].mWhichSelected);

	setVersusStoryCharacter(character->mName);
	gData.mNextScreen = &VersusStoryRollScreen;
	setFadeOut();
}

static void setSelected(int i) {
	gData.mSelection[i].mHasSelected = 1;
}

static void selectCharacter(int i) {
	if (hasSelected(i)) return;

	if (gData.mHasSameSelectionFlag) {
		setVersusAnimationFlickeringDuration(gData.mBothSelectedFlickering, 5);
	}
	else {
		setVersusAnimationFlickeringDuration(gData.mSelection[i].mSmallFrameFlickering, 5);
	}
	
	setSelected(i);

	SelectionBlink* e = allocMemory(sizeof(SelectionBlink));
	e->mID = i;
	addTimerCB(100, selectionFinished, e);
}

static void unselectCharacter(int i) {

}

static void checkSingleSelectionUpdate(int i, int tNext) {
	if (!isPlayerActive(i) || hasSelected(i)) return;
	updateSelection(i, tNext);
}

static void updateSinglePlayerSelect(int i) {
	FighterSelectCharacter* selected = vector_get(&gData.mCharacters, gData.mSelection[i].mWhichSelected);
	
	if (hasPressedLeftFlankSingle(i)) {
		checkSingleSelectionUpdate(i, selected->mLeft);
	}
	
	if (hasPressedRightFlankSingle(i)) {
		checkSingleSelectionUpdate(i, selected->mRight);
	}

	if (hasPressedUpFlankSingle(i)) {
		checkSingleSelectionUpdate(i, selected->mUp);
	}

	if (hasPressedDownFlankSingle(i)) {
		checkSingleSelectionUpdate(i, selected->mDown);
	}

	if (hasPressedBFlankSingle(i)) {
		if (hasSelected(i)) unselectCharacter(i);
		else if (isOtherPlayerActive(i)) setPlayerInactive(i);
		else {
			gData.mNextScreen = &VersusModeMenu;
			setFadeOut();
		}
	}

	if (i == 0 && hasPressedStartFlankSingle(i)) {
		if (isPlayerActive(i)) {
			selectCharacter(i);
		}
		else {
			setPlayerActive(i);
		}
	}
}

static void updateSelectScreen() {
	if (gData.mCannotSelectSomething) return;

	int i;
	for (i = 0; i < 2; i++) {
		updateSinglePlayerSelect(i);
	}
}

static Screen* getNextSelectScreenScreen() {
	if (hasPressedAbortFlank()) {
		return &VersusTitleScreen;
	}

	return gData.mReturnScreen;
}

Screen VersusFighterSelectScreen = {
	.mLoad = loadSelectScreen,
	.mUnload = unloadSelectScreen,
	.mUpdate = updateSelectScreen,
	.mGetNextScreen = getNextSelectScreenScreen
};
