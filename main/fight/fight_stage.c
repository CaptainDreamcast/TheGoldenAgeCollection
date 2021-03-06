#include "fight_stage.h"

#include <stdio.h>
#include <assert.h>

#include <prism/memoryhandler.h>
#include <prism/log.h>
#include <prism/system.h>
#include <prism/animation.h>
#include <prism/stagehandler.h>
#include <prism/math.h>
#include <prism/input.h>
#include <prism/mugendefreader.h>
#include <prism/mugenspritefilereader.h>

#include "fight_playerdefinition.h"
#include "fight_mugenstagehandler.h"

typedef struct {
	char mName[1024];
	char mDisplayName[1024];
	char mVersionDate[100];
	char mMugenVersion[100];
	char mAuthor[100];
} StageInfo;

typedef struct {
	Position mStartPosition;
	double mBoundLeft;
	double mBoundRight;
	double mBoundHigh;
	double mBoundLow;

	double mTension;
	double mTensionHigh;
	double mTensionLow;

	double mVerticalFollow;
	double mFloorTension;

	int mOverdrawHigh;
	int mOverdrawLow;
	int mCutHigh;
	int mCutLow;

	double mStartZoom;
	double mZoomOut;
	double mZoomIn;

} StageCamera;

typedef struct {
	Position mP1Start;
	Position mP2Start;

	int mP1Facing;
	int mP2Facing;

	double mLeftBound;
	double mRightBound;

} StagePlayerInfo;

typedef struct {
	double mScreenLeft;
	double mScreenRight;
} StageBound;

typedef struct {
	double mZOffset;
	int mZOffsetLink;
	int mAutoturn;
	int mResetBG;
	Vector3DI mLocalCoordinates;
	Vector3D mScale;

} StageStageInfo;

typedef struct {
	int mIntensity;
	Vector3D mColor;
	double mScaleY;
	Vector3DI mFadeRange;
	double mXShear;

} StageShadow;

typedef struct {
	int mReflect;
} StageReflection;

typedef struct {
	char mBGMusic[1024];
	int mBGVolume;
} StageMusic;

typedef struct {
	char mSpritePath[1024];
	int mDebugBG;

} StageBackgroundDefinition;

typedef enum {
	STAGE_BACKGROUND_STATIC,
	STAGE_BACKGROUND_ANIMATED,
} StageBackgroundType;

typedef struct {
	StageBackgroundType mType;
	Vector3DI mSpriteNo;
	int mLayerNo;
	Position mStart;
	Position mDelta;
	int mMask;
	Vector3DI mTile;
	Vector3DI mTileSpacing;
	int mListPosition;

	int mActionNumber;
} StageBackgroundElement;

static struct {
	StageInfo mInfo;
	StageCamera mCamera;
	StagePlayerInfo mPlayerInfo;
	StageBound mBound;
	StageStageInfo mStageInfo;
	StageShadow mShadow;
	StageReflection mReflection;
	StageMusic mMusic;
	StageBackgroundDefinition mBackgroundDefinition;

	List mBackgroundElements;

	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;

	char mDefinitionPath[1024];
} gData;

static void loadStageInfo(MugenDefScript* s) {
	getMugenDefStringOrDefault(gData.mInfo.mName, s, "Info", "name", "");
	getMugenDefStringOrDefault(gData.mInfo.mDisplayName, s, "Info", "displayname", gData.mInfo.mName);
	getMugenDefStringOrDefault(gData.mInfo.mVersionDate, s, "Info", "versiondate", "1.0");
	getMugenDefStringOrDefault(gData.mInfo.mMugenVersion, s, "Info", "mugenversion", "1.0");
	getMugenDefStringOrDefault(gData.mInfo.mAuthor, s, "Info", "author", "John Doe");
}

