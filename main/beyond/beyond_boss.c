#include "beyond_boss.h"

#include <assert.h>

#include <prism/mugenanimationhandler.h>
#include <prism/mugendefreader.h>
#include <prism/mugenscriptparser.h>
#include <prism/physicshandler.h>
#include <prism/log.h>
#include <prism/system.h>
#include <prism/math.h>
#include <prism/mugenassignmentevaluator.h>
#include <prism/animation.h>
#include <prism/texthandler.h>
#include <prism/wrapper.h>
#include <prism/screeneffect.h>
#include <prism/input.h>
#include <prism/sound.h>

#include "beyond_collision.h"
#include "beyond_shothandler.h"
#include "beyond_itemhandler.h"
#include "beyond_level.h"
#include "beyond_player.h"
#include "beyond_storyscreen.h"
#include "beyond_main.h"

typedef enum {
	BOSS_ACTION_TYPE_GOTO,
	BOSS_ACTION_TYPE_SHOT,
	BOSS_ACTION_TYPE_DROP_SMALL_POWER,
	BOSS_ACTION_TYPE_ADD_ROTATION,
	BOSS_ACTION_TYPE_SET_ROTATION,
	BOSS_ACTION_TYPE_ADVANCE_STAGE_PART,
	BOSS_ACTION_TYPE_FADE_TO_WHITE,
	BOSS_ACTION_TYPE_CHANGE_ANIMATION,
	BOSS_ACTION_TYPE_DISABLE_PLAYER_HIT,
	BOSS_ACTION_TYPE_SET_AID_TEXT,
	BOSS_ACTION_TYPE_SET_FINAL_BOSS,
	BOSS_ACTION_TYPE_DROP_LIFE,
	BOSS_ACTION_TYPE_DROP_BOMB,
} BossActionType;


typedef struct {
	MugenAssignment* mTarget;
	MugenAssignment* mSpeed;
	
} GotoAction;

typedef struct {
	int mShotID;
} ShotAction;

typedef struct {
	int mIsTimeBased;
	MugenAssignment* mTime;
	int mIsRepeating;
	MugenAssignment* mRepeatTime;

	int mIsHealthBased;
	MugenAssignment* mHealth;

	BossActionType mType;
	void* mData;
} BossAction;

typedef struct {
	int mLifeStart;

	Vector mActions;
} BossPattern;

typedef struct {
	MugenAssignment* mValue;
} SingleValueAction;

typedef enum {
	AID_TEXT_DOWN,
	AID_TEXT_UP,
	AID_TEXT_MID,
} AidTextDirection;

static struct {
	int mIsActive;
	int mIsLoaded;
	int mIdleAnimationNumber;
	MugenAnimation* mIdleAnimation;
	MugenSpriteFile* mSprites;
	MugenAnimations* mAnimations;

	char* mName;
	int mLifeMax;
	int mLife;
	Position mStartPosition;
	
	int mAnimationID;
	int mPhysicsID;
	BeyondCollisionData mCollisionData;

	Vector mPatterns;

	int mCurrentPattern;
	Duration mTime;
	Vector3D mTarget;

	Position mHealthBarPosition;
	int mHealthBarTextID;
	TextureData mHealthBarTexture;
	int mHealthBarAnimationID;

	double mSpeed;
	double mRotation;

	int mIsDefeated;

	int mIsFinalBoss;
	int mIsInvincible;

	AidTextDirection mAidTextDirection;
} gData;

static void loadBossHandler(void* tData) {
	(void)tData;
	gData.mIsActive = 0;
	gData.mIsLoaded = 0;

	gData.mHealthBarTexture = loadTexture("$/rd/effects/white.pkg");

	gData.mIsFinalBoss = 0;
}

static int isHeader(MugenDefScriptGroup* tGroup) {
	return !strcmp("Header", tGroup->mName);
}

static void loadHeader(MugenDefScriptGroup* tGroup) {
	gData.mName = getAllocatedMugenDefStringVariableAsGroup(tGroup, "name");
	gData.mLifeMax = getMugenDefNumberVariableAsGroup(tGroup, "life");
	gData.mStartPosition = getMugenDefVectorOrDefaultAsGroup(tGroup, "startpos", makePosition(0, 0, 0));
	gData.mIdleAnimationNumber = getMugenDefNumberVariableAsGroup(tGroup, "anim");
}

