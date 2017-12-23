#include "beyond_shothandler.h"

#include <assert.h>

#include <tari/mugenspritefilereader.h>
#include <tari/mugenanimationreader.h>
#include <tari/collisionhandler.h>
#include <tari/physicshandler.h>
#include <tari/log.h>
#include <tari/system.h>
#include <tari/mugenscriptparser.h>
#include <tari/mugenassignmentevaluator.h>
#include <tari/math.h>
#include <tari/wrapper.h>
#include <tari/drawing.h>

#include "beyond_collision.h"
#include "beyond_enemyhandler.h"
#include "beyond_boss.h"
#include "beyond_player.h"
#include "beyond_main.h"

typedef enum {
	SHOT_TYPE_NORMAL,
	SHOT_TYPE_HOMING,
	SHOT_TYPE_HOMING_FINAL,
	SHOT_TYPE_TARGET_RANDOM,
	SHOT_TYPE_TARGET_RANDOM_FINAL,
} ShotHomingType;

typedef struct {
	Vector3D mOffset;
	TextureData mTexture;

} ShotTexture;

typedef struct {
	MugenAssignment* mAmount;

	ShotHomingType mHomingType;

	MugenAssignment* mOffset;

	int mHasAbsolutePosition;
	MugenAssignment* mAbsolutePosition;

	int mHasVelocity;
	MugenAssignment* mVelocity;
	int mHasAngle;
	MugenAssignment* mAngle;
	int mHasSpeed;
	MugenAssignment* mSpeed;

	MugenAssignment* mStartRotation;
	MugenAssignment* mRotationAdd;

	MugenAssignment* mGimmick;

	ShotTexture* mTexture;
	int mIdleAnimation;
	int mHitAnimation;

	CollisionCirc mColCirc;
	MugenAssignment* mColor;
} SubShotType;

typedef struct {
	int mID;
	IntMap mSubShots;
} ShotType;


typedef struct {
	ShotType* mType;
	IntMap mSubShots;
	int mSubShotsLeft;

	BeyondCollisionData mCollisionData;
} ActiveShot;


typedef struct {
	ActiveShot* mRoot;

	SubShotType* mType;

	int mPhysicsID;
	int mCollisionID;

	Collider mCollider;
	int mListID;

	double mRotation;

	double mR;
	double mG;
	double mB;

	double mZ;

	int mHasGimmickData;
	void* mGimmickData;

	int mIsStillActive;
} ActiveSubShot;



static struct {
	IntMap mShotTextures;

	IntMap mShotTypes;

	IntMap mActiveShots;

	int mFinalBossShotsDeflected;
} gData;

static ShotType* gActiveShotType;

static int isShotType(MugenDefScriptGroup* tGroup) {
	return !strcmp("Shot", tGroup->mName);
}

static void handleNewShotType(MugenDefScriptGroup* tGroup) {
	ShotType* e = allocMemory(sizeof(ShotType));
	e->mID = getMugenDefNumberVariableAsGroup(tGroup, "id");
	e->mSubShots = new_int_map();

	int_map_push_owned(&gData.mShotTypes, e->mID, e);

	gActiveShotType = e;
}

static int isSubShotType(MugenDefScriptGroup* tGroup) {
	return !strcmp("SubShot", tGroup->mName);
}

static void parseHomingType(SubShotType* e, MugenDefScriptGroup* tGroup) {

	char* text = getAllocatedMugenDefStringVariableAsGroup(tGroup, "type");
	if (!strcmp("normal", text)) {
		e->mHomingType = SHOT_TYPE_NORMAL;
	}
	else if (!strcmp("homing", text)) {
		e->mHomingType = SHOT_TYPE_HOMING;
	}
	else if (!strcmp("homing_final", text)) {
		e->mHomingType = SHOT_TYPE_HOMING_FINAL;
	}
	else if (!strcmp("targetrandom", text)) {
		e->mHomingType = SHOT_TYPE_TARGET_RANDOM;
	}
	else if (!strcmp("targetrandom_final", text)) {
		e->mHomingType = SHOT_TYPE_TARGET_RANDOM_FINAL;
	}
	else {
		logError("Unrecognized type");
		logErrorString(text);
		abortSystem();
	}

	freeMemory(text);
}

