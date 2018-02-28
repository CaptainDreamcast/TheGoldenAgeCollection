#include "senpai_enemyhandler.h"

#include <prism/datastructures.h>
#include <prism/animation.h>
#include <prism/math.h>
#include <prism/input.h>
#include <prism/mugenanimationhandler.h>
#include <prism/tweening.h>
#include <prism/timer.h>
#include <prism/texthandler.h>
#include <prism/screeneffect.h>

#include "senpai_levelhandler.h"
#include "senpai_player.h"
#include "senpai_titlescreen.h"

typedef struct {
	Vector3DI mTilePosition;
	Vector3DI mTargetTilePosition;
	Vector3DI mForwardTilePosition;
	Position mPosition;

	int mType;
	Duration mSpeed;

	int mAnimationID;

	int mCallbackID;
} ActiveEnemy;

static struct {
	IntMap mActiveEnemies;

	TextureData mSmallTextures[11];

	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;

	int mHorrorTextAmount;

	int mTextBoxAnimationID;
	double mLargeEnemyAnimationScale;
	int mLargeEnemyAnimationID;
	int mHorrorTextID;
	int mIsShowingHorror;
	int mIsFadingHorrorOut;

} gData;

static char gHorrorTexts[][1024] = {
	"ONII-CHAN, YOU ARE NOW AWARE THAT YOUR EARS CLICK WHENEVER YOU SWALLOW!",
	"ONII-CHAN, STATISTICALLY THERE IS MORE THAN A 80% CHANCE YOUR GIRLFRIEND WILL LEAVE YOU!",
	"ONII-CHAN, DID YOU KNOW THERE ARE NON-LETHAL PARASITES LIVING IN YOUR BRAIN RIGHT NOW?",
	"ONII-CHAN, ARE YOU SURE YOUR STOVE IS TURNED OFF?",
	"ONII-CHAN, YOUR PARENTS ARE OLDER THAN YOU AND LIKELY TO DIE FIRST!",
	"ONII-CHAN, THERE'S NOT ONLY A SKELETON INSIDE OF YOU, BUT ALSO ORGANS!",
	"ONII-CHAN, IT WON'T GET BETTER!",
	"ONII-CHAN, THERE WILL NEVER BE A THIRD SEASON OF SPICE AND WOLF!",
	"ONII-CHAN, ANIME IS NOT REAL!",
	"ONII-CHAN, ISN'T YOUR HEART BEATING A BIT ABNORMALLY?",
	"ONII-CHAN, YOU ARE NOW BREATHING MANUALLY!",
	"ONII-CHAN, WHAT HAPPENED TO THE TOY YOU LOVED SO MUCH AS A CHILD?",
	"ONII-CHAN, WHEN WAS THE LAST TIME YOU CALLED YOUR GRANDMA?",
	"ONII-CHAN, ARE YOU SURE YOUR FRIENDS ARE FRIENDS?",
	"ONII-CHAN, WHAT IF YOUR CONSCIOUSNESS GETS ERASED EVERY TIME YOU FALL ASLEEP?",
	"ONII-CHAN, ONE DAY YOU WILL DIE AND IT IS UNAVOIDABLE!",
	"ONII-CHAN, ISN'T THERE SOMETHING YOU'RE FORGETTING RIGHT NOW?",
	"ONII-CHAN, HOW'S THE DEBT COMING ALONG?",
	"ONII-CHAN, ARE YOU HAPPY WITH YOUR CURRENT JOB?",
	"ONII-CHAN, YOU SHOULD BE WORKING RIGHT NOW!",
};

static void loadEnemyHandler(void* tData) {
	gData.mActiveEnemies = new_int_map();

	loadConsecutiveTextures(gData.mSmallTextures, "assets/main/senpai/enemies/small/ENEMY.pkg", 10);

	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/main/senpai/enemies/ENEMIES.sff");
	gData.mAnimations = loadMugenAnimationFile("assets/main/senpai/enemies/ENEMIES.air");

	gData.mHorrorTextAmount = 20;

	gData.mIsShowingHorror = 0;
}

