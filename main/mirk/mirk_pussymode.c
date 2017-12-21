#include "mirk_pussymode.h"

static struct {
	int mActive;
} gData;

int isInMirkPussyMode()
{
	return gData.mActive;
}

void setMirkPussyModeOn()
{
	gData.mActive = 1;
}

void setMirkPussyModeOff()
{
	gData.mActive = 0;
}
