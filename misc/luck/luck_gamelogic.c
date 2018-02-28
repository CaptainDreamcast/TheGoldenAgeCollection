#include "luck_gamelogic.h"

static struct {
	Screen* tCurrentScreen;
	Screen* tNextScreen;

} gData;

void setLuckCurrentScreen(Screen * tScreen)
{
	gData.tCurrentScreen = tScreen;
}

void setLuckNextScreen(Screen * tScreen)
{
	gData.tNextScreen = tScreen;
}

void moveLuckToNextScreen()
{
	setNewScreen(gData.tNextScreen);
}

void moveLuckToCurrentScreen()
{
	setNewScreen(gData.tCurrentScreen);
}
