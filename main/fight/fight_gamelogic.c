#include "fight_gamelogic.h"

#include <assert.h>

#include <prism/screeneffect.h>
#include <prism/wrapper.h>

#include "fight_playerdefinition.h"
#include "fight_fightui.h"
#include "fight_mugenstagehandler.h"
#include "fight_gameoverscreen.h"

static struct {
	int mGameTime;
	int mRoundNumber;
	int mRoundStateNumber;

	int mIsDisplayingIntro;
	int mIsDisplayingWinPose;

	int mRoundNotOverFlag;

	DreamPlayer* mRoundWinner;

	Screen* mNextScreen;

	int mIsInSinglePlayerMode;
} gData;

static void fightAnimationFinishedCB() {
	gData.mRoundStateNumber = 2;
	enableDreamTimer();
}

static void roundAnimationFinishedCB() {
	playDreamFightAnimation(fightAnimationFinishedCB);
}

static void introFinished() {
	gData.mIsDisplayingIntro = 0;
	playDreamRoundAnimation(gData.mRoundNumber, roundAnimationFinishedCB);
}

static void startIntro() {
	gData.mRoundStateNumber = 1;
	gData.mIsDisplayingIntro = 2;
	changePlayerState(getRootPlayer(0), 5900);
	changePlayerState(getRootPlayer(1), 5900);
}

static void fadeInFinished(void* tData) {
	(void)tData;

	/*
	setPlayerControl(getRootPlayer(0), 1);
	setPlayerControl(getRootPlayer(1), 1);
	fightAnimationFinishedCB();
	return;
	*/

	startIntro();
}

static void startRound() {
	disableDreamTimer();
	gData.mRoundStateNumber = 0;
	gData.mIsDisplayingIntro = 0;
	gData.mIsDisplayingWinPose = 0;
	changePlayerState(getRootPlayer(0), 0);
	changePlayerState(getRootPlayer(1), 0);
	setPlayerControl(getRootPlayer(0), 0);
	setPlayerControl(getRootPlayer(1), 0);
	addFadeIn(30, fadeInFinished, NULL);
}

static void resetRound(void*);

static void setWinner() {
	if (getPlayerLife(getRootPlayer(0)) >= getPlayerLife(getRootPlayer(1))) {
		gData.mRoundWinner = getRootPlayer(0);
	}
	else {
		gData.mRoundWinner = getRootPlayer(1);
	}

	gData.mRoundStateNumber = 3;
	setPlayerControl(getRootPlayer(0), 0);
	setPlayerControl(getRootPlayer(1), 0);
}

static void startWinPose();

static void timerFinishedCB() {
	disableDreamTimer();
	setWinner();
	startWinPose();
}

static void loadGameLogic(void* tData) {
	(void)tData;
	setDreamTimeDisplayFinishedCB(timerFinishedCB);

	gData.mGameTime = 0;
	gData.mRoundNumber = 1;
	startRound();
}

static void updateIntro() {
	if (!gData.mIsDisplayingIntro) return;
	
	if (gData.mIsDisplayingIntro == 2) { // TODO: fix call stuff
		gData.mIsDisplayingIntro--;
		return;
	}
	
	int isAFinished = !isPlayerInIntro(getRootPlayer(0));
	int isBFinished = !isPlayerInIntro(getRootPlayer(1));

	if (isAFinished && isBFinished) {

		introFinished();
	}
}

static void goToNextScreen(void* tCaller) {
	(void)tCaller;
	if (!gData.mNextScreen) return;

	setNewScreen(gData.mNextScreen);
}

static void goToGameOverScreen(void* tCaller) {
	(void)tCaller;
	setNewScreen(&DreamGameOverScreen);
}

static void continueAnimationFinishedCB() {
	addFadeOut(30, goToGameOverScreen, NULL);
}

static void resetGameLogic(void* tCaller) {
	(void)tCaller;

	resetRound(NULL);
	resetPlayersEntirely();

	gData.mGameTime = 0;
	gData.mRoundNumber = 1;
}

static void resetGameStart() {
	addFadeOut(30, resetGameLogic, NULL);
}

static void continuePressedCB() {
	resetGameStart();
}

static void startContinue() {
	playDreamContinueAnimation(continueAnimationFinishedCB, continuePressedCB);
}

static void winAnimationFinishedCB() {
	if (gData.mIsInSinglePlayerMode && getPlayerAILevel(gData.mRoundWinner)) {
		startContinue();
	}
	else {
		addFadeOut(30, goToNextScreen, NULL);
	}
}

static void startWinPose() {
	if (hasPlayerStateSelf(gData.mRoundWinner, 180)) {
		changePlayerState(gData.mRoundWinner, 180);

	}

	gData.mRoundStateNumber = 4;
	gData.mIsDisplayingWinPose = 1;
}

static void koAnimationFinishedCB() {
	startWinPose();
}

static void startKO() {
	disableDreamTimer();
	setWinner();
	playDreamKOAnimation(koAnimationFinishedCB);
}

static void updateWinCondition() {
	if (gData.mRoundStateNumber != 2) return;

	int i;
	for (i = 0; i < 2; i++) {

		if (!getPlayerLife(getRootPlayer(i))) {
			startKO();
			break;
		}
	}
}

static void updateNoControl() {
	if (gData.mRoundStateNumber < 3) return;
	setPlayerControl(getRootPlayer(0), 0);
	setPlayerControl(getRootPlayer(1), 0);
}

static void resetRound(void* tCaller) {
	(void)tCaller;
	enableDrawing();
	increasePlayerRoundsExisted();
	gData.mRoundNumber++;
	resetPlayers();
	resetDreamMugenStageHandlerCameraPosition();
	resetDreamTimer();
	startRound();
}

static void updateWinPose() {
	if (!gData.mIsDisplayingWinPose) return;

	if (!getRemainingPlayerAnimationTime(gData.mRoundWinner)) {
		increasePlayerRoundsWon(gData.mRoundWinner);
		if (hasPlayerWon(gData.mRoundWinner)) {
			playDreamWinAnimation(getPlayerDisplayName(gData.mRoundWinner), winAnimationFinishedCB);
		}
		else {
			addFadeOut(30, resetRound, NULL);
		}
		gData.mIsDisplayingWinPose = 0;
	}

}

static void updateGameLogic(void* tData) {
	(void)tData;
	gData.mGameTime++;

	updateIntro();
	updateWinCondition();
	updateNoControl();
	updateWinPose();
}

ActorBlueprint DreamGameLogic = {
	.mLoad = loadGameLogic,
	.mUpdate = updateGameLogic,
};


int getDreamGameTime()
{
	return gData.mGameTime;
}

int getDreamRoundNumber()
{
	return gData.mRoundNumber; 
}

int getDreamRoundStateNumber()
{
	return gData.mRoundStateNumber;
}

int getDreamMatchNumber()
{
	return 1; // TODO
}

int isDreamMatchOver()
{
	return 0; // TODO
}

void setDreamRoundNotOverFlag()
{
	gData.mRoundNotOverFlag = 1; // TODO: use
}

void setDreamScreenAfterFightScreen(Screen * tScreen)
{
	gData.mNextScreen = tScreen;
}

void setDreamGameModeStory()
{
	gData.mIsInSinglePlayerMode = 1;
}

void setDreamGameModeTwoPlayer()
{
	gData.mIsInSinglePlayerMode = 0;
}

