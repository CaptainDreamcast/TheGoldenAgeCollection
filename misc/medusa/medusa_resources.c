#include "medusa_resources.h"

static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;

} gData;

void loadMedusaResources()
{
	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/misc/medusa/MEDUSA.sff");
	gData.mAnimations = loadMugenAnimationFile("assets/misc/medusa/MEDUSA.air");
}

MugenSpriteFile * getMedusaSprites()
{
	return &gData.mSprites;
}

MugenAnimations * getMedusaAnimations()
{
	return &gData.mAnimations;
}