static int isNewPattern(MugenDefScriptGroup* tGroup) {
	return !strcmp("Pattern", tGroup->mName);
}

static void loadNewPattern(MugenDefScriptGroup* tGroup) {
	BossPattern* e = allocMemory(sizeof(BossPattern));
	e->mLifeStart = getMugenDefIntegerOrDefaultAsGroup(tGroup, "lifestart", gData.mLifeMax);
	e->mActions = new_vector();

	vector_push_back_owned(&gData.mPatterns, e);
}

static int isAction(MugenDefScriptGroup* tGroup) {
	return !strcmp("Action", tGroup->mName);
}

static void loadGotoAction(BossAction* tAction, MugenDefScriptGroup* tGroup) {
	GotoAction* e = allocMemory(sizeof(GotoAction));
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("value", tGroup, &e->mTarget, "");
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("speed", tGroup, &e->mSpeed, "");
	tAction->mData = e;
}


static void loadShotAction(BossAction* tAction, MugenDefScriptGroup* tGroup) {
	ShotAction* e = allocMemory(sizeof(ShotAction));
	e->mShotID = getMugenDefIntegerOrDefaultAsGroup(tGroup, "shottype", 0);
	tAction->mData = e;
}

static void loadSingleValueAction(BossAction* tAction, MugenDefScriptGroup* tGroup) {
	SingleValueAction* e = allocMemory(sizeof(SingleValueAction));
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("value", tGroup, &e->mValue, "");
	tAction->mData = e;
}

static void loadActionType(BossAction* e, MugenDefScriptGroup* tGroup) {
	char* typeString = getAllocatedMugenDefStringVariableAsGroup(tGroup, "type");
	if (!strcmp("goto", typeString)) {
		e->mType = BOSS_ACTION_TYPE_GOTO;
		loadGotoAction(e, tGroup);
	}
	else if (!strcmp("shot", typeString)) {
		e->mType = BOSS_ACTION_TYPE_SHOT;
		loadShotAction(e, tGroup);
	}
	else if (!strcmp("smallpowerdrop", typeString)) {
		e->mType = BOSS_ACTION_TYPE_DROP_SMALL_POWER;
		loadSingleValueAction(e, tGroup);
	}
	else if (!strcmp("lifedrop", typeString)) {
		e->mType = BOSS_ACTION_TYPE_DROP_LIFE;
		loadSingleValueAction(e, tGroup);
	}
	else if (!strcmp("bombdrop", typeString)) {
		e->mType = BOSS_ACTION_TYPE_DROP_BOMB;
		loadSingleValueAction(e, tGroup);
	}
	else if (!strcmp("setrotation", typeString)) {
		e->mType = BOSS_ACTION_TYPE_SET_ROTATION;
		loadSingleValueAction(e, tGroup);
	}
	else if (!strcmp("addrotation", typeString)) {
		e->mType = BOSS_ACTION_TYPE_ADD_ROTATION;
		loadSingleValueAction(e, tGroup);
	}
	else if (!strcmp("stagepartadvance", typeString)) {
		e->mType = BOSS_ACTION_TYPE_ADVANCE_STAGE_PART;
		loadSingleValueAction(e, tGroup);
	}
	else if (!strcmp("fadewhite", typeString)) {
		e->mType = BOSS_ACTION_TYPE_FADE_TO_WHITE;
		loadSingleValueAction(e, tGroup);
	}
	else if (!strcmp("changeanim", typeString)) {
		e->mType = BOSS_ACTION_TYPE_CHANGE_ANIMATION;
		loadSingleValueAction(e, tGroup);
	}
	else if (!strcmp("disableplayerhit", typeString)) {
		e->mType = BOSS_ACTION_TYPE_DISABLE_PLAYER_HIT;
		loadSingleValueAction(e, tGroup);
	}
	else if (!strcmp("setaidtextrandom", typeString)) {
		e->mType = BOSS_ACTION_TYPE_SET_AID_TEXT;
		loadSingleValueAction(e, tGroup);
	}
	else if (!strcmp("setfinalboss", typeString)) {
		e->mType = BOSS_ACTION_TYPE_SET_FINAL_BOSS;
		loadSingleValueAction(e, tGroup);
	}
	else {
		logError("Unrecognized action type");
		logErrorString(typeString);
		abortSystem();
	}

	freeMemory(typeString);
}