static ShotTexture* getShotTextureReference(int tID) {
	ShotTexture* e = int_map_get(&gData.mShotTextures, tID);
	return e;
}


static void handleNewSubShotType(MugenDefScriptGroup* tGroup) {
	assert(gActiveShotType);

	SubShotType* e = allocMemory(sizeof(SubShotType));
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("amount", tGroup, &e->mAmount, "");
	e->mIdleAnimation = getMugenDefNumberVariableAsGroup(tGroup, "anim");
	e->mHitAnimation = getMugenDefNumberVariableAsGroup(tGroup, "hitanim");
	e->mTexture = getShotTextureReference(e->mIdleAnimation);
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("offset", tGroup, &e->mOffset, "");
	e->mHasAbsolutePosition = fetchMugenAssignmentFromGroupAndReturnWhetherItExists("absolute", tGroup, &e->mAbsolutePosition);
	e->mHasVelocity = fetchMugenAssignmentFromGroupAndReturnWhetherItExists("velocity", tGroup, &e->mVelocity);
	e->mHasAngle = fetchMugenAssignmentFromGroupAndReturnWhetherItExists("angle", tGroup, &e->mAngle);
	e->mHasSpeed = fetchMugenAssignmentFromGroupAndReturnWhetherItExists("speed", tGroup, &e->mSpeed);
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("rotation", tGroup, &e->mStartRotation, "");
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("rotationadd", tGroup, &e->mRotationAdd, "");
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("color", tGroup, &e->mColor, "white");
	fetchMugenAssignmentFromGroupAndReturnWhetherItExistsDefaultString("gimmick", tGroup, &e->mGimmick, "");

	Position center = getMugenDefVectorOrDefaultAsGroup(tGroup, "center", makePosition(0, 0, 0));
	double radius = getMugenDefFloatVariableAsGroup(tGroup, "radius");
	e->mColCirc = makeCollisionCirc(center, radius);
	parseHomingType(e, tGroup);

	int_map_push_back_owned(&gActiveShotType->mSubShots, e);
}

static void loadShotTypesFromScript(MugenDefScript* tScript) {
	gActiveShotType = NULL;

	resetMugenScriptParser();
	addMugenScriptParseFunction(isShotType, handleNewShotType);
	addMugenScriptParseFunction(isSubShotType, handleNewSubShotType);
	parseMugenScript(tScript);
}

static void loadSingleShotTexture(int tID, Vector3D tOffset) {
	ShotTexture* e = allocMemory(sizeof(ShotTexture));
	
	char path[1024];
	sprintf(path, "assets/main/%s/shots/sprites/%d.pkg", getBeyondDirectory(), tID);

	e->mTexture = loadTexture(path);	
	e->mOffset = tOffset;

	int_map_push_owned(&gData.mShotTextures, tID, e);
}

static void loadShotTextures() {
	loadSingleShotTexture(1, makePosition(7, 7, 0));
	loadSingleShotTexture(2, makePosition(7, 7, 0));
	loadSingleShotTexture(3, makePosition(8, 4, 0));
	loadSingleShotTexture(4, makePosition(5, 2, 0));
	loadSingleShotTexture(5, makePosition(4, 4, 0));
	loadSingleShotTexture(1001, makePosition(11, 6, 0));
	loadSingleShotTexture(2001, makePosition(15, 11, 0));
	loadSingleShotTexture(4001, makePosition(16, 16, 0));
	loadSingleShotTexture(5001, makePosition(8, 8, 0));
}

