#include "beyond_player.h"

#include <tari/animation.h>
#include <tari/input.h>
#include <tari/physicshandler.h>
#include <tari/collisionhandler.h>
#include <tari/log.h>
#include <tari/system.h>
#include <tari/math.h>
#include <tari/screeneffect.h>
#include <tari/wrapper.h>
#include <tari/mugenanimationhandler.h>

#include "beyond_collision.h"
#include "beyond_shothandler.h"
#include "beyond_ui.h"
#include "beyond_effecthandler.h"
#include "beyond_titlescreen.h"
#include "beyond_continuehandler.h"
#include "beyond_gameoverscreen.h"
#include "beyond_boss.h"
#include "beyond_main.h"
#include "beyond_level.h"

static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;

	TextureData mHitboxTexture;
	int mHitBoxAnimationID;

	int mAnimationID;
	int mPhysicsID;
	int mCollisionID;
	
	BeyondCollisionData mCollisionData;
	Collider mCollider;

	int mItemCollisionID;
	Collider mItemCollider;
	
	double mAcceleration;
	double mFocusSpeed;
	double mNormalSpeed;
	int mIsFocused;

	int mIsInCooldown;
	Duration mCooldownNow;
	Duration mCooldownDuration;

	int mIsBombing;
	Duration mBombNow;
	Duration mBombDuration;
	int mIsFinalBossBombing;

	int mIsDying;
	Duration mDyingNow;
	Duration mDyingDuration;

	int mPower;

	int mLocalBombCount;
	int mLocalDeathCount;

	int mLifeAmount;
	int mBombAmount;
	int mContinueAmount;

	int mIsHit;
	Duration mIsHitNow;
	Duration mIsHitDuration;

	int mCanBeHitByEnemies;
	int mFullPower;

	int mHasPowerUpdate;
	int mPowerDelta;
	void(*mPowerUpdateCB)();
} gData;

static void playerHitCB(void* tCaller, void* tCollisionData);

static void loadPlayer(void* tData) {
	(void)tData;
	setBeyondLifeText(gData.mLifeAmount);
	setBeyondBombText(gData.mBombAmount);
	setBeyondPowerText(gData.mPower);

	char path[1024];
	sprintf(path, "assets/main/%s/player/PLAYER.sff", getBeyondDirectory());
	gData.mSprites = loadMugenSpriteFileWithoutPalette(path);
	sprintf(path, "assets/main/%s/player/PLAYER.air", getBeyondDirectory());
	gData.mAnimations = loadMugenAnimationFile(path);

	gData.mPhysicsID = addToPhysicsHandler(makePosition(40, 200, 0));
	setHandledPhysicsDragCoefficient(gData.mPhysicsID, makePosition(1, 1, 0));

	gData.mAnimationID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 1), &gData.mSprites, makePosition(0, 0, 10));
	setMugenAnimationBasePosition(gData.mAnimationID, getHandledPhysicsPositionReference(gData.mPhysicsID));

	gData.mCollisionData.mCollisionList = getBeyondPlayerCollisionList();
	gData.mCollider = makeColliderFromCirc(makeCollisionCirc(makePosition(0, 0, 0), 2));
	gData.mCollisionID = addColliderToCollisionHandler(getBeyondPlayerCollisionList(), getHandledPhysicsPositionReference(gData.mPhysicsID), gData.mCollider, playerHitCB, NULL, &gData.mCollisionData);

	gData.mItemCollider = makeColliderFromCirc(makeCollisionCirc(makePosition(0, 0, 0), 40));
	gData.mItemCollisionID = addColliderToCollisionHandler(getBeyondPlayerItemCollisionList(), getHandledPhysicsPositionReference(gData.mPhysicsID), gData.mItemCollider, playerHitCB, NULL, &gData.mCollisionData);
	
	gData.mHitboxTexture = loadTexture("assets/debug/collision_circ.pkg");
	gData.mHitBoxAnimationID = playOneFrameAnimationLoop(makePosition(-8, -8, 35), &gData.mHitboxTexture);
	setAnimationBasePositionReference(gData.mHitBoxAnimationID, getHandledPhysicsPositionReference(gData.mPhysicsID));
	setAnimationSize(gData.mHitBoxAnimationID, makePosition(4, 4, 0), makePosition(8, 8, 0));
	setAnimationTransparency(gData.mHitBoxAnimationID, 0);
	
	gData.mAcceleration = 10;
	gData.mNormalSpeed = 4;
	gData.mFocusSpeed = 1;
	setHandledPhysicsMaxVelocity(gData.mPhysicsID, gData.mNormalSpeed);

	gData.mIsInCooldown = 0;
	gData.mCooldownNow = 0;

	gData.mCooldownDuration = 20;

	gData.mIsBombing = 0;
	gData.mBombNow = 0;
	gData.mBombDuration = 180;

	gData.mIsFocused = 0;

	gData.mLocalBombCount = 0;
	gData.mLocalDeathCount = 0;

	gData.mIsHit = 0;
	gData.mIsHitDuration = 120;

	gData.mIsDying = 0;
	gData.mDyingDuration = 5;

	gData.mCanBeHitByEnemies = 1;
	gData.mHasPowerUpdate = 0;

	if (isUltimateFrontier()) gData.mFullPower = 128;
	else gData.mFullPower = 400;
}