static void loadAction(MugenDefScriptGroup* tGroup) {
	BossAction* e = allocMemory(sizeof(BossAction));
	e->mIsTimeBased = fetchMugenAssignmentFromGroupAndReturnWhetherItExists("time", tGroup, &e->mTime);
	e->mIsHealthBased = fetchMugenAssignmentFromGroupAndReturnWhetherItExists("health", tGroup, &e->mHealth);
	assert(e->mIsTimeBased ^ e->mIsHealthBased);
	e->mIsRepeating= fetchMugenAssignmentFromGroupAndReturnWhetherItExists("timerepeated", tGroup, &e->mRepeatTime);

	loadActionType(e, tGroup);

	assert(vector_size(&gData.mPatterns));
	BossPattern* pattern = vector_get_back(&gData.mPatterns);

	vector_push_back_owned(&pattern->mActions, e);
}

void loadBeyondBossFromDefinitionPath(char * tDefinitionPath, MugenAnimations* tAnimations, MugenSpriteFile* tSprites)
{
	gData.mPatterns = new_vector();

	MugenDefScript script = loadMugenDefScript(tDefinitionPath);
	resetMugenScriptParser();
	addMugenScriptParseFunction(isHeader, loadHeader);
	addMugenScriptParseFunction(isNewPattern, loadNewPattern);
	addMugenScriptParseFunction(isAction, loadAction);
	parseMugenScript(&script);

	unloadMugenDefScript(script);

	gData.mAnimations = tAnimations;
	gData.mIdleAnimation = getMugenAnimation(tAnimations, gData.mIdleAnimationNumber);
	gData.mSprites = tSprites;
	gData.mCollisionData.mCollisionList = getBeyondEnemyCollisionList();
	gData.mCollisionData.mIsItem = 0;

	gData.mIsLoaded = 1;
}

static void updateHealthBarSize() {
	double start = gData.mHealthBarPosition.x;
	double end = 620;

	double totalLength = end - start;
	double t = gData.mLife / (double)gData.mLifeMax;
	double length = totalLength*t;
	setAnimationSize(gData.mHealthBarAnimationID, makePosition(length, 10, 1), makePosition(0, 0, 0));
}

void goToBeyondStoryScreen(void* tCaller) {
	(void)tCaller;
	char path[1024];
	if (getBeyondContinueAmount() == 5) {
		sprintf(path, "assets/main/%s/story/GOOD.def", getBeyondDirectory());
	}
	else {
		sprintf(path, "assets/main/%s/story/BAD.def", getBeyondDirectory());
	}
	setBeyondCurrentStoryDefinitionFile(path);

	setNewScreen(&BeyondStoryScreen);
}

static void setBossDefeated() {
	if (gData.mIsDefeated) return;
	gData.mIsDefeated = 1;
	if (!gData.mIsFinalBoss) {
		goToNextBeyondLevel();
	}
	else {
		addFadeOut(30, goToBeyondStoryScreen, NULL);
	}
}

static void bossHitCB(void* tCaller, void* tCollisionData) {
	(void)tCaller;
	(void)tCollisionData;

	if (gData.mIsDefeated) return;
	if (gData.mIsInvincible) return;

	gData.mLife--;
	updateHealthBarSize();

	if (gData.mLife <= 0) {
		setBossDefeated();
	}
}

