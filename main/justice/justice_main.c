#include "justice_main.h"

#include "justice_titlescreen.h"
#include "justice_gamestate.h"

void startFistsOfJustice()
{
	resetJusticeGameState();
	setNewScreen(&JusticeTitleScreen);
}