static void loadShotHandler(void* tData) {
	(void)tData;


	gData.mShotTextures = new_int_map();
	gData.mShotTypes = new_int_map();
	gData.mActiveShots = new_int_map();

	loadShotTextures();

	char path[1024];
	sprintf(path, "assets/main/%s/shots/SHOTS.def", getBeyondDirectory());
	MugenDefScript script = loadMugenDefScript(path);
	loadShotTypesFromScript(&script);
	unloadMugenDefScript(script);

	gData.mFinalBossShotsDeflected = 0;
}

static void unloadSubShot(ActiveSubShot* e) {
	if (e->mHasGimmickData) {
		freeMemory(e->mGimmickData);
	}

	removeFromCollisionHandler(e->mRoot->mCollisionData.mCollisionList, e->mCollisionID);
	destroyCollider(&e->mCollider);
	removeFromPhysicsHandler(e->mPhysicsID);
	e->mRoot->mSubShotsLeft--;
}

static Position getClosestEnemyPositionIncludingBoss(Position p) {
	Position closest = getClosestBeyondEnemyPosition(p);

	if (!isBeyondBossActive()) return closest;

	Position bPosition = getBeyondBossPosition();
	if (getDistance2D(p, closest) < 1e-6 || getDistance2D(p, bPosition) < getDistance2D(p, closest)) {
		return bPosition;
	}
	else {
		return closest;
	}
}

static void updateHoming(ActiveSubShot* e) {
	if (e->mType->mHomingType != SHOT_TYPE_HOMING) return;
	Position p = *getHandledPhysicsPositionReference(e->mPhysicsID);
	Position closestEnemy = getClosestEnemyPositionIncludingBoss(p);

	Velocity* vel = getHandledPhysicsVelocityReference(e->mPhysicsID);
	Vector3D dir = vecScale(vecNormalize(vecSub(closestEnemy, p)), vecLength(*vel));
	dir.z = 0;
	if (vecLength(dir) < 1e-6) return;

	double angle = getAngleFromDirection(dir);
	e->mRotation = angle;
	*vel = dir;
}

static void updateFinalHoming(ActiveSubShot* e) {
	if (e->mType->mHomingType != SHOT_TYPE_HOMING_FINAL) return;
	Position p = *getHandledPhysicsPositionReference(e->mPhysicsID);
	Position closestEnemy = getBeyondPlayerPosition();

	Velocity* vel = getHandledPhysicsVelocityReference(e->mPhysicsID);
	Vector3D dir = vecScale(vecNormalize(vecSub(closestEnemy, p)), vecLength(*vel));
	dir.z = 0;
	if (vecLength(dir) < 1e-6) return;

	double angle = getAngleFromDirection(dir);
	e->mRotation = angle;
	*vel = dir;
}

static void updateRotation(ActiveSubShot* e) {

	SubShotType* subShot = e->mType;

	double rotationAdd = getMugenAssignmentAsFloatValueOrDefaultWhenEmpty(subShot->mRotationAdd, NULL, 0);
	e->mRotation += rotationAdd;
}

static void updateGimmick(ActiveSubShot* e) {
	getMugenAssignmentAsIntegerValueOrDefaultWhenEmpty(e->mType->mGimmick, e, 0);
}

static int updateSubShot(void* tCaller, void* tData) {
	(void)tCaller;
	ActiveSubShot* e = tData;

	updateRotation(e);
	updateHoming(e);
	updateFinalHoming(e);
	updateGimmick(e);

	Position p = *getHandledPhysicsPositionReference(e->mPhysicsID);
	if (!e->mIsStillActive || p.x < -100 || p.x > 740 || p.y < -100 || p.y > 480) {
		unloadSubShot(e);
		return 1;
	}

	return 0;
}

static int unloadSubShotCB(void* tCaller, void* tData) {
	(void)tCaller;
	ActiveSubShot* e = tData;
	unloadSubShot(e);
	return 1;
}

static void unloadShot(ActiveShot* e) {
	int_map_remove_predicate(&e->mSubShots, unloadSubShotCB, NULL);
	delete_int_map(&e->mSubShots);
}

