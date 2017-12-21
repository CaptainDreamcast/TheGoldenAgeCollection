#pragma once

#include <tari/actorhandler.h>

#include "fight_mugencommandreader.h"
#include "fight_mugenanimationhandler.h"
#include "fight_playerdefinition.h"

int registerDreamMugenCommands(DreamPlayer* tPlayer, DreamMugenCommands* tCommands);

int isDreamCommandActive(int tID, char* tCommandName);
void setDreamPlayerCommandActiveForAI(int tID, char* tCommandName, Duration tBufferTime);

void setDreamMugenCommandFaceDirection(int tID, FaceDirection tDirection);

extern ActorBlueprint DreamMugenCommandHandler;