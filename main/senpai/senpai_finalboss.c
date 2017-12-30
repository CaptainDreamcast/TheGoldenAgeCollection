#include "senpai_finalboss.h"

#include <tari/input.h>
#include <tari/texthandler.h>
#include <tari/math.h>
#include <tari/screeneffect.h>

#include "senpai_enemyhandler.h"
#include "senpai_creditscreen.h"

static struct {
	int mStage;
	int mTextID;

	int mTextAmount;
	int mIsActive;
} gData;

static char gTexts[][1024] = {
"SO IT HAS COME TO THIS.",
"YOU REACHED THE END.",
"THE END OF THIS MAZE.",
"IN THE END YOU TRULY WERE...",
"SENPAI SCREAMS.",
"WELL, YOU PROBABLY EXPECT SOME HARDCORE FINAL BOSS FIGHT.",
"I WILL NOW PROCEED TO CHARGE MY SUPER MEGA DEATH LASER.",
"WHILE I CHARGE MY SUPER MEGA DEATH LASER...",
"... WHICH WILL BE VERY AMAZING, BY THE WAY...",
"MIND IF I ASK YOU SOMETHING?",
"WHAT EXACTLY ARE YOU SEEING RIGHT NOW?",
"WE BOTH KNOW YOU'RE NOT PLAYING WITH A FULL DECK, JAZZ.",
"I KEEP TELLING YOU A TRILLION TIMES, THERE WAS NEVER ANY ANIME TO BEGIN WITH.",
"... THE SUPER MEGA DEATH LASER? STILL CHARGING.",
"THIS IS SOME SILENT HILL 4 STUFF, DO THEY LOOK LIKE MONSTERS TO YOU ETC.",
"JAZZ, I'M TELLING YOU THIS BECAUSE I TRULY LOVE YOU: PLEASE DON'T DO THIS.",
"DEFEATING THE THREATENING FINAL BOSS LOOKS LIKE IT COULD GET YOU OUT...",
"BUT IT WON'T. YOU'LL BE STUCK IN THIS MAZE.",
"PROBABLY.",
"... THE SUPER MEGA WHAT?",
"OH, THE SUPER MEGA DEATH LASER. YEAH YEAH, CHARGING AND STUFF.",
"SO PLEASE DON'T COME CLOSER.",
"NO, THAT'S THE OPPOSITE OF NOT CLOSER, IT'S ACTUALLY CLOSER.",
"GODDAMMIT JAZZ, THIS ISN'T FUNNY.",
"STAY AWAY!",
"[VARIOUS SOBBING NOISES]",
"PLEASE NO!",
"AAAAAAAAAAAA!",
"I'M DYING!",
"I AM IN THE PROCESS OF DYING!",
"I AM CLOSER TO DYING THAT NOT DYING!",
"I AM NOW DECEASED!",
//"I REALLY LOVED YOU",
};

static void loadFinalBoss(void* tCaller) {
	gData.mIsActive = 0;
	gData.mTextAmount = 32;
}

static void gotoCreditScreen(void* tCaller) {
	setNewScreen(&SenpaiCreditScreen);
}

static void startCredits() {
	addFadeOut(120, gotoCreditScreen, NULL);
}

static void gotoNextText() {
	gData.mStage++;

	if (gData.mStage >= gData.mTextAmount) {
		startCredits();
		return;
	}

	removeHandledText(gData.mTextID);
	gData.mTextID = addHandledTextWithBuildup(makePosition(41, 318, 60), gTexts[gData.mStage], 0, COLOR_WHITE, makePosition(25, 25, 1), makePosition(-5, 0, 0), makePosition(558, 132, 1), INF, strlen(gTexts[gData.mStage]) * 5);
}

static void updateFinalBoss(void* tCaller) {
	if (!gData.mIsActive) return;

	if (hasPressedAFlank()) {
		if (isHandledTextBuiltUp(gData.mTextID)) {
			gotoNextText();
		}
		else {
			setHandledTextBuiltUp(gData.mTextID);
		}
	}
}

ActorBlueprint SenpaiFinalBoss = {
	.mLoad = loadFinalBoss,
	.mUpdate = updateFinalBoss,
};

void startSenpaiFinalBossScene()
{
	addMugenAnimation(getMugenAnimation(getSenpaiEnemyAnimations(), 19), getSenpaiEnemySprites(), makePosition(320, 140, 40));
	addMugenAnimation(getMugenAnimation(getSenpaiEnemyAnimations(), 100), getSenpaiEnemySprites(), makePosition(320, 460, 50));

	gData.mStage = -1;
	gData.mTextID = addHandledTextWithBuildup(makePosition(41, 318, 60), gTexts[0], 0, COLOR_WHITE, makePosition(25, 25, 1), makePosition(-5, 0, 0), makePosition(558, 132, 1), INF, strlen(gTexts[0]) * 5);
	gotoNextText();
	
	gData.mIsActive = 1;

}