static void loadStageCamera(MugenDefScript* s) {
	gData.mCamera.mStartPosition = makePosition(0, 0, 0);
	gData.mCamera.mStartPosition.x = getMugenDefFloatOrDefault(s, "Camera", "startx", 0);
	gData.mCamera.mStartPosition.y = getMugenDefFloatOrDefault(s, "Camera", "starty", 0);
	gData.mCamera.mBoundLeft = getMugenDefFloatOrDefault(s, "Camera", "boundleft", 0);
	gData.mCamera.mBoundRight = getMugenDefFloatOrDefault(s, "Camera", "boundright", 0);
	gData.mCamera.mBoundHigh = getMugenDefFloatOrDefault(s, "Camera", "boundhigh", 0);
	gData.mCamera.mBoundLow = getMugenDefFloatOrDefault(s, "Camera", "boundlow", 0);
	gData.mCamera.mTension = getMugenDefFloatOrDefault(s, "Camera", "tension", 0);
	gData.mCamera.mTensionHigh = getMugenDefFloatOrDefault(s, "Camera", "tensionhigh", 0);
	gData.mCamera.mTensionLow = getMugenDefFloatOrDefault(s, "Camera", "tensionlow", 0);
	gData.mCamera.mVerticalFollow = getMugenDefFloatOrDefault(s, "Camera", "verticalfollow", 0);
	gData.mCamera.mFloorTension = getMugenDefFloatOrDefault(s, "Camera", "floortension", 0);
	gData.mCamera.mOverdrawHigh = getMugenDefIntegerOrDefault(s, "Camera", "overdrawhigh", 0);
	gData.mCamera.mOverdrawLow = getMugenDefIntegerOrDefault(s, "Camera", "overdrawlow", 0);
	gData.mCamera.mCutHigh = getMugenDefIntegerOrDefault(s, "Camera", "cuthigh", 0);
	gData.mCamera.mCutLow = getMugenDefIntegerOrDefault(s, "Camera", "cutlow", 0);
	gData.mCamera.mStartZoom = getMugenDefFloatOrDefault(s, "Camera", "startzoom", 1);
	gData.mCamera.mZoomOut = getMugenDefFloatOrDefault(s, "Camera", "zoomout", 1);
	gData.mCamera.mZoomIn = getMugenDefFloatOrDefault(s, "Camera", "zoomin", 1);
}

static void loadStagePlayerInfo(MugenDefScript* s) {

	gData.mPlayerInfo.mP1Start = makePosition(0, 0, 0);
	gData.mPlayerInfo.mP1Start.x = getMugenDefFloatOrDefault(s, "PlayerInfo", "p1startx", 0);
	gData.mPlayerInfo.mP1Start.y = getMugenDefFloatOrDefault(s, "PlayerInfo", "p1starty", 0);

	gData.mPlayerInfo.mP2Start = makePosition(0, 0, 0);
	gData.mPlayerInfo.mP2Start.x = getMugenDefFloatOrDefault(s, "PlayerInfo", "p2startx", 0);
	gData.mPlayerInfo.mP2Start.y = getMugenDefFloatOrDefault(s, "PlayerInfo", "p2starty", 0);

	gData.mPlayerInfo.mP1Facing = getMugenDefIntegerOrDefault(s, "PlayerInfo", "p1facing", 0);
	gData.mPlayerInfo.mP2Facing = getMugenDefIntegerOrDefault(s, "PlayerInfo", "p2facing", 0);

	gData.mPlayerInfo.mLeftBound = getMugenDefFloatOrDefault(s, "PlayerInfo", "leftbound", 0);
	gData.mPlayerInfo.mRightBound = getMugenDefFloatOrDefault(s, "PlayerInfo", "rightbound", 0);
}

static void loadStageBound(MugenDefScript* s) {
	gData.mBound.mScreenLeft = getMugenDefFloatOrDefault(s, "Bound", "screenleft", 0);
	gData.mBound.mScreenRight = getMugenDefFloatOrDefault(s, "Bound", "screenright", 0);
}

