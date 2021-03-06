#pragma once

#include <prism/actorhandler.h>

#include "fight_mugenstatereader.h"
#include "fight_playerdefinition.h"

extern ActorBlueprint DreamMugenStateHandler;

int registerDreamMugenStateMachine(DreamMugenStates* tStates, DreamPlayer* tPlayer);
void removeDreamRegisteredStateMachine(int tID);
int getDreamRegisteredStateState(int tID);
int getDreamRegisteredStatePreviousState(int tID);
void pauseDreamRegisteredStateMachine(int tID);
void unpauseDreamRegisteredStateMachine(int tID);
void disableDreamRegisteredStateMachine(int tID);

int getDreamRegisteredStateTimeInState(int tID);
void setDreamRegisteredStateTimeInState(int tID, int tTime);
void setDreamRegisteredStateToHelperMode(int tID);
void setDreamRegisteredStateDisableCommandState(int tID);

int hasDreamHandledStateMachineState(int tID, int tNewState);
int hasDreamHandledStateMachineStateSelf(int tID, int tNewState);
void changeDreamHandledStateMachineState(int tID, int tNewState);
void changeDreamHandledStateMachineStateToOtherPlayerStateMachine(int tID, int tTemporaryID, int tNewState);
void changeDreamHandledStateMachineStateToOwnStateMachine(int tID, int tNewState);

void updateDreamSingleStateMachineByID(int tID);

