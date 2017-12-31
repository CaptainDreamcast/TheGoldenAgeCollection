#include "justice_main.h"

#include <tari/memoryhandler.h>

#include "../../maingamemenu.h"

#include "justice_titlescreen.h"
#include "justice_gamestate.h"

void startFistsOfJustice()
{
	resetJusticeGameState();
	setMemoryHandlerCompressionActive();
	setNewScreen(&JusticeTitleScreen);
}

void stopFistsOfJustice() {
	setMemoryHandlerCompressionInactive();
	setNewScreen(&MainGameMenu);
}
