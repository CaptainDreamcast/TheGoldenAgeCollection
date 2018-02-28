#include <prism/framerateselectscreen.h>
#include <prism/pvr.h>
#include <prism/physics.h>
#include <prism/file.h>
#include <prism/drawing.h>
#include <prism/log.h>
#include <prism/wrapper.h>
#include <prism/system.h>
#include <prism/stagehandler.h>
#include <prism/logoscreen.h>
#include <prism/collisionhandler.h>

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


