#include "hack_hackscreen.h"

#include <prism/mugenanimationhandler.h>
#include <prism/input.h>
#include <prism/screeneffect.h>
#include <prism/optionhandler.h>
#include <prism/math.h>
#include <prism/sound.h>

#include "hack_selectscreen.h"
#include "hack_performancehandler.h"
#include "hack_projectlogic.h"
#include "hack_storyscreen.h"

static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;
} gData;

static void loadScreenOptions();

static void loadTitleScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());
	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/misc/hack/sprites/HACK.sff");
	gData.mAnimations = loadMugenAnimationFile("assets/misc/hack/sprites/HACK.air");
	addMugenAnimation(createOneFrameMugenAnimationForSprite(0, 0), &gData.mSprites, makePosition(0, 0, 1));
	addMugenAnimation(getMugenAnimation(&gData.mAnimations, 30+getHackTime()), &gData.mSprites, makePosition(444, 54, 2));

	loadScreenOptions();

	addFadeIn(30, NULL, NULL);

	playTrack(24);
}

static void gotoCode(void* tCaller) {
	(void)tCaller;
	char path[1000];
	sprintf(path, "assets/misc/hack/story/code/%d.def", randfromInteger(0, 2));

	setCurrentHackStoryDefinitionFile(path);
	setNewScreen(&HackStoryScreen);
}

static void selectCode(void* tCaller) {
	addFadeOut(30, gotoCode, NULL);
}

static void gotoDesign(void* tCaller) {
	(void)tCaller;
	char path[1000];
	sprintf(path, "assets/misc/hack/story/design/%d.def", randfromInteger(0, 2));

	setCurrentHackStoryDefinitionFile(path);
	setNewScreen(&HackStoryScreen);
}

static void selectDesign(void* tCaller) {
	addFadeOut(30, gotoDesign, NULL);
}

static void gotoMarketing(void* tCaller) {
	(void)tCaller;
	char path[1000];
	sprintf(path, "assets/misc/hack/story/marketing/%d.def", randfromInteger(0, 2));

	setCurrentHackStoryDefinitionFile(path);
	setNewScreen(&HackStoryScreen);
}

static void selectMarketing(void* tCaller) {
	addFadeOut(30, gotoMarketing, NULL);
}

static void gotoHarrassment(void* tCaller) {
	(void)tCaller;
	char path[1000];
	sprintf(path, "assets/misc/hack/story/GAMEOVER.def");

	setCurrentHackStoryDefinitionFile(path);
	setNewScreen(&HackStoryScreen);
}

static void selectHarrassment(void* tCaller) {
	addFadeOut(30, gotoHarrassment, NULL);
}


static void loadScreenOptions() {
	instantiateActor(getOptionHandlerBlueprint());
	setOptionButtonA();
	setOptionButtonStart();
	setOptionTextSize(15);
	double x = 190;
	double y = 283;
	double dy = 26;
	addOption(makePosition(x, y, 3), "", selectCode, NULL);
	addOption(makePosition(x, y + dy, 3), "", selectDesign, NULL);
	addOption(makePosition(x, y + dy * 2, 3), "", selectMarketing, NULL);
	addOption(makePosition(x, y + dy * 3, 3), "", selectHarrassment, NULL);

	addHackPerformance(makePosition(105, 93, 3), makeVector3DI(getHackCodeProjectValue(), getHackDesignProjectValue(), getHackMarketingProjectValue()), &gData.mSprites, &gData.mAnimations);
}

static void gotoSelectScreen(void* tCaller) {

	setNewScreen(&HackSelectScreen);
}

static void updateTitleScreen() {
	if (hasPressedAbortFlank()) {
		abortScreenHandling();
	}
}

Screen HackHackScreen = {
	.mLoad = loadTitleScreen,
	.mUpdate = updateTitleScreen,
};