static int updateShot(void* tCaller, void* tData) {
	(void)tCaller;
	ActiveShot* e = tData;

	int_map_remove_predicate(&e->mSubShots, updateSubShot, e);

	if (!e->mSubShotsLeft) {
		unloadShot(e);
		return 1;
	}

	return 0;
}

static void updateActiveShots() {
	int_map_remove_predicate(&gData.mActiveShots, updateShot, NULL);
}

static void updateShotHandler(void* tData) {
	(void)tData;
	if (isWrapperPaused()) return;
	updateActiveShots();
}

static void drawSubShot(void* tCaller, void* tData) {
	(void)tCaller;
	ActiveSubShot* e = tData;

	Position p = *getHandledPhysicsPositionReference(e->mPhysicsID);
	Position center = e->mType->mTexture->mOffset;
	Position screenCenter = p;
	p = vecSub(p, center);
	p.z = e->mZ;

	setDrawingBaseColorAdvanced(e->mR, e->mG, e->mB);
	setDrawingRotationZ(e->mRotation, screenCenter);
	drawSprite(e->mType->mTexture->mTexture, p, makeRectangleFromTexture(e->mType->mTexture->mTexture));
}

static void drawShot(void* tCaller, void* tData) {
	(void)tCaller;
	ActiveShot* e = tData;

	int_map_map(&e->mSubShots, drawSubShot, e);
}

static void drawShotHandler(void* tData) {
	int_map_map(&gData.mActiveShots, drawShot, NULL);
	setDrawingParametersToIdentity();
}

ActorBlueprint BeyondShotHandler = {
	.mLoad = loadShotHandler,
	.mUpdate = updateShotHandler,
	.mDraw = drawShotHandler,
};



static void shotHitCB(void* tCaller, void* tCollisionData) {
	(void)tCollisionData;
	ActiveSubShot* e = tCaller;
	unloadSubShot(e); 
	int_map_remove(&e->mRoot->mSubShots, e->mListID);
}

static void finalBossShotHitCB(void* tCaller, void* tCollisionData) {
	gData.mFinalBossShotsDeflected++;
	
	shotHitCB(tCaller, tCollisionData);
}

static Position getRandomEnemyOrBossPosition() {
	Position p = getRandomBeyondEnemyPosition();

	if (!isBeyondBossActive()) {
		if (p.x == INF) p = makePosition(randfrom(-100, 740), randfrom(-100, 580), 0);
		return p;
	}

	Position bossP = getBeyondBossPosition();
	if (p.x == INF) return bossP;

	int amount = getBeyondEnemyAmount();
	double probability = 1 / (amount + 1);
	if (randfrom(0, 1) <= probability) return bossP;
	else return p;
}

typedef struct {
	ActiveShot* mRoot;
	Position mPosition;
} SubShotCaller;

static void swap(double* a, double* b) {
	double c = *a;
	*a = *b;
	*b = c;
}

typedef struct {
	ActiveSubShot* mActiveShot;
	SubShotCaller* mActiveCaller;
	Position* mOffsetReference; // TODO: better
	int i;
} SubShotAssignmentParseCaller;

void getBeyondCurrentSubShotIndex(char* tOutput, void* tCaller) {
	SubShotAssignmentParseCaller* caller = tCaller;

	sprintf(tOutput, "%d", caller->i);
}

void getBeyondShotAngleTowardsPlayer(char* tOutput, void* tCaller) {
	SubShotAssignmentParseCaller* caller = tCaller;

	Position pos = vecAdd(caller->mActiveCaller->mPosition, *caller->mOffsetReference);
	Position playerPos = getBeyondPlayerPosition();

	Vector3D direction = vecNormalize(vecSub(playerPos, pos));
	direction.x *= -1; // TODO
	double angle = getAngleFromDirection(direction);

	sprintf(tOutput, "%f", angle);
}

