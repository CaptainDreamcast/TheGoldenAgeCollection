#include "justice_sound.h"

#include <prism/soundeffect.h>

static struct {
	int punchSFX;

} gData;

void loadJusticeSound() {
	gData.punchSFX = loadSoundEffect("/assets/main/justice/sounds/PUNCH.wav");

}
void playJusticeHitSoundEffect() {
	playSoundEffect(gData.punchSFX);
}

