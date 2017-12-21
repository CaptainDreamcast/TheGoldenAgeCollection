#include "luck_narrator.h"

#include <tari/animation.h>
#include <tari/texthandler.h>
#include <tari/math.h>

static struct {
	TextureData mBG;
	int mBGID;

	int mIsTextBuiltUp;
	int mIsDisplayingText;
	int mTextID;
	char mText[1024];

} gData;

static void displayNarration() {
	if (gData.mIsTextBuiltUp) {
		gData.mTextID = addHandledText(makePosition(20, 400, 11), gData.mText, 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, -5, 0), makePosition(620, 100, 1), INF);
	}
	else {
		gData.mTextID = addHandledTextWithBuildup(makePosition(20, 400, 11), gData.mText, 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, 0, 0), makePosition(600, 100, 1), INF, strlen(gData.mText) * 5);
	}

}

static void loadNarrator(void* tData) {
	(void)tData;
	gData.mBG = loadTexture("assets/misc/luck/sprites/WHITE.pkg");
	gData.mBGID = playOneFrameAnimationLoop(makePosition(0, 380, 10), &gData.mBG);
	setAnimationSize(gData.mBGID, makePosition(640, 100, 1), makePosition(0, 0, 0));
	setAnimationColorType(gData.mBGID, COLOR_BLACK);

	displayNarration();
	gData.mIsDisplayingText = 1;

}

static void unloadNarrator(void* tData) {
	(void)tData;
	gData.mIsDisplayingText = 0;
}

ActorBlueprint LuckNarratorBP = {
	.mLoad = loadNarrator,
	.mUnload = unloadNarrator,
};

void setLuckNarratorText(char * tText)
{
	strcpy(gData.mText, tText);
	gData.mIsTextBuiltUp = 0;

	if (gData.mIsDisplayingText) {
		removeHandledText(gData.mTextID);
		displayNarration();
	}
}
