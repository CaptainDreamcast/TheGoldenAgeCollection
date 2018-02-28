#include "beyond_bg.h"

#include <prism/mugenanimationhandler.h>
#include <prism/mugendefreader.h>
#include <prism/mugenscriptparser.h>
#include <prism/animation.h>
#include <prism/wrapper.h>

typedef struct {
	Position mPosition;
	Position mOffset;
	MugenAnimation* mAnimation;
	int mAnimationID;

	double mRotation;
	double mRotationAdd;

	double mScale;
	double mScaleAdd;

} BackgroundElement;

static struct {
	MugenSpriteFile* mSprites;
	MugenAnimations* mAnimations;

	double mSpeed;
	double mSize;

	TextureData mWhiteTexture;
	int mBlackAnimationID;

	Position mBasePosition;
	Vector mElements;
} gData;

static void loadBackgroundHandler(void* tData) {
	(void)tData;
	// TODO
	gData.mElements = new_vector();

	gData.mWhiteTexture = loadTexture("$/rd/effects/white.pkg");
	gData.mBlackAnimationID = playOneFrameAnimationLoop(makePosition(0, 0, 6), &gData.mWhiteTexture);
	setAnimationSize(gData.mBlackAnimationID, makePosition(640, 480, 1), makePosition(0, 0, 0));
	setAnimationColor(gData.mBlackAnimationID, 0, 0, 0);
}

static int isHeader(MugenDefScriptGroup* tGroup) {
	return !strcmp("Header", tGroup->mName);
}

static void handleHeader(MugenDefScriptGroup* tGroup) {
	(void)tGroup;
	gData.mSize = getMugenDefFloatOrDefaultAsGroup(tGroup, "size", 640*3);
	gData.mSpeed = getMugenDefFloatOrDefaultAsGroup(tGroup, "speed", 7);
	
	double opacity = getMugenDefFloatOrDefaultAsGroup(tGroup, "bgopacity", 0.2);
	setAnimationTransparency(gData.mBlackAnimationID, opacity);
}

static int isBackgroundElement(MugenDefScriptGroup* tGroup) {
	return !strcmp("BG Element", tGroup->mName);
}

static void handleBackgroundElement(MugenDefScriptGroup* tGroup) {
	BackgroundElement* e = allocMemory(sizeof(BackgroundElement));
	e->mOffset = getMugenDefVectorOrDefaultAsGroup(tGroup, "offset", makePosition(0, 0, 0));
	e->mPosition = e->mOffset;


	if (isMugenDefNumberVariableAsGroup(tGroup, "animation")) {
		int animationID = getMugenDefNumberVariableAsGroup(tGroup, "animation");
		e->mAnimation = getMugenAnimation(gData.mAnimations, animationID);
	}
	else {
		Vector3DI sprite = getMugenDefVectorIVariableAsGroup(tGroup, "sprite");
		e->mAnimation = createOneFrameMugenAnimationForSprite(sprite.x, sprite.y);
	}
	
	e->mRotation = 0;
	e->mRotationAdd = getMugenDefFloatOrDefaultAsGroup(tGroup, "rotationspeed", 0);
	
	e->mAnimationID = addMugenAnimation(e->mAnimation, gData.mSprites, makePosition(0, 0, 5));
	setMugenAnimationBasePosition(e->mAnimationID, &e->mPosition);
	
	e->mScale = getMugenDefFloatOrDefaultAsGroup(tGroup, "scale", 1);
	setMugenAnimationDrawScale(e->mAnimationID, makePosition(e->mScale, e->mScale, 1));

	e->mScaleAdd = getMugenDefFloatOrDefaultAsGroup(tGroup, "scaledelta", 0);

	vector_push_back_owned(&gData.mElements, e);
}

void setBeyondBackground(char* tPath, MugenSpriteFile* tSprites, MugenAnimations* tOptionalAnimations) {
	MugenDefScript script = loadMugenDefScript(tPath);
	gData.mSprites = tSprites;
	gData.mAnimations = tOptionalAnimations;
	gData.mBasePosition = makePosition(0, 0, 2);

	resetMugenScriptParser();
	addMugenScriptParseFunction(isHeader, handleHeader);
	addMugenScriptParseFunction(isBackgroundElement, handleBackgroundElement);
	parseMugenScript(&script);

	unloadMugenDefScript(script);
}

double getBeyondFinalFrontierLevelBackgroundScale()
{
	BackgroundElement* e = vector_get(&gData.mElements, 0);
	return e->mScale;
}

static void updatePosition() {

	gData.mBasePosition.x += gData.mSpeed;
	if (gData.mBasePosition.x >= gData.mSize) {
		gData.mBasePosition.x -= gData.mSize;
	}
}

static void updateSingleBackgroundElement(void* tCaller, void* tData) {
	(void)tCaller;

	BackgroundElement* e = tData;
	e->mPosition = vecSub(e->mOffset, gData.mBasePosition);
	e->mRotation += e->mRotationAdd;
	setMugenAnimationDrawAngle(e->mAnimationID, e->mRotation);

	e->mScale += e->mScaleAdd;
	setMugenAnimationDrawScale(e->mAnimationID, makePosition(e->mScale, e->mScale, 1));
}

static void updateBackgroundHandler(void* tData) {
	(void)tData;
	if (isWrapperPaused()) return;

	updatePosition();
	vector_map(&gData.mElements, updateSingleBackgroundElement, NULL);
}

ActorBlueprint BeyondBackgroundHandler = {
	.mLoad = loadBackgroundHandler,
	.mUpdate = updateBackgroundHandler,
};
