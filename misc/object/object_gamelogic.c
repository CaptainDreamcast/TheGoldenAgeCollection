#include "object_gamelogic.h"

#include "object_clockhandler.h"
#include "object_girlfriendhandler.h"
#include "object_congratsgirlfriendscreen.h"


void startObjectificationGame()
{
	initObjectGirlfriends();
	resetObjectClock();

	setObjectCongratsGirlfriendID(getNewObjectGirlID());
	setNewScreen(&ObjectCongratsGirlfriendScreen);
}
