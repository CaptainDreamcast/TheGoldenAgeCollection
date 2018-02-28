#include "hack_projectlogic.h"

static struct {
	int mCodeValue;
	int mDesignValue;
	int mMarketingValue;
	int mTime;
} gData;

void resetHackProject()
{
	gData.mCodeValue = 1;
	gData.mDesignValue = 1;
	gData.mMarketingValue = 1;
	gData.mTime = 0;
}

int getHackCodeProjectValue()
{
	return gData.mCodeValue;
}

int getHackDesignProjectValue()
{
	return gData.mDesignValue;
}

int getHackMarketingProjectValue()
{
	return gData.mMarketingValue;
}

int getHackTime()
{
	return gData.mTime;
}

void changeHackProjectCode(int tDelta)
{
	gData.mCodeValue += tDelta;
}

void changeHackProjectDesign(int tDelta)
{
	gData.mDesignValue += tDelta;
}

void changeHackProjectMarketing(int tDelta)
{
	gData.mMarketingValue += tDelta;
}

void changeHackProjectTime(int tDelta)
{
	gData.mTime += tDelta;
}

int isHackTimeOver()
{
	return gData.mTime > 9;
}
