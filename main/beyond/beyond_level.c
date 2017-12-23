#include "beyond_level.h"

#include <tari/mugendefreader.h>
#include <tari/animation.h>
#include <tari/collisionhandler.h>
#include <tari/mugenanimationreader.h>
#include <tari/mugenassignmentevaluator.h>
#include <tari/log.h>
#include <tari/system.h>
#include <tari/wrapper.h>
#include <tari/screeneffect.h>
#include <tari/mugenanimationhandler.h>
#include <tari/math.h>

#include "beyond_enemyhandler.h"
#include "beyond_boss.h"
#include "beyond_bg.h"
#include "beyond_player.h"
#include "beyond_gamescreen.h"
#include "beyond_ui.h"
#include "beyond_main.h"
#include "beyond_effecthandler.h"

typedef struct {
	TextureData mTextures[10];
	Animation mAnimation;

	Collider mCollider;

} ShotType;

typedef enum {
	LEVEL_ACTION_TYPE_ENEMY,
	LEVEL_ACTION_TYPE_BOSS,
	LEVEL_ACTION_TYPE_BREAK,
	LEVEL_ACTION_TYPE_RESET_LOCAL_COUNTERS,

} LevelActionType;

typedef struct {
	int mHasBeenActivated;
	int mStagePart;
	Duration mTime;

	LevelActionType mType;
	void* mData;
} LevelAction;

static struct {
	int mCurrentLevel;

	MugenAnimations mAnimations;
	MugenSpriteFile mSprites;

	List mStageActions;

	Duration mTime;

	int mStagePart;
} gData;

static void loadSpritesAndAnimations(MugenDefScript* tScript) {
	char* animationPath = getAllocatedMugenDefStringVariable(tScript, "Header", "animations");
	gData.mAnimations = loadMugenAnimationFile(animationPath);
	freeMemory(animationPath);

	char* spritePath = getAllocatedMugenDefStringVariable(tScript, "Header", "sprites");
	gData.mSprites = loadMugenSpriteFileWithoutPalette(spritePath);
	freeMemory(spritePath);
}

static int isStageEnemy(char* tName) {
	return !strcmp("Enemy", tName);
}

static void loadStageEnemyMovementType(StageEnemy* e, MugenDefScriptGroup* tGroup) {
	char* type = getAllocatedMugenDefStringVariableAsGroup(tGroup, "movementtype");
	
	if (!strcmp("wait", type)) {
		e->mMovementType = ENEMY_MOVEMENT_TYPE_WAIT;
	} else if (!strcmp("rush", type)) {
		e->mMovementType = ENEMY_MOVEMENT_TYPE_RUSH;
	}
	else {
		logError("Unrecognized movement type");
		logErrorString(type);
		abortSystem();
	}

	freeMemory(type);
}

static void loadStageEnemy(MugenDefScriptGroup* tGroup, LevelAction* tLevelAction) {
	StageEnemy* e = allocMemory(sizeof(StageEnemy));
	e->mType = getMugenDefNumberVariableAsGroup(tGroup, "id");
	
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("position", tGroup, &e->mStartPosition, "");
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("waitposition", tGroup, &e->mWaitPosition, "");
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("waitduration", tGroup, &e->mWaitDuration, "");
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("finalposition", tGroup, &e->mFinalPosition, "");
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("speed", tGroup, &e->mSpeed, "");
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("shotfrequency", tGroup, &e->mShotFrequency, "");
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("shottype", tGroup, &e->mShotType, "");
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("health", tGroup, &e->mHealth, "");
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("smallpower", tGroup, &e->mSmallPowerAmount, "");
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("lifedrop", tGroup, &e->mLifeDropAmount, "");
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("bombdrop", tGroup, &e->mBombDropAmount, "");
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("amount", tGroup, &e->mAmount, "");

	loadStageEnemyMovementType(e, tGroup);
	
	tLevelAction->mData = e;
}

static int isStageBoss(char* tName) {
	return !strcmp("Boss", tName);
}