static void updateMovement() {
	Position* pos = getHandledPhysicsPositionReference(gData.mPhysicsID);
	*pos = clampPositionToGeoRectangle(*pos, makeGeoRectangle(0, 0, 640, 327));
	
	if (hasPressedLeftSingle(0) || hasPressedLeftSingle(1)) {
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(-gData.mAcceleration, 0, 0));
	}
	if (hasPressedRightSingle(0)|| hasPressedRightSingle(1)) {
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(gData.mAcceleration, 0, 0));
	}
	if (hasPressedUpSingle(0) || hasPressedUpSingle(1)) {
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(0, -gData.mAcceleration, 0));
	}
	if (hasPressedDownSingle(0) || hasPressedDownSingle(1)) {
		addAccelerationToHandledPhysics(gData.mPhysicsID, makePosition(0, gData.mAcceleration, 0));
	}
}

static void updateFocus() {
	if (hasPressedRSingle(0) || hasPressedRSingle(1)) {
		setHandledPhysicsMaxVelocity(gData.mPhysicsID, gData.mFocusSpeed);
		setAnimationTransparency(gData.mHitBoxAnimationID, 1);
		gData.mIsFocused = 1;
	}
	else {
		setHandledPhysicsMaxVelocity(gData.mPhysicsID, gData.mNormalSpeed);
		setAnimationTransparency(gData.mHitBoxAnimationID, 0);
		gData.mIsFocused = 0;
	}
}

static void firePlayerShotBeyond() {
	Position p = *getHandledPhysicsPositionReference(gData.mPhysicsID);

	int powerBase = gData.mPower / 100;
	int shotID = gData.mIsFocused * 10 + powerBase;
	addBeyondShot(shotID, getBeyondPlayerShotCollisionList(), p);

	if (hasPressedASingle(0)) {
		addFinalBeyondBossShot(shotID + 30);
	}

	gData.mCooldownNow = 0;
	gData.mIsInCooldown = 1;
}

static void updateShotBeyond() {
	if (gData.mIsInCooldown) {
		if (handleDurationAndCheckIfOver(&gData.mCooldownNow, gData.mCooldownDuration)) {
			gData.mIsInCooldown = 0;
		}
		return;
	}

	if (hasPressedASingle(0) || hasPressedASingle(1)) {
		firePlayerShotBeyond();
	}
}

static void firePlayerShotUltimateFrontier() {
	Position p = *getHandledPhysicsPositionReference(gData.mPhysicsID);

	int level = gData.mPower / 8;
	int shotID;
	if (level < 4) {
		shotID = 10;
	}
	else if (level < 8) {
		shotID = 11;
	}
	else if (level < 16) {
		shotID = 12;
	}
	else {
		shotID = 13;
	}

	addBeyondShot(shotID, getBeyondPlayerShotCollisionList(), p);

	gData.mCooldownNow = 0;
	gData.mIsInCooldown = 1;
}

static void updateShotUltimateFrontier() {
	int level = gData.mPower / 8;

	if (gData.mIsInCooldown) {
		if (handleDurationAndCheckIfOver(&gData.mCooldownNow, gData.mCooldownDuration - level)) {
			gData.mIsInCooldown = 0;
		}
		return;
	}

	if (hasPressedASingle(0) || hasPressedASingle(1)) {
		firePlayerShotUltimateFrontier();
	}
}