static void setShotColor(SubShotType* subShot, ActiveSubShot* e, SubShotAssignmentParseCaller* caller) {
	double r, g, b;

	char* text = evaluateMugenAssignmentAndReturnAsAllocatedString(subShot->mColor, caller);

	if (!strcmp("white", text)) {
		r = g = b = 1;
	}
	else if (!strcmp("red", text)) {
		r = 1;
		b = g = 0;
	}
	else if (!strcmp("grey", text)) {
		r = g = b = 0.5;
	}
	else if (!strcmp("yellow", text)) {
		r = g = 1;
		b = 0;
	}
	else if (!strcmp("purple", text)) {
		r = b = 1;
		g = 0;
	}
	else if (!strcmp("rainbow", text)) {
		r = randfromInteger(0, 1);
		g = randfromInteger(0, 1);
		b = randfromInteger(0, 1);
		if (!r && !g && !b) r = 1;
	}
	else if (!strcmp("green", text)) {
		r = 0;
		g = 1;
		b = 0;
	}
	else {
		r = g = b = 1;
		logError("Unrecognized color.");
		logErrorString(text);
		abortSystem();
	}

	e->mR = r;
	e->mG = g;
	e->mB = b;
}

static void addSingleSubShot(SubShotCaller* caller, SubShotType* subShot, int i) {
	ActiveSubShot* e = allocMemory(sizeof(ActiveSubShot));
	e->mType = subShot;
	e->mRoot = caller->mRoot;

	SubShotAssignmentParseCaller assignmentCaller;
	assignmentCaller.mActiveCaller = caller;
	assignmentCaller.mActiveShot = e;
	assignmentCaller.i = i;

	Vector3D offset = getMugenAssignmentAsVector3DValueOrDefaultWhenEmpty(subShot->mOffset, &assignmentCaller, makePosition(0, 0, 0));
	Vector3D velocity = makePosition(0, 0, 0);
	assignmentCaller.mOffsetReference = &offset; // TODO: better

	if (subShot->mHasAbsolutePosition) {
		caller->mPosition = getMugenAssignmentAsVector3DValueOrDefaultWhenEmpty(subShot->mAbsolutePosition, &assignmentCaller, makePosition(0, 0, 0));
	}

	double angle = 0;
	if (subShot->mHasVelocity) {
		velocity = getMugenAssignmentAsVector3DValueOrDefaultWhenEmpty(subShot->mVelocity, &assignmentCaller, makePosition(0, 0, 0));
		angle = getAngleFromDirection(velocity);
	}
	
	if (subShot->mHasAngle) {
		angle = evaluateMugenAssignmentAndReturnAsFloat(subShot->mAngle, &assignmentCaller);
		velocity = getDirectionFromAngleZ(angle);	
		angle *= -1; // TODO: fix
	}

	if (subShot->mHomingType == SHOT_TYPE_TARGET_RANDOM || subShot->mHomingType == SHOT_TYPE_TARGET_RANDOM_FINAL) {
		Position p = vecAdd(caller->mPosition, offset);
		Position enemyPos;
		if (subShot->mHomingType == SHOT_TYPE_TARGET_RANDOM) {
			enemyPos = getRandomEnemyOrBossPosition();
		}
		else {
			enemyPos = getBeyondPlayerPosition();
		}
		
		swap(&enemyPos.x, &p.x);
		angle = getAngleFromDirection(vecSub(enemyPos, p));
		velocity = getDirectionFromAngleZ(angle);
		//offset = vecRotateZ(offset, angle);
		angle *= -1; // TODO: fix
	}
	
	if (subShot->mHasSpeed) {
		double speed = evaluateMugenAssignmentAndReturnAsFloat(subShot->mSpeed, &assignmentCaller);
		velocity = vecScale(vecNormalize(velocity), speed);
	}

	if (caller->mRoot->mCollisionData.mCollisionList == getBeyondEnemyShotCollisionList()) e->mZ = 30;
	else e->mZ = 25;

	e->mPhysicsID = addToPhysicsHandler(vecAdd(caller->mPosition, offset));
	addAccelerationToHandledPhysics(e->mPhysicsID, velocity);

	e->mCollider = makeColliderFromCirc(subShot->mColCirc);

	void(*hitCB)(void*, void*);
	if (caller->mRoot->mCollisionData.mCollisionList == getBeyondEnemyCollisionList()) hitCB = finalBossShotHitCB;
	else hitCB = shotHitCB;

	e->mCollisionID = addColliderToCollisionHandler(caller->mRoot->mCollisionData.mCollisionList, getHandledPhysicsPositionReference(e->mPhysicsID), e->mCollider, hitCB, e, &caller->mRoot->mCollisionData);


	e->mRotation = getMugenAssignmentAsFloatValueOrDefaultWhenEmpty(subShot->mStartRotation, &assignmentCaller, angle);

	setShotColor(subShot, e, &assignmentCaller);

	e->mHasGimmickData = 0;
	e->mIsStillActive = 1;

	caller->mRoot->mSubShotsLeft++;
	e->mListID = int_map_push_back_owned(&caller->mRoot->mSubShots, e);
}

