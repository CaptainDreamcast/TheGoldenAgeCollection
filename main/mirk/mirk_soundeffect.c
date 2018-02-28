#include "mirk_soundeffect.h"

#include <prism/file.h>
#include <prism/memoryhandler.h>

static struct {
	SoundEffectCollection mTextSoundEffects;

} gData;

void initMirkGameSoundEffects()
{
	gData.mTextSoundEffects = loadConsecutiveSoundEffectsToCollection("assets/main/mirk/sfx/text/NORMAL.wav", 5);
}

SoundEffectCollection getMirkTextSoundEffectCollection()
{
	return gData.mTextSoundEffects;
}