static void updateShot() {
	if (isUltimateFrontier()) {
		updateShotUltimateFrontier();
	}
	else {
		updateShotBeyond();
	}
}

static void updateBomb() {
	if (gData.mIsBombing) {
		if (!isUltimateFrontier()) {
			removeBeyondEnemyShots();
		}
		Position p = *getHandledPhysicsPositionReference(gData.mPhysicsID);
		addBeyondShot(20, getBeyondPlayerShotCollisionList(), p);
		if (gData.mIsFinalBossBombing) {
			addFinalBeyondBossShot(50);
		}

		if (handleDurationAndCheckIfOver(&gData.mBombNow, gData.mBombDuration)) {
			gData.mIsBombing = 0;
			if (isUltimateFrontier()) {
				setMugenAnimationTransparency(gData.mAnimationID, 1);
			}
			if (gData.mIsFinalBossBombing) {
				setFinalBeyondBossVulnerable();
			}
		}
		return;
	}
	
	if (!gData.mBombAmount) return;

	int isSecondPort = hasPressedBFlankSingle(1);
	if (hasPressedBFlankSingle(0) || isSecondPort) {
		gData.mLocalBombCount++;
		gData.mBombAmount--;
		setBeyondBombText(gData.mBombAmount);
		gData.mIsBombing = 1;
		gData.mBombNow = 0;
		gData.mIsDying = 0;
		
		if (isUltimateFrontier()) {
			setMugenAnimationTransparency(gData.mAnimationID, 0.5);
		}

		gData.mIsFinalBossBombing = !isSecondPort;
		if (gData.mIsFinalBossBombing) {
			setFinalBeyondBossInvincible();
		}
	}

}

static void updateBeingHit() {
	if (!gData.mIsHit) return;

	if (handleDurationAndCheckIfOver(&gData.mIsHitNow, gData.mIsHitDuration)) {
		gData.mIsHit = 0;
		setMugenAnimationTransparency(gData.mAnimationID, 1);
	}
}

static void setDead();

static void updateDying() {
	if (!gData.mIsDying) return;

	if (handleDurationAndCheckIfOver(&gData.mDyingNow, gData.mDyingDuration)) {
		setDead();
	}
}

static void handleSmallPowerItemCollection() {
	if (isUltimateFrontier() && getBeyondCurrentLevel() == 3 && getBeyondStagePart() == 2) return;

	gData.mPower = min(gData.mFullPower, gData.mPower + 1);
	setBeyondPowerText(gData.mPower);
}

static void updatePowerDrain() {
	if (!gData.mHasPowerUpdate) return;

	gData.mPower += gData.mPowerDelta;
	gData.mPower = min(999, gData.mPower);
	gData.mPower = max(0, gData.mPower);
	setBeyondPowerText(gData.mPower);

	if (gData.mPower == 0 || gData.mPower >= 400) {
		gData.mPowerUpdateCB();
	}

}

static void updatePlayer(void* tData) {
	(void)tData;
	if (isWrapperPaused()) return;

	updateMovement();
	updateFocus();
	updateShot();
	updateBomb();
	updateBeingHit();
	updateDying();
	updatePowerDrain();
}

ActorBlueprint BeyondPlayer = {
	.mLoad = loadPlayer,
	.mUpdate = updatePlayer,
};

static void handleLifeItemCollection() {
	gData.mLifeAmount++;
	setBeyondLifeText(gData.mLifeAmount);
}

static void handleBombItemCollection() {
	gData.mBombAmount++;
	setBeyondBombText(gData.mBombAmount);
}

static void handleItemCollection(BeyondCollisionData* tCollisionData) {
	if (tCollisionData->mItemType == ITEM_TYPE_SMALL_POWER) {
		handleSmallPowerItemCollection();
	} else if (tCollisionData->mItemType == ITEM_TYPE_LIFE) {
		handleLifeItemCollection();
	}
	else if (tCollisionData->mItemType == ITEM_TYPE_BOMB) {
		handleBombItemCollection();
	}
	else {
		logError("Unrecognized item type.");
		logErrorInteger(tCollisionData->mItemType);
		abortSystem();
	}
}