static void loadStageBoss(MugenDefScriptGroup* tGroup, LevelAction* tLevelAction) {
	tLevelAction->mData = NULL;
}

static int isStageBreak(char* tName) {
	return !strcmp("Break", tName);
}

static void loadStageBreak(MugenDefScriptGroup* tGroup, LevelAction* tLevelAction) {
	tLevelAction->mData = NULL;
	gData.mStagePart++;
}

static int isLocalCountReset(char* tName) {
	return !strcmp("ResetLocalCounts", tName);
}

static void loadLocalCountReset(MugenDefScriptGroup* tGroup, LevelAction* tLevelAction) {
	tLevelAction->mData = NULL;
}

static LevelAction* loadLevelActionFromGroup(MugenDefScriptGroup* tGroup) {
	LevelAction* e = allocMemory(sizeof(LevelAction));
	e->mTime = getMugenDefNumberVariableAsGroup(tGroup, "time");
	e->mHasBeenActivated = 0;
	e->mStagePart = gData.mStagePart;
	return e;
}

static void loadSingleLevelAction(MugenDefScriptGroup* tGroup, LevelActionType tType, void(*tLoadFunc)(MugenDefScriptGroup*, LevelAction*)) {
	LevelAction* e = loadLevelActionFromGroup(tGroup);
	e->mType = tType;
	tLoadFunc(tGroup, e);
	list_push_back_owned(&gData.mStageActions, e);
}

static void loadStageEnemiesFromScript(MugenDefScript* tScript) {
	gData.mStagePart = 0;

	MugenDefScriptGroup* current = tScript->mFirstGroup;
	while (current != NULL) {
		
		if (isStageEnemy(current->mName)) {
			loadSingleLevelAction(current, LEVEL_ACTION_TYPE_ENEMY, loadStageEnemy);
		}
		else if (isStageBreak(current->mName)) {
			loadSingleLevelAction(current, LEVEL_ACTION_TYPE_BREAK, loadStageBreak);
		}
		else if (isStageBoss(current->mName)) {
			loadSingleLevelAction(current, LEVEL_ACTION_TYPE_BOSS, loadStageBoss);
		}
		else if (isLocalCountReset(current->mName)) {
			loadSingleLevelAction(current, LEVEL_ACTION_TYPE_RESET_LOCAL_COUNTERS, loadLocalCountReset);
		}

		
		current = current->mNext;
	}

}

static void loadBoss(MugenDefScript* tScript) {
	char* defPath = getAllocatedMugenDefStringVariable(tScript, "Header", "boss");
	loadBeyondBossFromDefinitionPath(defPath, &gData.mAnimations, &gData.mSprites);
	freeMemory(defPath);
}

static void loadStage(MugenDefScript* tScript) {
	char* defPath = getAllocatedMugenDefStringVariable(tScript, "Header", "bg");
	setBeyondBackground(defPath, &gData.mSprites, &gData.mAnimations);
	freeMemory(defPath);
}

static void loadLevelHandler(void* tData) {
	(void)tData;
	gData.mStageActions = new_list();

	char path[1024];

// TODO: more elegant
#ifdef DREAMCAST
    sprintf(path, "assets/main/%s/stage/dc/%d.def", getBeyondDirectory(), gData.mCurrentLevel);
#else
	sprintf(path, "assets/main/%s/stage/%d.def", getBeyondDirectory(), gData.mCurrentLevel);
#endif
	MugenDefScript script = loadMugenDefScript(path);

	loadSpritesAndAnimations(&script);
	loadBeyondEnemyTypesFromScript(&script, &gData.mAnimations, &gData.mSprites);
	loadStageEnemiesFromScript(&script);
	loadBoss(&script);
	loadStage(&script);
	unloadMugenDefScript(script);

	loadBeyondStageDisplay(&gData.mSprites);

	gData.mStagePart = 0;
	gData.mTime = 0;
}

static void updateTime() {
	gData.mTime++;
}

