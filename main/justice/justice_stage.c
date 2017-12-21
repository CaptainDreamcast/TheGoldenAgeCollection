#include "justice_stage.h"

#include <string.h>
#include <stdio.h>

#include <tari/geometry.h>
#include <tari/script.h>
#include <tari/log.h>
#include <tari/system.h>
#include <tari/collisionhandler.h>
#include <tari/memoryhandler.h>
#include <tari/datastructures.h>
#include <tari/timer.h>
#include <tari/mugenanimationhandler.h>
#include <tari/input.h>

#include "justice_enemies.h"
#include "justice_player.h"
#include "justice_userinterface.h"
#include "justice_gamestate.h"
#include "justice_system.h"


static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;

	Script script;
	ScriptPosition scriptPosition;

	int isWaitingForDefeat;
	int isWaitingForAnimation;
	int isWaitingForDuration;
	int isBackgroundScrolling;

	int isOver;

	int isScrollingRightForced;

	Position mScreenPosition;
	Position mInvertedScreenPosition;
	int mBackgroundAmount;

	int mBlockingAnimationID;

} gData;


static ScriptPosition loadBackgroundElementWithoutAnimation(ScriptPosition position, char* path, Position* texturePosition) {	
	char name[100];
	position = getNextScriptString(position, name);
	sprintf(path, "sprites/%s.pkg", name);
		
	position = getNextScriptDouble(position, &texturePosition->x);
	position = getNextScriptDouble(position, &texturePosition->y);
	return position;
}

static ScriptPosition loadBackgroundElementAnimation(ScriptPosition position, Animation* animation) {	
	resetAnimation(animation);
	int v;
	position = getNextScriptInteger(position, &v);
	animation->mFrameAmount = v;
	position = getNextScriptDouble(position, &animation->mDuration);
	return position;
}

static ScriptPosition loadStageAnimation(ScriptPosition position) {
	int frameAmount;
	position = getNextScriptInteger(position, &frameAmount);

	int dummy;
	position = getNextScriptInteger(position, &dummy);

	char word[100];
	int i;
	for (i = 0; i < frameAmount; i++) {
		position = getNextScriptString(position, word);
	}

	return position;
}

static ScriptPosition loadStageBackground1(ScriptPosition position) {
	char word[100];
	position = getNextScriptString(position, word);

	int dummy;
	position = getNextScriptInteger(position, &dummy);
	position = getNextScriptInteger(position, &dummy);


	return position;
}

static ScriptPosition loader(void* caller, ScriptPosition position) {
	char word[100];	
	position = getNextScriptString(position, word);

	if(!strcmp(word, "BACKGROUND1")) {
		int id = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 100+gData.mBackgroundAmount), &gData.mSprites, makePosition(640 * gData.mBackgroundAmount, 0, 1));
		setMugenAnimationBasePosition(id, &gData.mInvertedScreenPosition);
		gData.mBackgroundAmount++;
		position = loadStageBackground1(position);
	} else if(!strcmp(word, "BACKGROUND_ANIMATION1")) {
		int id = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 100+gData.mBackgroundAmount), &gData.mSprites, makePosition(640 * gData.mBackgroundAmount, 0, 1));
		setMugenAnimationBasePosition(id, &gData.mInvertedScreenPosition);
		gData.mBackgroundAmount++;
	} else if(!strcmp(word, "BACKGROUND2")) {
		int id = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 100+gData.mBackgroundAmount), &gData.mSprites, makePosition(640 * gData.mBackgroundAmount, 0, 1));
		setMugenAnimationBasePosition(id, &gData.mInvertedScreenPosition);
		gData.mBackgroundAmount++;
	}  else if(!strcmp(word, "BACKGROUND_ANIMATION2")) {
		int id = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 100+gData.mBackgroundAmount), &gData.mSprites, makePosition(640 * gData.mBackgroundAmount, 0, 1));
		setMugenAnimationBasePosition(id, &gData.mInvertedScreenPosition);
		gData.mBackgroundAmount++;
	} else if(!strcmp(word, "ANIMATION")) {
		position = loadStageAnimation(position);
	} else {
		logError("Unrecognized token.");
		logErrorString(word);
		abortSystem();
	}

	return position;
}

void loadJusticeStage() {

	gData.isWaitingForDefeat = 0;
	gData.isBackgroundScrolling = 1;
	gData.isScrollingRightForced = 0;
	gData.isWaitingForAnimation = 0;
	gData.isWaitingForDuration = 0;

	gData.isOver = 0;
	gData.mScreenPosition = makePosition(0, 0, 0);
	gData.mInvertedScreenPosition = gData.mScreenPosition;
	gData.mBackgroundAmount = 0;

	gData.mSprites = loadMugenSpriteFileWithoutPalette("LEVEL.sff");
	gData.mAnimations = loadMugenAnimationFile("LEVEL.air");

	gData.script = loadScript("scripts/stage.txt");
	ScriptRegion r = getScriptRegion(gData.script, "LOAD");
	executeOnScriptRegion(r, loader, NULL);

	gData.scriptPosition = getScriptRegionStart(getScriptRegion(gData.script, "MAIN"));

	Position* p = &gData.mScreenPosition;
	setJusticePlayerScreenPositionReference(p);
	setJusticeEnemiesScreenPositionReference(p);
	setJusticeUserInterfaceScreenPositionReference(p);
	addJusticePlayerShadow();
	setCollisionHandlerDebuggingScreenPositionReference(p);

}

static void blockingAnimationFinished(void* caller) {
	gData.isWaitingForAnimation = 0;
}

