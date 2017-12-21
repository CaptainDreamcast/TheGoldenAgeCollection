#include "beyond_main.h"

#include <stdio.h>

#include "beyond_titlescreen.h"

static struct {
	int mIsUltimateFrontier;
	char mDirectory[100];
} gData;

static void setUltimateFrontier() {
	sprintf(gData.mDirectory, "frontier");
	gData.mIsUltimateFrontier = 1;
}

static void setBeyond() {
	sprintf(gData.mDirectory, "beyond");
	gData.mIsUltimateFrontier = 0;
}

char * getBeyondDirectory()
{
	return gData.mDirectory;
}

void startEyeOfTheMedusa2()
{
	setUltimateFrontier();
	setNewScreen(&BeyondTitleScreen);
}


void startEyeOfTheMedusa3()
{
	setBeyond();
	setNewScreen(&BeyondTitleScreen);
}

int isUltimateFrontier()
{
	return gData.mIsUltimateFrontier;
}
