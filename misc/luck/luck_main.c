#include <tari/framerateselectscreen.h>
#include <tari/pvr.h>
#include <tari/physics.h>
#include <tari/file.h>
#include <tari/drawing.h>
#include <tari/log.h>
#include <tari/wrapper.h>
#include <tari/system.h>
#include <tari/stagehandler.h>
#include <tari/logoscreen.h>
#include <tari/collisionhandler.h>

#include "luck_titlescreen.h"
#include "luck_screen1.h"
#include "luck_screen3.h"
#include "luck_screen4.h"
#include "luck_screen5.h"
#include "luck_screen6.h"
#include "luck_screen7.h"
#include "luck_screen8.h"
#include "luck_congratsscreen.h"


void startHankInLuck() {
	setNewScreen(&LuckTitleScreen);
}