static void blockingDurationFinished(void* caller) {
	gData.isWaitingForDuration = 0;
}

static ScriptPosition showAnimation(ScriptPosition pos, int isBlocking) {
	int id;
	Position p;
	gData.scriptPosition = getNextScriptInteger(gData.scriptPosition, &id);
	gData.scriptPosition = getNextScriptDouble(gData.scriptPosition, &p.x);
	gData.scriptPosition = getNextScriptDouble(gData.scriptPosition, &p.y);
	gData.scriptPosition = getNextScriptDouble(gData.scriptPosition, &p.z);

	gData.mBlockingAnimationID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, id), &gData.mSprites, p);
	setMugenAnimationNoLoop(gData.mBlockingAnimationID);
	setMugenAnimationCallback(gData.mBlockingAnimationID, blockingAnimationFinished, NULL);

	gData.isWaitingForAnimation = isBlocking;
	return pos;

}

static void updateScript() {

	int isActive = 1;
	while(isActive) {

		if(gData.isWaitingForDefeat) {
			int activeEnemyAmount = getActiveJusticeEnemyAmount();
			if(activeEnemyAmount) return;

			gData.isWaitingForDefeat = 0;
			gData.isBackgroundScrolling = 1;
		}

		if(gData.isWaitingForAnimation || gData.isWaitingForDuration) return;

		if(!hasNextScriptWord(gData.scriptPosition)) {
			gData.isOver = 1;
			setJusticeLevelCleared(getJusticePlayerHealth());
			return;
		}

		int screenPositionX;
		ScriptPosition pos = getNextScriptInteger(gData.scriptPosition, &screenPositionX);
		
		if(gData.mScreenPosition.x < screenPositionX) return;
		gData.scriptPosition = pos;

		char word[100];
		gData.scriptPosition = getNextScriptString(gData.scriptPosition, word);

		if(!strcmp("WAIT", word)) {
			gData.isWaitingForDefeat = 1;
			gData.isBackgroundScrolling = 0;
		} else if(!strcmp("ENEMY", word)) {
			Position enemyPosition;
			int enemyType;
			gData.scriptPosition = getNextScriptDouble(gData.scriptPosition, &enemyPosition.x);
			gData.scriptPosition = getNextScriptDouble(gData.scriptPosition, &enemyPosition.y);
			gData.scriptPosition = getNextScriptDouble(gData.scriptPosition, &enemyPosition.z);
			gData.scriptPosition = getNextScriptInteger(gData.scriptPosition, &enemyType);
			enemyPosition = vecAdd(enemyPosition, gData.mScreenPosition);
			spawnJusticeEnemy(enemyType, enemyPosition);	
		} else if(!strcmp("LEVEL", word)) {
			char name[100];
			gData.scriptPosition = getNextScriptString(gData.scriptPosition, name);
			setCurrentJusticeLevelName(name);
		}  else if(!strcmp("FREEZE_PLAYER", word)) {
			freezeJusticePlayer();
		} else if(!strcmp("UNFREEZE_PLAYER", word)) {
			unfreezeJusticePlayer();
		} else if(!strcmp("FREEZE_ENEMY", word)) {
			freezeJusticeEnemies();
		} else if(!strcmp("UNFREEZE_ENEMY", word)) {
			unfreezeJusticeEnemies();
		} else if(!strcmp("SCROLL_SCREEN_RIGHT", word)) {
			gData.isScrollingRightForced = 1;
		} else if(!strcmp("STOP_SCROLL", word)) {
			gData.isScrollingRightForced = 0;
		}  else if(!strcmp("BLOCK_SCROLL", word)) {
			gData.isBackgroundScrolling = 0;
		} else if(!strcmp("ANIMATION", word)) {
			pos = showAnimation(pos, 0);
		}  else if(!strcmp("ANIMATION_BLOCKING", word)) {
			pos = showAnimation(pos, 1);
		} else if(!strcmp("WAIT_DURATION", word)) {
			double duration;
			gData.scriptPosition = getNextScriptDouble(gData.scriptPosition, &duration);
			addTimerCB(duration, blockingDurationFinished, NULL);
			gData.isWaitingForDuration = 1;
		}  else if(!strcmp("CONGRATS", word)) {
			setJusticeGameCleared();
		} else {
			logError("Unknown token");
			logErrorString(word);
			abortSystem();
		}

		
	

		gData.scriptPosition = getNextScriptInstruction(gData.scriptPosition);
	}
}

static Position getRealJusticeScreenPosition(Position p) {
	return vecSub(p, gData.mScreenPosition);
}

static void scrollJusticeBackgroundRight(double tFactor) {
	if (isJusticeGamePaused()) return;

	gData.mScreenPosition.x += tFactor*10;
	gData.mInvertedScreenPosition.x = -gData.mScreenPosition.x;
}

static void updateStageMovement() {
	if(!gData.isScrollingRightForced && !gData.isBackgroundScrolling) return;

	Position p = getRealJusticeScreenPosition(getJusticePlayerPosition());
	if(gData.isScrollingRightForced || p.x >= 175) {
		scrollJusticeBackgroundRight(0.25);
	}
}

static void updateAnimationSkipping() {
	if (!gData.isWaitingForAnimation) return;

	if (hasPressedStartFlank()) {
		removeMugenAnimation(gData.mBlockingAnimationID);
		gData.isWaitingForAnimation = 0;
	}
}

void updateJusticeStage() {
		updateScript();
		updateStageMovement();
		updateAnimationSkipping();
}

int isJusticePlayingBlockingAnimation()
{
	return gData.isWaitingForAnimation;
}