void activateBeyondBoss() {
	gData.mPhysicsID = addToPhysicsHandler(gData.mStartPosition);

	gData.mAnimationID = addMugenAnimation(gData.mIdleAnimation, gData.mSprites, makePosition(0, 0, 15));
	setMugenAnimationBasePosition(gData.mAnimationID, getHandledPhysicsPositionReference(gData.mPhysicsID));
	setMugenAnimationCollisionActive(gData.mAnimationID, getBeyondEnemyCollisionList(), bossHitCB, NULL, &gData.mCollisionData);

	gData.mTime = 0;
	gData.mCurrentPattern = 0;
	gData.mLife = gData.mLifeMax;
	gData.mTarget = *getHandledPhysicsPositionReference(gData.mPhysicsID);
	gData.mSpeed = 0;
	gData.mRotation = 0;

	char bossText[100];
	sprintf(bossText, "%s", gData.mName);
	gData.mHealthBarTextID = addHandledText(makePosition(21, 21, 80), bossText, 0, COLOR_BLACK, makePosition(15, 15, 0), makePosition(-2, -2, 1), makePosition(INF, INF, INF), INF);
	gData.mHealthBarTextID = addHandledText(makePosition(20, 20, 81), bossText, 0, COLOR_WHITE, makePosition(15, 15, 0), makePosition(-2, -2, 1), makePosition(INF, INF, INF), INF);
	gData.mHealthBarPosition = makePosition(strlen(bossText)*(15-2)+32, 22, 80);
	gData.mHealthBarAnimationID = playOneFrameAnimationLoop(gData.mHealthBarPosition, &gData.mHealthBarTexture);
	setAnimationColorType(gData.mHealthBarAnimationID, COLOR_DARK_RED);
	updateHealthBarSize();

	gData.mIsFinalBoss = 0;
	gData.mIsInvincible = 0;

	gData.mIsDefeated = 0;
	gData.mIsActive = 1;

	if (isUltimateFrontier() && getBeyondCurrentLevel() == 3) {
		playTrack(18);
	}

}

void damageBeyondBoss()
{
	bossHitCB(NULL, NULL);
}

void fetchBeyondBossTimeVariable(char * tDst, void * tCaller)
{
	(void)tCaller;
	sprintf(tDst, "%f", gData.mTime);
}

int isBeyondBossActive()
{
	return gData.mIsActive;
}

Position getBeyondBossPosition()
{
	assert(gData.mIsActive);
	Position p = *getHandledPhysicsPositionReference(gData.mPhysicsID);
	return p;
}

void evaluateBeyondTextAidFunction(char * tDst, void * tCaller)
{
	(void)tCaller;
	sprintf(tDst, "%d", gData.mAidTextDirection);
}

void addFinalBeyondBossShot(int mID)
{
	if (!gData.mIsFinalBoss) return;

	Position p = *getHandledPhysicsPositionReference(gData.mPhysicsID);
	addBeyondShot(mID, getBeyondEnemyCollisionList(), p);
}

void setFinalBeyondBossInvincible()
{
	if (!gData.mIsFinalBoss) return;
	gData.mIsInvincible = 1;
}

void setFinalBeyondBossVulnerable()
{
	if (!gData.mIsFinalBoss) return;
	gData.mIsInvincible = 0;
}


static void updateGoingToNextPattern() {
	if (gData.mCurrentPattern >= vector_size(&gData.mPatterns) - 1) return;

	BossPattern* nextPattern = vector_get(&gData.mPatterns, gData.mCurrentPattern + 1);
	if (gData.mLife <= nextPattern->mLifeStart) {
		gData.mLife = nextPattern->mLifeStart;
		gData.mTime = 0;
		gData.mCurrentPattern++;
	}
}

static void setBossTarget(Vector3D tTarget) {
	gData.mTarget = tTarget;
}

static void performGoto(BossAction* tAction) {
	GotoAction* e = tAction->mData;
	gData.mSpeed = getMugenAssignmentAsFloatValueOrDefaultWhenEmpty(e->mSpeed, NULL, 2);
	Vector3D target = getMugenAssignmentAsVector3DValueOrDefaultWhenEmpty(e->mTarget, NULL, makePosition(0, 0, 0));

	setHandledPhysicsMaxVelocity(gData.mPhysicsID, gData.mSpeed);
	setBossTarget(target);
}

