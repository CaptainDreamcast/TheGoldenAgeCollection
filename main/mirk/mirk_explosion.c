#include "mirk_explosion.h"

#include <tari/animation.h>
#include <tari/stagehandler.h>
#include <tari/soundeffect.h>

#include "mirk_stage.h"
#include "mirk_pussymode.h"

static struct {
	int mBoomSFX;

	Animation mExplosionAnimation;
	TextureData mExplosion[20];
} gData;

void loadMirkExplosions()
{
	gData.mExplosionAnimation = createEmptyAnimation();
	gData.mExplosionAnimation.mFrameAmount = 17;
	gData.mExplosionAnimation.mDuration = 3;
	loadConsecutiveTextures(gData.mExplosion, "assets/main/mirk/effects/EXPLOSION.pkg", gData.mExplosionAnimation.mFrameAmount);
	gData.mBoomSFX = loadSoundEffect("assets/main/mirk/sfx/boom.wav");
}

void addMirkExplosion(Position p, double r) {
	playSoundEffect(gData.mBoomSFX);

	if (!isInMirkPussyMode()) {
		addStageHandlerScreenShake(50);
	}

	double fac = 2.5;
	double cfac = fac / 2;

	p = vecSub(p, makePosition(cfac*r, cfac*r, 0));

	int anim = playAnimation(p, gData.mExplosion, gData.mExplosionAnimation, makeRectangleFromTexture(gData.mExplosion[0]), NULL, NULL);
	setAnimationScreenPositionReference(anim, getMirkStagePositionReference());
	setAnimationSize(anim, makePosition(fac*r, fac*r, 1), makePosition(0, 0, 0));
}