static void loadStageStageInfo(MugenDefScript* s) {
	gData.mStageInfo.mZOffset = getMugenDefFloatOrDefault(s, "StageInfo", "zoffset", 0);
	gData.mStageInfo.mZOffsetLink = getMugenDefIntegerOrDefault(s, "StageInfo", "zoffsetlink", 0);
	gData.mStageInfo.mAutoturn = getMugenDefIntegerOrDefault(s, "StageInfo", "autoturn", 1);
	gData.mStageInfo.mResetBG = getMugenDefIntegerOrDefault(s, "StageInfo", "resetBG", 0);
	gData.mStageInfo.mLocalCoordinates = getMugenDefVectorIOrDefault(s, "StageInfo", "localcoord", makeVector3DI(320, 240, 0));

	gData.mStageInfo.mScale = makePosition(1, 1, 1);
	gData.mStageInfo.mScale.x = getMugenDefFloatOrDefault(s, "StageInfo", "xscale", 1);
	gData.mStageInfo.mScale.y = getMugenDefFloatOrDefault(s, "StageInfo", "yscale", 1);
}

static void loadStageShadow(MugenDefScript* s) {
	gData.mShadow.mIntensity = getMugenDefIntegerOrDefault(s, "Shadow", "intensity", 128);
	gData.mShadow.mColor = getMugenDefVectorOrDefault(s, "StageInfo", "color", makePosition(1, 1, 1));
	gData.mShadow.mScaleY = getMugenDefFloatOrDefault(s, "StageInfo", "yscale", 1);
	gData.mShadow.mFadeRange = getMugenDefVectorIOrDefault(s, "StageInfo", "fade.range", makeVector3DI(0, 0, 0));
	gData.mShadow.mXShear = getMugenDefFloatOrDefault(s, "StageInfo", "xshear", 0);
}

static void loadStageReflection(MugenDefScript* s) {
	gData.mReflection.mReflect = getMugenDefIntegerOrDefault(s, "Reflection", "reflect", 0);
}

static void loadStageMusic(MugenDefScript* s) {
	getMugenDefStringOrDefault(gData.mMusic.mBGMusic, s, "Music", "bgmusic", "");
	gData.mMusic.mBGVolume = getMugenDefIntegerOrDefault(s, "Music", "bgvolume", 0);
}

static MugenDefScriptGroup* loadStageBackgroundDefinitionAndReturnGroup(MugenDefScript* s) {
	MugenDefScriptGroup* bgdef;
	char name[100];
	if (string_map_contains(&s->mGroups, "BGDef")) {	
		strcpy(name, "BGDef");
	} else if(string_map_contains(&s->mGroups, "BGdef")) {
		strcpy(name, "BGdef");
	}
	else {
		strcpy(name, "");
		logError("No background definition found.");
		abortSystem();
	}

	bgdef = string_map_get(&s->mGroups, name);

	getMugenDefStringOrDefault(gData.mBackgroundDefinition.mSpritePath, s, name, "spr", "");
	gData.mBackgroundDefinition.mDebugBG = getMugenDefIntegerOrDefault(s, name, "debugbg", 0);

	return bgdef;
}

static int isBackgroundElementGroup(MugenDefScriptGroup* tGroup) {
	return tGroup->mName[0] == 'B' && tGroup->mName[1] == 'G';
}

static int isActionGroup(MugenDefScriptGroup* tGroup) {
	char firstW[100], secondW[100];
	int items = sscanf(tGroup->mName, "%s %s", firstW, secondW);
	if (items < 2) return 0;

	turnStringLowercase(firstW);
	turnStringLowercase(secondW);
	return !strcmp("begin", firstW) && !strcmp("action", secondW);
}