static void performShot(BossAction* tAction) {
	ShotAction* e = tAction->mData;
	Position p = *getHandledPhysicsPositionReference(gData.mPhysicsID);
	addBeyondShot(e->mShotID, getBeyondEnemyShotCollisionList(), p);
}

static void performSmallPowerDrop(BossAction* tAction) {
	SingleValueAction* e = tAction->mData;

	Position p = *getHandledPhysicsPositionReference(gData.mPhysicsID);
	int amount = getMugenAssignmentAsIntegerValueOrDefaultWhenEmpty(e->mValue, NULL, 0);
	addBeyondSmallPowerItems(p, amount);
}

static void performLifeDrop(BossAction* tAction) {
	SingleValueAction* e = tAction->mData;

	Position p = *getHandledPhysicsPositionReference(gData.mPhysicsID);
	int amount = getMugenAssignmentAsIntegerValueOrDefaultWhenEmpty(e->mValue, NULL, 0);
	addBeyondLifeItems(p, amount);
}

static void performBombDrop(BossAction* tAction) {
	SingleValueAction* e = tAction->mData;

	Position p = *getHandledPhysicsPositionReference(gData.mPhysicsID);
	int amount = getMugenAssignmentAsIntegerValueOrDefaultWhenEmpty(e->mValue, NULL, 0);
	addBeyondBombItems(p, amount);
}

static void performSettingRotation(BossAction* tAction) {
	SingleValueAction* e = tAction->mData;

	double value = getMugenAssignmentAsFloatValueOrDefaultWhenEmpty(e->mValue, NULL, 0);
	gData.mRotation = value;
	setMugenAnimationDrawAngle(gData.mAnimationID, gData.mRotation);
}

static void performAddingRotation(BossAction* tAction) {
	SingleValueAction* e = tAction->mData;

	double value = getMugenAssignmentAsFloatValueOrDefaultWhenEmpty(e->mValue, NULL, 0);
	gData.mRotation += value;
	setMugenAnimationDrawAngle(gData.mAnimationID, gData.mRotation);
}

static void performStagePartAdvancement() {
	advanceBeyondStagePart();
}

static void fadeOutOver(void* tCaller) {
	(void)tCaller;
	enableDrawing();
}

static void performAnimationChange(BossAction* tAction) {
	SingleValueAction* e = tAction->mData;

	int value = getMugenAssignmentAsIntegerValueOrDefaultWhenEmpty(e->mValue, NULL, 0);
	changeMugenAnimation(gData.mAnimationID, getMugenAnimation(gData.mAnimations, value));
}

static void performFadeToWhite() {
	addFadeOut(179, fadeOutOver, NULL);
}

static void performPlayerHitDisable() {
	disableBeyondPlayerBossCollision();
}

static void performSettingRandomAidText() {
	gData.mAidTextDirection = randfromInteger(AID_TEXT_DOWN, AID_TEXT_MID);
}

static void performSettingFinalBoss() {
	gData.mIsFinalBoss = 1;


	PhysicsObject* physics = getPhysicsFromHandler(gData.mPhysicsID);
	PhysicsObject* playerPhysics = getBeyondPlayerPhysics();
	*physics = *playerPhysics;
	physics->mPosition.x = 320 + (320 - physics->mPosition.x);
	setHandledPhysicsDragCoefficient(gData.mPhysicsID, makePosition(1, 1, 0));
}