static void addSubShot(void* tCaller, void* tData) {
	SubShotCaller* caller = tCaller;
	SubShotType* subShot = tData;

	int amount = getMugenAssignmentAsIntegerValueOrDefaultWhenEmpty(subShot->mAmount, NULL, 1);
	int i;
	for (i = 0; i < amount; i++) {
		addSingleSubShot(caller, subShot, i);
	}
}

void addBeyondShot(int tID, int tCollisionList, Position tPosition)
{
	ActiveShot* e = allocMemory(sizeof(ActiveShot));
	assert(int_map_contains(&gData.mShotTypes, tID));
	e->mType = int_map_get(&gData.mShotTypes, tID);
	e->mCollisionData.mCollisionList = tCollisionList;
	e->mCollisionData.mIsItem = 0;
	e->mSubShotsLeft = 0;
	e->mSubShots = new_int_map();
	int_map_push_back_owned(&gData.mActiveShots, e);

	SubShotCaller caller;
	caller.mRoot = e;
	caller.mPosition = tPosition;
	int_map_map(&e->mType->mSubShots, addSubShot, &caller);
}

typedef struct {
	int mCollisionList;
} RemoveShotsForSingleListCaller;

static int removeShotTypeForSingleSubShot(void* tCaller, void* tData) {
	(void)tCaller;
	ActiveSubShot* e = tData;
	unloadSubShot(e);
	return 1;
}

static void removeShotTypeForSingleShot(void* tCaller, void* tData) {
	ActiveShot* e = tData;
	RemoveShotsForSingleListCaller* caller = tCaller;
	if (caller->mCollisionList != e->mCollisionData.mCollisionList) return;

	int_map_remove_predicate(&e->mSubShots, removeShotTypeForSingleSubShot, NULL);
}

void removeBeyondEnemyShots()
{
	RemoveShotsForSingleListCaller caller;
	caller.mCollisionList = getBeyondEnemyShotCollisionList();
	int_map_map(&gData.mActiveShots, removeShotTypeForSingleShot, &caller);
}

typedef struct {
	Position mTarget;
	int mState;
} BigBangData;

static void bangOut(BigBangData* data) {
	int side = randfromInteger(0, 3);
	if (side == 0) {
		data->mTarget = makePosition(randfrom(0, 640), 0, 0);
	}
	else if (side == 1) {
		data->mTarget = makePosition(randfrom(0, 640), 327, 0);
	}
	else if (side == 2) {
		data->mTarget = makePosition(0, randfrom(0, 327), 0);
	}
	else {
		data->mTarget = makePosition(0, randfrom(0, 327), 0);
	}

	data->mState = 0;
}

static void bangIn(BigBangData* data) {

	data->mTarget = makePosition(320, 163, 0);
	data->mState = 1;
}

static void loadBigBang(ActiveSubShot* e) {
	e->mGimmickData = allocMemory(sizeof(BigBangData));
	e->mHasGimmickData = 1;

	BigBangData* data = e->mGimmickData;
	bangOut(data);
}