static void setHit() {
	Position pos = *getHandledPhysicsPositionReference(gData.mPhysicsID);
	addBeyondExplosionEffect(pos);

	setMugenAnimationTransparency(gData.mAnimationID, 0.5);
	gData.mIsHit = 1;
	gData.mIsHitNow = 0;
}

static void goToGameOverScreen(void* tCaller) {
	setNewScreen(&BeyondGameOverScreen); 
}

static void setDead() {
	gData.mLocalDeathCount++;

	if (isUltimateFrontier()) {
		gData.mPower /= 2;
	}
	else {
		gData.mPower = max(0, gData.mPower - 100);
	}
	setBeyondPowerText(gData.mPower);

	gData.mBombAmount = max(3, gData.mBombAmount);
	setBeyondBombText(gData.mBombAmount);

	if (!gData.mLifeAmount) {
		if (gData.mContinueAmount) {
			setBeyondContinueActive();
		}
		else {
			addFadeOut(30, goToGameOverScreen, NULL);
		}
	}
	else {
		gData.mLifeAmount--;
		setBeyondLifeText(gData.mLifeAmount);
	}

	setHit();
	gData.mIsDying = 0;
}

static void playerHitCB(void* tCaller, void* tCollisionData) {
	(void)tCaller;
	BeyondCollisionData* collisionData = tCollisionData;

	if (collisionData->mIsItem) {
		handleItemCollection(collisionData);
		return;
	}

	if (gData.mIsHit || gData.mIsBombing) return;
	if (!gData.mCanBeHitByEnemies && collisionData->mCollisionList == getBeyondEnemyCollisionList()) return;
	if (gData.mIsDying) return;

	gData.mIsDying = 1;
	gData.mDyingNow = 0;
}

Position getBeyondPlayerPosition()
{
	return *getHandledPhysicsPositionReference(gData.mPhysicsID);
}

PhysicsObject * getBeyondPlayerPhysics()
{
	return getPhysicsFromHandler(gData.mPhysicsID);
}

double getBeyondPlayerAcceleration()
{
	return gData.mAcceleration;
}

double getBeyondPlayerSpeed()
{
	return gData.mIsFocused ? gData.mFocusSpeed : gData.mNormalSpeed;
}

void resetBeyondPlayerState()
{
	gData.mPower = 0;
	gData.mLifeAmount = 2; 
	gData.mBombAmount = 3;
	gData.mContinueAmount = 5;
}

void resetBeyondLocalPlayerCounts()
{
	gData.mLocalBombCount = 0;
	gData.mLocalDeathCount = 0;
}

void getBeyondLocalDeathCountVariable(char * tDst, void * tCaller)
{
	(void)tCaller;
	sprintf(tDst, "%d", gData.mLocalDeathCount);
}

void getBeyondLocalBombCountVariable(char * tDst, void * tCaller)
{
	(void)tCaller;
	sprintf(tDst, "%d", gData.mLocalBombCount);
}

void setBeyondPlayerToFullPower()
{
	gData.mPower = gData.mFullPower;

	if (isUltimateFrontier() && getBeyondCurrentLevel() == 3 && getBeyondStagePart() == 2) {
		gData.mPower = 0;
		gData.mBombAmount = 0;
	}

	setBeyondPowerText(gData.mPower);
	setBeyondLifeText(gData.mLifeAmount); // TODO: move
	setBeyondBombText(gData.mBombAmount); // TODO: move
}

int getBeyondContinueAmount()
{
	return gData.mContinueAmount;
}

void reduceBeyondContinueAmount()
{
	gData.mLifeAmount = 2;
	gData.mBombAmount = 3;
	gData.mContinueAmount--;
}

void disableBeyondPlayerBossCollision()
{
	gData.mCanBeHitByEnemies = 0;
}

void drainBeyondPlayerPower(void(*tCB)())
{
	gData.mHasPowerUpdate = 1;
	gData.mPowerDelta = -1;
	gData.mPowerUpdateCB = tCB;
}

void fillBeyondPlayerPower(void(*tCB)())
{
	gData.mHasPowerUpdate = 1;
	gData.mPowerDelta = 1;
	gData.mPowerUpdateCB = tCB;
}

void burnBeyondPlayer()
{
	gData.mBombAmount = 0;
	setBeyondBombText(gData.mBombAmount);
	gData.mPower = 0;
	setBeyondPowerText(gData.mPower);
}