static void performAction(BossAction* e) {
	if (e->mType == BOSS_ACTION_TYPE_GOTO) {
		performGoto(e);
	}
	else if (e->mType == BOSS_ACTION_TYPE_SHOT) {
		performShot(e);
	}
	else if (e->mType == BOSS_ACTION_TYPE_DROP_SMALL_POWER) {
		performSmallPowerDrop(e);
	}
	else if (e->mType == BOSS_ACTION_TYPE_DROP_LIFE) {
		performLifeDrop(e);
	}
	else if (e->mType == BOSS_ACTION_TYPE_DROP_BOMB) {
		performBombDrop(e);
	}
	else if (e->mType == BOSS_ACTION_TYPE_SET_ROTATION) {
		performSettingRotation(e);
	}
	else if (e->mType == BOSS_ACTION_TYPE_ADD_ROTATION) {
		performAddingRotation(e);
	}
	else if (e->mType == BOSS_ACTION_TYPE_ADVANCE_STAGE_PART) {
		performStagePartAdvancement();
	}
	else if (e->mType == BOSS_ACTION_TYPE_FADE_TO_WHITE) {
		performFadeToWhite();
	}
	else if (e->mType == BOSS_ACTION_TYPE_CHANGE_ANIMATION) {
		performAnimationChange(e);
	}
	else if (e->mType == BOSS_ACTION_TYPE_DISABLE_PLAYER_HIT) {
		performPlayerHitDisable();
	}
	else if (e->mType == BOSS_ACTION_TYPE_SET_AID_TEXT) {
		performSettingRandomAidText();
	}
	else if (e->mType == BOSS_ACTION_TYPE_SET_FINAL_BOSS) {
		performSettingFinalBoss();
	}
	else {
		logError("Unrecognized boss action type");
		logErrorInteger(e->mType);
		abortSystem();
	}
	
}

static void updateSingleAction(void* tCaller, void* tData) {
	(void)tCaller;
	BossAction* e = tData;
	
	int isTimeTrigger = e->mIsTimeBased && isDurationOver(gData.mTime, evaluateMugenAssignmentAndReturnAsFloat(e->mTime, NULL));
	if (isTimeTrigger) {
		if (e->mIsRepeating) {
			int repeatTime = getMugenAssignmentAsIntegerValueOrDefaultWhenEmpty(e->mRepeatTime, NULL, INF);
			e->mTime = makeFloatMugenAssignment(gData.mTime + repeatTime);
		}
		else {
			e->mTime = makeFloatMugenAssignment(INF);
		}
	}

	int isHealthTrigger = e->mIsHealthBased && gData.mLife < evaluateMugenAssignmentAndReturnAsInteger(e->mHealth, NULL);
	if (isHealthTrigger) {
		e->mHealth = makeNumberMugenAssignment(-1);
	}
	
	if (isTimeTrigger || isHealthTrigger) {
		performAction(e);
	}
}

static void updateActions() {
	BossPattern* pattern = vector_get(&gData.mPatterns, gData.mCurrentPattern);
	vector_map(&pattern->mActions, updateSingleAction, NULL);
}

static void updateMovement() {
	if (gData.mIsFinalBoss) return;
	Position p = *getHandledPhysicsPositionReference(gData.mPhysicsID);
	Vector3D delta = vecSub(gData.mTarget, p);
	Velocity* vel = getHandledPhysicsVelocityReference(gData.mPhysicsID);
	double speed = min(vecLength(delta), gData.mSpeed);
	*vel = vecScale(vecNormalize(delta), speed);
}

static void updateTime() {
	gData.mTime++;
}

static void updateFinalBoss() {
	if (!gData.mIsFinalBoss) return;
	
	setHandledPhysicsMaxVelocity(gData.mPhysicsID, getBeyondPlayerSpeed());
	Position* pos = getHandledPhysicsPositionReference(gData.mPhysicsID);
	*pos = clampPositionToGeoRectangle(*pos, makeGeoRectangle(0, 0, 640, 327));

	if(hasPressedLeftSingle(0)) {
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(getBeyondPlayerAcceleration(), 0, 0));
	}
	if (hasPressedRightSingle(0)) {
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(-getBeyondPlayerAcceleration(), 0, 0));
	}
	if (hasPressedUpSingle(0)) {
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(0, -getBeyondPlayerAcceleration(), 0));
	}
	if (hasPressedDownSingle(0)) {
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(0, getBeyondPlayerAcceleration(), 0));
	}

}

static void updateBoss(void* tData) {
	(void)tData;
	if (!gData.mIsActive) return;
	if (isWrapperPaused()) return;

	updateGoingToNextPattern();
	updateActions();
	updateMovement();
	updateFinalBoss();
	updateTime();
	
}

ActorBlueprint BeyondBossHandler = {
	.mLoad = loadBossHandler,
	.mUpdate = updateBoss,
};