void evaluateBeyondBigBangFunction(char * tDst, void * tCaller)
{
	ActiveSubShot* e = tCaller;

	if (!e->mHasGimmickData) {
		loadBigBang(e);
	}
	
	BigBangData* data = e->mGimmickData;
	Position pos = *getHandledPhysicsPositionReference(e->mPhysicsID);
	Vector3D* vel = getHandledPhysicsVelocityReference(e->mPhysicsID);
	
	Vector3D delta = vecSub(data->mTarget, pos);
	double l = vecLength(delta);
	if (l < 2) {
		if (data->mState) bangOut(data);
		else bangIn(data);
	}
		
	*vel = vecScale(vecNormalize(delta), 2);
	
	strcpy(tDst, "");
}

void evaluateBeyondBounceFunction(char * tDst, void * tCaller)
{
	ActiveSubShot* e = tCaller;
	Position pos = *getHandledPhysicsPositionReference(e->mPhysicsID);
	Velocity* vel = getHandledPhysicsVelocityReference(e->mPhysicsID);

	if (pos.x < 0) vel->x = 1;
	if (pos.x > 640) vel->x = -1;

	strcpy(tDst, "");
}

typedef struct {

	Velocity mDirection;
	double mState;
	int mIsActive;
} AckermannData;

static void initAckermann(ActiveSubShot* e) {
	e->mGimmickData = allocMemory(sizeof(AckermannData));
	e->mHasGimmickData = 1;

	AckermannData* data = e->mGimmickData;
	data->mIsActive = 0;
}

void evaluateBeyondAckermannFunction(char * tDst, void * tCaller)
{
	ActiveSubShot* e = tCaller;
	if (!e->mHasGimmickData) {
		initAckermann(e);
	}

	Velocity* vel = getHandledPhysicsVelocityReference(e->mPhysicsID);
	AckermannData* data = e->mGimmickData;
	if (!data->mIsActive) {
		if (vecLength(*vel) > 0 && randfrom(0, 1) < 0.005) {
			data->mIsActive = 1;
			data->mState = 0;
			data->mDirection = *vel;
		}
	}
	else {
		data->mState = min(data->mState + 1 / 60.0, 1);
		*vel = vecRotateZ(data->mDirection, 2*M_PI*data->mState);
		double angle = getAngleFromDirection(*vel);
		e->mRotation = angle;
	}

	strcpy(tDst, "");
}

void evaluateBeyondSwirlFunction(char * tDst, void * tCaller)
{
	ActiveSubShot* e = tCaller;
	Velocity* vel = getHandledPhysicsVelocityReference(e->mPhysicsID);
	*vel = vecRotateZ(*vel, 0.01);
	double angle = getAngleFromDirection(*vel);
	e->mRotation = angle;

	strcpy(tDst, "");
}

void evaluateBeyondBlamFunction(char * tDst, void * tCaller)
{

	ActiveSubShot* e = tCaller;
	Velocity* vel = getHandledPhysicsVelocityReference(e->mPhysicsID);
	
	double l = vecLength(*vel);
	if (l > 0) {
		*vel = vecScale(vecNormalize(*vel), min(l*1.1, 20));
	}

	strcpy(tDst, "");
}


typedef struct {
	Duration mNow;	
} TransienceData;

static void initTransience(ActiveSubShot* e) {
	e->mGimmickData = allocMemory(sizeof(TransienceData));
	e->mHasGimmickData = 1;

	TransienceData* data = e->mGimmickData;
	data->mNow = 0;
}

void evaluateBeyondTransienceFunction(char * tDst, void * tCaller)
{
	ActiveSubShot* e = tCaller;
	if (!e->mHasGimmickData) {
		initTransience(e);
	}
	TransienceData* data = e->mGimmickData;

	if (handleDurationAndCheckIfOver(&data->mNow, 60)) {
		e->mIsStillActive = 0;
	}

	strcpy(tDst, "");
}

int getBeyondFinalBossShotsDeflected()
{
	return gData.mFinalBossShotsDeflected;
}
