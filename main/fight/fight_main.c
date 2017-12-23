#include "fight_main.h"

#include <tari/memoryhandler.h>

#include "../../maingamemenu.h"

#include "fight_warningscreen.h"

void startDreamFight16()
{
	setMemoryHandlerCompressionActive();
	setNewScreen(&DreamWarningScreen);
}

void stopDreamFight16 () {
	setMemoryHandlerCompressionInactive();
	setNewScreen(&MainGameMenu);
}
