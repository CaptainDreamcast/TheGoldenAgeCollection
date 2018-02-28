#include "octo_assethandler.h"

static struct {
	MugenAnimations mAnimations;
	MugenSpriteFile mSprites;
} gData;

void setupOctoAssets()
{
	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/misc/octo/SPRITES.sff");
	gData.mAnimations = loadMugenAnimationFile("assets/misc/octo/ANIMATIONS.air");
}

MugenAnimations * getOctoAnimations()
{
	return &gData.mAnimations;
}

MugenSpriteFile * getOctoSprites()
{
	return &gData.mSprites;
}