static void showEnemyHorror(ActiveEnemy* e) {
	
	gData.mLargeEnemyAnimationID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 10+e->mType), &gData.mSprites, makePosition(320, 140, 40));
	gData.mTextBoxAnimationID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 100), &gData.mSprites, makePosition(320, 460, 50));

	int horrorTextIndex = randfromInteger(0, gData.mHorrorTextAmount - 1);
	int textSize = 25;
	gData.mHorrorTextID = addHandledTextWithBuildup(makePosition(41, 318, 60), gHorrorTexts[horrorTextIndex], 0, COLOR_WHITE, makePosition(textSize, textSize, 1), makePosition(-5, 0, 0), makePosition(558, 132, 1), INF, strlen(gHorrorTexts[horrorTextIndex]) * 5);

	gData.mLargeEnemyAnimationScale = 1;
	
	gData.mIsFadingHorrorOut = 0;
	gData.mIsShowingHorror = 1;
}

static void unloadEnemyHorror() {
	removeMugenAnimation(gData.mTextBoxAnimationID);
	removeMugenAnimation(gData.mLargeEnemyAnimationID);
	removeHandledText(gData.mHorrorTextID);
	gData.mIsShowingHorror = 0;
}

static void updateActiveEnemy(void* tCaller, void* tData) {
	(void)tCaller;
	ActiveEnemy* e = tData;

	if (gData.mIsShowingHorror) return;

	Vector3DI playerPosition = getSenpaiPlayerTilePosition();
	if (playerPosition.x == e->mTargetTilePosition.x && playerPosition.y == e->mTargetTilePosition.y) {
		showEnemyHorror(e);
	}

}

static void resetPlayerAfterGameOver(void* tCaller) {
	(void)tCaller;
	unloadEnemyHorror();
	resetSenpaiPlayerAfterGameOver();

	enableDrawing();
}

static void updateLargeEnemyAnimation() {
	if (!gData.mIsShowingHorror) return;

	setMugenAnimationBaseDrawScale(gData.mLargeEnemyAnimationID, gData.mLargeEnemyAnimationScale);
	gData.mLargeEnemyAnimationScale += 0.002;

	if (!gData.mIsFadingHorrorOut && (isHandledTextBuiltUp(gData.mHorrorTextID) || hasPressedAFlank() || hasPressedStartFlank())) {
		addFadeOut(180, resetPlayerAfterGameOver, NULL);
		gData.mIsFadingHorrorOut = 1;
	}
}

static void updateEnemyHandler(void* tData) {
	int_map_map(&gData.mActiveEnemies, updateActiveEnemy, NULL);

	updateLargeEnemyAnimation();
}

ActorBlueprint SenpaiEnemyHandler = {
	.mLoad = loadEnemyHandler,
	.mUpdate = updateEnemyHandler
};

static Vector3DI getNewEnemyTilePosition() {
	Vector3DI tilePosition;
	int i;
	for (i = 0; i < 100; i++) {
		tilePosition = makeVector3DI(randfromInteger(1, getSenpaiLevelSizeX() - 2), randfromInteger(1, getSenpaiLevelSizeY() - 2), 0);
		if (canSenpaiEnemyGoThere(tilePosition)) break;
	}
	return tilePosition;
}

static int doesEnemyTypeExist(int tType);

static int getNewEnemyType() {
	int ret = -1;
	int i;
	for (i = 0; i < 100; i++) {
		ret = randfromInteger(0, 8);
		if (!doesEnemyTypeExist(ret)) break;
	}
	return ret;
}

static Vector3DI findEnemyTarget(ActiveEnemy* e) {
	if (isSenpaiPositionCrossroads(e->mTargetTilePosition)) {
		return getSenpaiPositionCrossroadsContinueDirection(e->mTargetTilePosition);
	}

	Vector3DI nextStep = makeVector3DI(e->mTilePosition.x + e->mForwardTilePosition.x, e->mTilePosition.y + e->mForwardTilePosition.y, 0);
	if (canSenpaiEnemyGoThere(nextStep)) {
		return nextStep;
	}

	return getSenpaiPositionCrossroadsContinueDirection(e->mTargetTilePosition);
}

static void startWalking(ActiveEnemy* e);

static void walkingFinished(void* tData) {
	ActiveEnemy* e = tData;
	e->mTilePosition = e->mTargetTilePosition;

	startWalking(e);
}

