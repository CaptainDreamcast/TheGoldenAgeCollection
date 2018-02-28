#pragma once

#include <prism/mugendefreader.h>

#include "fight_mugenassignment.h"
#include "fight_playerdefinition.h"

DreamMugenStateController* parseDreamMugenStateControllerFromGroup(MugenDefScriptGroup* tGroup);
int handleDreamMugenStateControllerAndReturnWhetherStateChanged(DreamMugenStateController* tController, DreamPlayer* tPlayer);