static void addBackgroundElementToStageHandler(StageBackgroundElement* e) {
	e->mStart.z = e->mListPosition + e->mLayerNo * 30; // TODO
	if (e->mType == STAGE_BACKGROUND_STATIC) {
		addDreamMugenStageHandlerStaticBackgroundElement(e->mStart, e->mSpriteNo.x, e->mSpriteNo.y, &gData.mSprites, e->mDelta, e->mTile, e->mTileSpacing, gData.mStageInfo.mLocalCoordinates);
	} else if (e->mType == STAGE_BACKGROUND_ANIMATED) {
		addDreamMugenStageHandlerAnimatedBackgroundElement(e->mStart, e->mActionNumber, &gData.mAnimations, &gData.mSprites, e->mDelta, e->mTile, e->mTileSpacing, gData.mStageInfo.mLocalCoordinates);
	}
	else {
		logError("Unable to determine bg element type");
		logErrorInteger(e->mType);
		abortSystem();
	}
}

static void loadBackgroundElement(MugenDefScript* s, char* tName, int i) {
	StageBackgroundElement* e = allocMemory(sizeof(StageBackgroundElement));

	debugLog("Load background element.");
 	debugString(tName);

	char type[100];
	getMugenDefStringOrDefault(type, s, tName, "type", "normal");
	if (!strcmp("normal", type) || !strcmp("parallax", type)) { // TODO: parallax
		e->mType = STAGE_BACKGROUND_STATIC;
	} else if (!strcmp("anim", type)) {
		e->mType = STAGE_BACKGROUND_ANIMATED;
		e->mActionNumber = getMugenDefIntegerOrDefault(s, tName, "actionno", -1);
	}
	else {
		logError("Unknown type.");
		logErrorString(type);
		abortSystem();
	}

	e->mSpriteNo = getMugenDefVectorIOrDefault(s, tName, "spriteno", makeVector3DI(0, 0, 0));
	e->mLayerNo = getMugenDefIntegerOrDefault(s, tName, "layerno", 0);
	e->mStart = getMugenDefVectorOrDefault(s, tName, "start", makePosition(0, 0, 0));
	e->mDelta = getMugenDefVectorOrDefault(s, tName, "delta", makePosition(1, 1, 1));
	e->mMask = getMugenDefIntegerOrDefault(s, tName, "mask", 0);
	e->mTile = getMugenDefVectorIOrDefault(s, tName, "tile", makeVector3DI(0, 0, 0));
	e->mTileSpacing = getMugenDefVectorIOrDefault(s, tName, "tilespacing", makeVector3DI(0, 0, 0));
	e->mListPosition = i;

	addBackgroundElementToStageHandler(e);

	list_push_back_owned(&gData.mBackgroundElements, e);
}

static void loadBackgroundDefinitionGroup(MugenDefScript* s, MugenDefScriptGroup* tGroup, int i) {
	if (isBackgroundElementGroup(tGroup)) {
		loadBackgroundElement(s, tGroup->mName, i);
	}
	else if (isActionGroup(tGroup)) {
		// TODO: ignore properly
	}
	else {
		logError("Unknown background definition group.");
		logErrorString(tGroup->mName);
		abortSystem();
	}
}

static void loadStageTextures(char* tPath) {
	char path[1024];
	getPathToFile(path, tPath);
	char sffFile[1024];
	sprintf(sffFile, "%s%s", path, gData.mBackgroundDefinition.mSpritePath);
	if (!isFile(sffFile)) {
		sprintf(sffFile, "assets/%s", gData.mBackgroundDefinition.mSpritePath);
		if (!isFile(sffFile)) {
			logError("Unable to locate sff file.");
			logErrorString(gData.mBackgroundDefinition.mSpritePath);
			abortSystem();
		}
	}
	gData.mSprites = loadMugenSpriteFileWithoutPalette(sffFile);
}