static void startWalking(ActiveEnemy* e) {
	Vector3DI target = findEnemyTarget(e);
	
	double* dst;
	
	if (gData.mIsShowingHorror || !canSenpaiEnemyGoThere(target)) {
		dst = &getAnimationPositionReference(e->mAnimationID)->x;
		e->mCallbackID = tweenDouble(dst, *dst, *dst, linearTweeningFunction, e->mSpeed, walkingFinished, e);
		return;
	}

	e->mTargetTilePosition = target;
	e->mForwardTilePosition = makeVector3DI(e->mTargetTilePosition.x - e->mTilePosition.x, e->mTargetTilePosition.y - e->mTilePosition.y, 0);

	int delta;
	if (target.x > e->mTilePosition.x || target.y > e->mTilePosition.y) delta = 1;
	else delta = -1;

	
	if(target.x > e->mTilePosition.x || target.x < e->mTilePosition.x) dst = &getAnimationPositionReference(e->mAnimationID)->x;
	else dst = &getAnimationPositionReference(e->mAnimationID)->y;

	e->mCallbackID = tweenDouble(dst, *dst, *dst + delta * 64, linearTweeningFunction, e->mSpeed, walkingFinished, e);
}

void addSenpaiEnemy()
{
	ActiveEnemy* e = allocMemory(sizeof(ActiveEnemy));
	e->mTilePosition = getNewEnemyTilePosition();
	e->mTargetTilePosition = e->mTilePosition;
	e->mForwardTilePosition = makeVector3DI(0, 1, 0);
	e->mType = getNewEnemyType();
	e->mSpeed = randfrom(20, 30);
	e->mPosition = makePosition(e->mTilePosition.x*64, e->mTilePosition.y*64, 20);
	e->mAnimationID = playOneFrameAnimationLoop(e->mPosition, &gData.mSmallTextures[e->mType]);
	setAnimationBasePositionReference(e->mAnimationID, getSenpaiLevelBasePositionReference());

	int_map_push_back(&gData.mActiveEnemies, e);

	startWalking(e);
}

static int unloadSingleEnemy(void* tCaller, void* tData) {
	ActiveEnemy* e = tData;

	removeTween(e->mCallbackID);
	removeHandledAnimation(e->mAnimationID);

	return 1;
}

void unloadSenpaiEnemies()
{
	int_map_remove_predicate(&gData.mActiveEnemies, unloadSingleEnemy, NULL);
}

typedef struct {
	Vector3DI mTilePosition;
	int mFound;

} TilePositionSearchCaller;

static void checkActiveEnemyTilePosition(void* tCaller, void* tData) {
	ActiveEnemy* e = tData;
	TilePositionSearchCaller* caller = tCaller;

	if ((e->mTilePosition.x == caller->mTilePosition.x && e->mTilePosition.y == caller->mTilePosition.y) ||
		(e->mTargetTilePosition.x == caller->mTilePosition.x && e->mTargetTilePosition.y == caller->mTilePosition.y)) {
		caller->mFound = 1;
	}
}

int doesContainSenpaiEnemy(Vector3DI tTilePosition)
{
	TilePositionSearchCaller caller;
	caller.mFound = 0;
	caller.mTilePosition = tTilePosition;

	int_map_map(&gData.mActiveEnemies, checkActiveEnemyTilePosition, &caller);
	return caller.mFound;
}

int isSenpaiShowingHorror()
{
	return gData.mIsShowingHorror;
}

void addSenpaiFinalBoss(Vector3DI tTilePosition)
{
	int id = playOneFrameAnimationLoop(makePosition(tTilePosition.x * 64, tTilePosition.y * 64, 20), &gData.mSmallTextures[9]);
	setAnimationBasePositionReference(id, getSenpaiLevelBasePositionReference());
}

MugenSpriteFile * getSenpaiEnemySprites()
{
	return &gData.mSprites;
}

MugenAnimations * getSenpaiEnemyAnimations()
{
	return &gData.mAnimations;
}

typedef struct {
	int mType;
	int mFound;

} TypeSearchCaller;

static void checkActiveEnemyType(void* tCaller, void* tData) {
	ActiveEnemy* e = tData;
	TypeSearchCaller* caller = tCaller;

	if (e->mType == caller->mType) {
		caller->mFound = 1;
	}
}

static int doesEnemyTypeExist(int tType) {
	TypeSearchCaller caller;
	caller.mFound = 0;
	caller.mType = tType;

	int_map_map(&gData.mActiveEnemies, checkActiveEnemyType, &caller);
	return caller.mFound;
}
