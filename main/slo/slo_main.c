#include "slo_main.h"

#include <stdio.h>

#include "slo_storyscreen.h"
#include "slo_selectscreen.h"

static struct {
	int mStage;

} gData;

void startSloMoDo()
{

	if (gData.mStage <= 2) {
		char path[1024];
		sprintf(path, "assets/main/slo/story/%d.def", gData.mStage);
		setCurrentSloStoryDefinitionFile(path);
		setNewScreen(&SloStoryScreen);
	}
	else {
		setNewScreen(&SloSelectScreen);
	}
}

void increaseSloStage()
{
	gData.mStage++;
}