static void loadStageBackgroundElements(char* tPath, MugenDefScript* s) {
	MugenDefScriptGroup* bgdef = loadStageBackgroundDefinitionAndReturnGroup(s);

	loadStageTextures(tPath);

	gData.mBackgroundElements = new_list();
	int i = 0;
	MugenDefScriptGroup* cur = bgdef->mNext;
	while (cur != NULL) {
		loadBackgroundDefinitionGroup(s, cur, i);
		i++;
		cur = cur->mNext;
	}

}

static void setStageCamera() {
	double sizeX = gData.mCamera.mBoundRight - gData.mCamera.mBoundLeft;
	double sizeY = gData.mCamera.mBoundLow - gData.mCamera.mBoundHigh;
	setDreamMugenStageHandlerCameraRange(makeGeoRectangle(gData.mCamera.mBoundLeft, gData.mCamera.mBoundHigh, sizeX, sizeY));
}

static void loadStage(void* tData)
{
	(void)tData;
	instantiateActor(DreamMugenStageHandler);

	gData.mAnimations = loadMugenAnimationFile(gData.mDefinitionPath);

	MugenDefScript s = loadMugenDefScript(gData.mDefinitionPath);

	loadStageInfo(&s);
	loadStageCamera(&s);
	loadStagePlayerInfo(&s);
	loadStageBound(&s);
	loadStageStageInfo(&s);
	loadStageShadow(&s);
	loadStageReflection(&s);
	loadStageMusic(&s);
	setDreamMugenStageHandlerCameraCoordinates(makeVector3DI(320, 240, 0)); // TODO

	setStageCamera();
	loadStageBackgroundElements(gData.mDefinitionPath, &s);

	unloadMugenDefScript(s);
}

static void updateCameraMovement() {
	double x1 = getPlayerPositionX(getRootPlayer(0), gData.mStageInfo.mLocalCoordinates.y);
	double x2 = getPlayerPositionX(getRootPlayer(1), gData.mStageInfo.mLocalCoordinates.y);
	double minX = min(x1, x2);
	double maxX = max(x1, x2);
	minX -= gData.mStageInfo.mLocalCoordinates.x / 2;
	maxX -= gData.mStageInfo.mLocalCoordinates.x / 2;
	minX -= getDreamCameraPositionX(gData.mStageInfo.mLocalCoordinates.y);
	maxX -= getDreamCameraPositionX(gData.mStageInfo.mLocalCoordinates.y);

	double right = getDreamCameraPositionX(gData.mStageInfo.mLocalCoordinates.y) + gData.mStageInfo.mLocalCoordinates.x / 2;
	double left = getDreamCameraPositionX(gData.mStageInfo.mLocalCoordinates.y) - gData.mStageInfo.mLocalCoordinates.x / 2;
	
	double lx = (left + gData.mCamera.mTension) - minX;
	double rx = maxX - (right - gData.mCamera.mTension);

	if (lx > 0 && rx > 0) {
		// TODO
	}
	else if (rx > 0) {
		double delta = min(rx, -lx);
		addDreamMugenStageHandlerCameraPositionX(delta);
	}
	else if (lx > 0) {
		double delta = min(lx, -rx);
		addDreamMugenStageHandlerCameraPositionX(-delta);
	}
}

static void updateStage(void* tData) {
	(void)tData;

	updateCameraMovement();
}

ActorBlueprint DreamStageBP = {
	.mLoad = loadStage,
	.mUpdate = updateStage,
};


void setDreamStageMugenDefinition(char * tPath)
{
	strcpy(gData.mDefinitionPath, tPath);
}

double parseDreamCoordinatesToLocalCoordinateSystem(double tCoordinate, int tOtherCoordinateSystemAsP)
{
	ScreenSize sz = getScreenSize();
	int currentP = sz.y; 
	double fac = currentP / (double)tOtherCoordinateSystemAsP;

	return tCoordinate*fac;
}

