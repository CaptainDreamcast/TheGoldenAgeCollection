#include "object_congratsscreen.h"

#include "object_storyscreen.h"

static void updateGameOverScreen() {

	setCurrentObjectStoryDefinitionFile("assets/misc/object/story/CONGRATS.def");
	setNewScreen(&ObjectStoryScreen);
}

Screen ObjectCongratsScreen = {
	.mUpdate = updateGameOverScreen,
};