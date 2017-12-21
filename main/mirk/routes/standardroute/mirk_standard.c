#include "mirk_standard.h"

#include <stdio.h>
#include <string.h>

#include <tari/animation.h>
#include <tari/math.h>
#include <tari/collisionhandler.h>
#include <tari/texthandler.h>
#include <tari/timer.h>
#include <tari/soundeffect.h>
#include <tari/input.h>
#include <tari/system.h>

#include "../../mirk_preciouspeople.h"
#include "../../mirk_mirklinghandler.h"
#include "../../mirk_collision.h"
#include "../../mirk_soundeffect.h"
#include "../../mirk_player.h"
#include "../../mirk_stage.h"
#include "../../mirk_particles.h"
#include "../../mirk_mirkling.h"
#include "../../mirk_noise.h"
#include "../../mirk_deathcount.h"
#include "../../mirk_pausemenu.h"


static struct {
	char mWaveString[1024];
	int mWaveText;
	char mFunnyString[1024];
	int mFunnyText;
	TextureData mBGTexture;
	int mBG;
	int mIsWaitingToShow;
	int mHasWon;
	int mIsShowingUI;
	int mTimer;

	int mLimit;

	int mIsKeepingPlayerShotPaused;

	void(*mStoppedShowingWaveScreenCB)(void*);
	void* mStoppedShowingWaveScreenCaller;

} gData;


static void pauseGameplay() {
	pauseMirkMirklingGeneration();
	pauseMirkPlayerShooting();
}

static void showNextLevelUIOver(void* tCaller) {
	(void)tCaller;
	removeHandledText(gData.mFunnyText);
	removeHandledText(gData.mWaveText);
	removeHandledAnimation(gData.mBG);
	unloadTexture(gData.mBGTexture);
	gData.mIsShowingUI = 0;
	unpauseMirkMirklingGeneration();
	setMirkPauseMenuPossible();

	if (!gData.mIsKeepingPlayerShotPaused) {
		unpauseMirkPlayerShooting();
	}

	if (gData.mStoppedShowingWaveScreenCB) {
		gData.mStoppedShowingWaveScreenCB(gData.mStoppedShowingWaveScreenCaller);
	}
}

static void showFunnyText() {
	Position p = makePosition(50, 240, 12);
	Vector3D size = makePosition(20, 20, 1);
	Vector3D breakSize = makePosition(-5, 0, 0);
	Vector3D textBoxSize = makePosition(540, 300, 1);
	Duration buildSpeed = 120;

	gData.mFunnyText = addHandledTextWithBuildup(p, gData.mFunnyString, 0, COLOR_WHITE, size, breakSize, textBoxSize, INF, buildSpeed);
}

static void showWaveText() {
	double dx = strlen(gData.mWaveString) * 30;
	Position p = makePosition(320 - dx / 2, 200, 12);
	gData.mWaveText = addHandledTextWithInfiniteDurationOnOneLine(p, gData.mWaveString, 0, COLOR_WHITE, makePosition(30, 30, 1));
}

static void showNextLevelUI() {

	showWaveText();
	showFunnyText();
	setHandledTextSoundEffects(gData.mFunnyText, getMirkTextSoundEffectCollection());
	setMirkPauseMenuImpossible();

	gData.mBG = playAnimationLoop(makePosition(-320, -240, 11), &gData.mBGTexture, createOneFrameAnimation(), makeRectangleFromTexture(gData.mBGTexture));
	setAnimationSize(gData.mBG, makePosition(1280, 960, 1), makePosition(0, 0, 0));
	gData.mIsShowingUI = 1;
	gData.mTimer = addTimerCB(400, showNextLevelUIOver, NULL);
}

void loadMirkStandard()
{
	gData.mBGTexture = loadTexture("assets/main/mirk/text/WAVE_TEXT_BG.pkg");
	pauseGameplay();

	gData.mIsWaitingToShow = 0;
	gData.mHasWon = 0;
	gData.mStoppedShowingWaveScreenCB = NULL;
	gData.mStoppedShowingWaveScreenCaller = NULL;
	gData.mIsKeepingPlayerShotPaused = 0;
	showNextLevelUI();
	resetMirkMirklingAmount();
}

static void showNextLevelCB(void* tCaller) {
	(void)tCaller;
	pauseMirkPlayerShooting();
	gData.mHasWon = 1;
}

void updateMirkStandard()
{
	if (getMirkMirklingAmount() >= gData.mLimit) {
		pauseMirkMirklingGeneration();
		if (!getMirkMirklingAmountOnScreen() && !gData.mIsWaitingToShow) {
			addTimerCB(120, showNextLevelCB, NULL);
			gData.mIsWaitingToShow = 1;
		}
	}

	if (gData.mIsShowingUI && hasPressedStartFlank()) {
		removeTimer(gData.mTimer);
		showNextLevelUIOver(NULL);
	}
}

int hasMirkLostStandard()
{
	return !hasMirkPreciousPeopleLeft();
}

int hasMirkWonStandard()
{
	return gData.mHasWon;
}

void setMirkStandardWaveText(char * tText)
{
	strcpy(gData.mWaveString, tText);
}

void setMirkStandardFunnyText(char * tText)
{
	strcpy(gData.mFunnyString, tText);
}

void setMirkStandardLevelMirklingAmount(int tLimit)
{
	int variance = (int)(0.1*tLimit);
	tLimit = randfromInteger(tLimit-variance, tLimit);

	gData.mLimit = (int)max(1, tLimit*PERFORMANCE_FACTOR);
}

int getStandardLevelMirklingAmount() {
	return gData.mLimit;
}

void setMirkStandardFunnyTextPositionAfterLoad(Position tPos)
{
	setHandledTextPosition(gData.mFunnyText, tPos);
}

void setMirkStandardStoppedShowingWaveScreenCB(void(*tCB)(void *), void * tCaller)
{
	gData.mStoppedShowingWaveScreenCB = tCB;
	gData.mStoppedShowingWaveScreenCaller = tCaller;
}

void setMirkStandardKeepPlayerShotPaused()
{
	gData.mIsKeepingPlayerShotPaused = 1;
}

int getMirkStandardGeneratedMirklingAmount()
{
	return getMirkMirklingAmount();
}

void setMirkGameReal() {
	addMirkScreenNoise(randfromInteger(5, 10));
	setMirkStageReal();
	setMirkPreciousPeopleReal();
	setMirkParticlesReal();
	setMirkMirklingsReal();
	setMirkDeathCountReal();
}

void setMirkGameUnreal() {
	addMirkScreenNoise(randfromInteger(5, 10));
	setMirkStageUnreal();
	setMirkPreciousPeopleUnreal();
	setMirkParticlesUnreal();
	setMirkMirklingsUnreal();
	setMirkDeathCountUnreal();
}