Position getDreamPlayerStartingPosition(int i, int tCoordinateP)
{
	Position ret;
	if (!i) ret = gData.mPlayerInfo.mP1Start;
	else ret = gData.mPlayerInfo.mP2Start;

	ret = vecAdd(ret, makePosition(gData.mStageInfo.mLocalCoordinates.x / 2,0,0));

	return vecScale(ret, tCoordinateP / (double)gData.mStageInfo.mLocalCoordinates.y);
}

Position getDreamStageCoordinateSystemOffset(int tCoordinateP)
{
	Position ret = makePosition(0, gData.mStageInfo.mZOffset, 0);
	return vecScale(ret, tCoordinateP / (double)gData.mStageInfo.mLocalCoordinates.y);
}

int doesDreamPlayerStartFacingLeft(int i)
{
	if (!i) return gData.mPlayerInfo.mP1Facing == -1;
	else return gData.mPlayerInfo.mP2Facing == -1;
}

double getDreamCameraPositionX(int tCoordinateP)
{
	Position p = *getDreamMugenStageHandlerCameraPositionReference();
	ScreenSize sz = getScreenSize();
	p = transformDreamCoordinatesVector(p, sz.y, tCoordinateP);

	return p.x;
}

double getDreamCameraPositionY(int tCoordinateP)
{
	Position p = *getDreamMugenStageHandlerCameraPositionReference();
	ScreenSize sz = getScreenSize();
	p = transformDreamCoordinatesVector(p, sz.y, tCoordinateP);

	return p.y;
}

double getDreamScreenFactorFromCoordinateP(int tCoordinateP)
{
	ScreenSize sz = getScreenSize();
	return sz.y / (double)tCoordinateP;
}

int getDreamStageCoordinateP()
{
	return gData.mStageInfo.mLocalCoordinates.y;
}

double transformDreamCoordinates(double tVal, int tSrcP, int tDstP)
{
	return tVal * (tDstP / (double) tSrcP);
}

Vector3D transformDreamCoordinatesVector(Vector3D tVal, int tSrcP, int tDstP)
{
	return vecScale(tVal, (tDstP / (double)tSrcP));
}

double getDreamStageTopOfScreenBasedOnPlayer(int tCoordinateP)
{
	return getDreamCameraPositionY(tCoordinateP);
}

double getDreamStageLeftOfScreenBasedOnPlayer(int tCoordinateP)
{
	Position p = *getDreamMugenStageHandlerCameraPositionReference();
	ScreenSize sz = getScreenSize();
	p = transformDreamCoordinatesVector(p, sz.y, tCoordinateP);

	return p.x;
}

double getDreamStageRightOfScreenBasedOnPlayer(int tCoordinateP)
{
	Position p = *getDreamMugenStageHandlerCameraPositionReference();
	ScreenSize sz = getScreenSize();
	p = transformDreamCoordinatesVector(p, sz.y, tCoordinateP);
	double screenSize = transformDreamCoordinates(sz.x, sz.y, tCoordinateP);

	return p.x + screenSize;
}

Position getDreamStageCenterOfScreenBasedOnPlayer(int tCoordinateP)
{
	Position ret = *getDreamMugenStageHandlerCameraPositionReference();

	ret = vecAdd(ret, makePosition(gData.mStageInfo.mLocalCoordinates.x / 2, 0, 0));

	return vecScale(ret, tCoordinateP / (double)gData.mStageInfo.mLocalCoordinates.y);
}

int getDreamGameWidth(int tCoordinateP)
{
	return (int)transformDreamCoordinates(640, 480, tCoordinateP); // TODO: non-hardcoded
}

int getDreamStageLeftEdgeMinimumPlayerDistance(int tCoordinateP)
{
	return (int)transformDreamCoordinates(gData.mBound.mScreenLeft, getDreamStageCoordinateP(), tCoordinateP);
}

int getDreamStageRightEdgeMinimumPlayerDistance(int tCoordinateP)
{
	return (int)transformDreamCoordinates(gData.mBound.mScreenRight, getDreamStageCoordinateP(), tCoordinateP);
}
