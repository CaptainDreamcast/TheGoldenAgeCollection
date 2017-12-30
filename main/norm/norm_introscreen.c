#include "norm_introscreen.h"

#include <tari/optionhandler.h>
#include <tari/texthandler.h>
#include <tari/math.h>
#include <tari/timer.h>
#include <tari/input.h>
#include <tari/sound.h>
#include <tari/soundeffect.h>

#include "norm_gamescreen.h"
#include "norm_title.h"

static struct {
	int mStage;

	int mNarrationID;
	int mOptionID1;
	int mOptionID2;

	int mStageAmount;
	int mTimer;

	int mHasSelected; // TODO: fix list
	int mClickSoundEffect;
	
	Duration mBuildUpDuration;
} gData;


static char gNarration[][1024] = {
	"Super-Mega-Ultra-Combo Man is the strongest super hero of all time. However, he loathes being a super hero and longs for a normal life.",
	"There is only one way to solve his predicament: Beat up people until he is out of Super-Mega-Ultra-Combo energy.",
	"Being a true super hero, SMUC Man grows stronger when people hug him lovingly. Avoid this at all costs!",
	"You can punch people by pressing the button appearing at the top of the screen! SMUC Man will slowly turn into SCUM Man!",
	"The game will get harder as SCUM man gets weaker! Also, his fans will try even harder to hug him in order to support him!",
	"Do your best, and make sure to play this normal game in a normal way! Enjoy yourself (normally).",
};

static char gOption1[][1024] = {
	"What a great setting!",
	"That makes sense.",
	"Avoiding signs of affection is my talent!",
	"I want to be SCUM too!",
	"Go SCUM Man!",
	"Thanks for this super-duper-mega amazing game!",
};

static char gOption2[][1024] = {
	"Oi, isn't this just a budget One-P*nch Man?",
	"NONONONO, THIS DOESN'T MAKE SENSE AT ALL DAMMIT!",
	"That's just not how it works.",
	"SCUM Man? What the hell is a SCUM Man?",
	"He's beating up his fans?!",
	"Let's just get this over with.",
};

static void hasBuiltUpText(void* tCaller);

static void buildUpText() {
	gData.mStage++;

	gData.mNarrationID = addHandledTextWithBuildup(makePosition(40, 60, 2), gNarration[gData.mStage / 2], 0, COLOR_WHITE, makePosition(30, 30, 1), makePosition(-5, 0, 0), makePosition(560, 400, 1), INF, gData.mBuildUpDuration);
	gData.mTimer = addTimerCB(gData.mBuildUpDuration, hasBuiltUpText, NULL);
}

static void hasSelectedOption(void* tCaller) {
	(void)tCaller;
	gData.mHasSelected = 1;
	playSoundEffect(gData.mClickSoundEffect);
}

static void performSelection() {
	gData.mHasSelected = 0;

	if (gData.mStage == 11) {
		setNewScreen(&NormGameScreen);
	}

	removeHandledText(gData.mNarrationID);
	removeOption(gData.mOptionID2);
	removeOption(gData.mOptionID1);

	buildUpText();

}

static void hasBuiltUpText(void* tCaller) {
	(void)tCaller;

	gData.mStage++;

	gData.mOptionID1 = addOption(makePosition(50, 350, 2), gOption1[gData.mStage / 2], hasSelectedOption, NULL);
	gData.mOptionID2 = addOption(makePosition(50, 380, 2), gOption2[gData.mStage / 2], hasSelectedOption, NULL);
}

static void loadIntroScreen() {
	instantiateActor(getOptionHandlerBlueprint());
	setOptionTextBreakSize(-5);
	setOptionButtonA();
	setOptionButtonStart();
	setOptionTextSize(16);


	gData.mStageAmount = 5;
	gData.mStage = -1;
	gData.mBuildUpDuration = 200;
	gData.mClickSoundEffect = loadSoundEffect("assets/main/norm/music/CLICK.wav");

	buildUpText();
}

static void setTextBuiltUp() {
	removeTimer(gData.mTimer);
	removeHandledText(gData.mNarrationID);
	gData.mNarrationID = addHandledText(makePosition(40, 60, 2), gNarration[gData.mStage / 2], 0, COLOR_WHITE, makePosition(30, 30, 1), makePosition(-5, 0, 0), makePosition(560, 400, 1), INF);

	hasBuiltUpText(NULL);
}

static void updateIntroScreen() {	
	if (gData.mHasSelected) {
		performSelection();
		return;
	}



	if (gData.mStage % 2) return;

	if (hasPressedAFlank() || hasPressedStartFlank()) {
		setTextBuiltUp();
	}

}

static Screen* getNextIntroScreenScreen() {

	if (hasPressedAbortFlank()) {
		return &NormTitleScreen;
	}

	return NULL;
}

Screen NormIntroScreen = {
	.mLoad = loadIntroScreen,
	.mUpdate = updateIntroScreen,
	.mGetNextScreen = getNextIntroScreenScreen,
};
