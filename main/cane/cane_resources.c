#include "cane_resources.h"

#include <tari/memoryhandler.h>
#include <tari/physicshandler.h>

static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;

} gData;

void loadCaneResources()
{
	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/main/cane/CANE.sff");
	gData.mAnimations = loadMugenAnimationFile("assets/main/cane/CANE.air");

}

void shutdownCaneResources()
{
	shutdownPhysicsHandler();

}

MugenSpriteFile * getCaneSprites()
{
	return &gData.mSprites;
}

MugenAnimations * getCaneAnimations()
{
	return &gData.mAnimations;
}