static void updateSingleStageEnemy(LevelAction* tLevelAction) {
	StageEnemy* e = tLevelAction->mData;
	tLevelAction->mHasBeenActivated = 1;	
	addBeyondEnemy(e);
}


static void updateBoss(LevelAction* tLevelAction) {

	tLevelAction->mHasBeenActivated = 1;
	activateBeyondBoss();

}

static void increaseStagePart() {
	gData.mStagePart++;
	gData.mTime = 0;
}

static void updateSingleBreak(LevelAction* tLevelAction) {
	if (getBeyondEnemyAmount()) return;

	tLevelAction->mHasBeenActivated = 1;
	increaseStagePart();
}

static void updateLocalCounterReset(LevelAction* tLevelAction) {
	tLevelAction->mHasBeenActivated = 1;
	resetBeyondLocalPlayerCounts();
}

static void updateSingleAction(void* tCaller, void* tData) {
	(void)tCaller;
	LevelAction* e = tData;

	if (e->mHasBeenActivated) return;
	if (e->mStagePart != gData.mStagePart) return;
	if (!isDurationOver(gData.mTime, e->mTime)) return;
	
	if (e->mType == LEVEL_ACTION_TYPE_ENEMY) {
		updateSingleStageEnemy(e);
	} else if (e->mType == LEVEL_ACTION_TYPE_BOSS) {
		updateBoss(e);
	}
	else if (e->mType == LEVEL_ACTION_TYPE_BREAK) {
		updateSingleBreak(e);
	}
	else if (e->mType == LEVEL_ACTION_TYPE_RESET_LOCAL_COUNTERS) {
		updateLocalCounterReset(e);
	}
	else {
		logError("Unrecognized action type.");
		logErrorInteger(e->mType);
		abortSystem();
	}
}

static void updateActions() {
	list_map(&gData.mStageActions, updateSingleAction, NULL);
}

static void updateFinalFrontierLevelInit() {
	double scale = getBeyondFinalFrontierLevelBackgroundScale();

	if (scale >= 2) {
		addMugenAnimation(getMugenAnimation(&gData.mAnimations, 9001), &gData.mSprites, makePosition(320, 480, 47));

		burnBeyondPlayer();
		gData.mStagePart++;
	}
}

static void updateFinalFrontierLevelBasics() {
	if (((int)gData.mTime) % 30 == 0) {
		addBeyondExplosionEffect(vecAdd(getBeyondBossPosition(), makePosition(randfrom(-50, 50), randfrom(-50, 50), 1)));
	}

	damageBeyondBoss();
}

static void updateFinalFrontierLevel() {
	if (!isUltimateFrontier() || gData.mCurrentLevel != 3) return;
	if (gData.mStagePart == 1) {
		updateFinalFrontierLevelInit();
	}
	else if (gData.mStagePart == 2) {
		updateFinalFrontierLevelBasics();
	}

	
}

static void updateLevelHandler(void* tData) {
	if (isWrapperPaused()) return;

	(void)tData;
	updateTime();
	updateActions();
	updateFinalFrontierLevel();
}

ActorBlueprint BeyondLevelHandler = {
	.mLoad = loadLevelHandler,
	.mUpdate = updateLevelHandler,
};

void setBeyondLevelToStart()
{
	gData.mCurrentLevel = 3;
}

void goToNextBeyondLevel()
{
	if (gData.mCurrentLevel == 3 && isUltimateFrontier()) {
		addFadeOut(30, goToBeyondStoryScreen, NULL);
		return;
	}

	gData.mCurrentLevel++;

	setNewScreen(&BeyondGameScreen);
}

void fetchBeyondStagePartTime(char * tDst, void * tCaller)
{
	(void)tCaller;
	sprintf(tDst, "%d", (int)gData.mTime);
}

void advanceBeyondStagePart()
{
	gData.mStagePart++;
}

int getBeyondCurrentLevel()
{
	return gData.mCurrentLevel;
}

int getBeyondStagePart()
{
	return gData.mStagePart;
}
