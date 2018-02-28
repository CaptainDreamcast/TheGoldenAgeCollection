#include "beyond_effecthandler.h"

#include <prism/mugenanimationhandler.h>

#include "beyond_main.h"

static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;

} gData;

static void loadEffectHandler(void* tData) {
	(void)tData;
	char path[1024];
	sprintf(path, "assets/main/%s/effects/EFFECTS.sff", getBeyondDirectory());
	gData.mSprites = loadMugenSpriteFileWithoutPalette(path);
	sprintf(path, "assets/main/%s/effects/EFFECTS.air", getBeyondDirectory());
	gData.mAnimations = loadMugenAnimationFile(path);
}

ActorBlueprint BeyondEffectHandler = {
	.mLoad = loadEffectHandler,
};

void addBeyondExplosionEffect(Position tPosition)
{
	tPosition = vecAdd(tPosition, makePosition(0, 0, 20));
	MugenAnimation* animation = getMugenAnimation(&gData.mAnimations, 2);
	int id = addMugenAnimation(animation, &gData.mSprites, tPosition);
	setMugenAnimationNoLoop(id);
}
