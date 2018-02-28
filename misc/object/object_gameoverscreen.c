#include "object_gameoverscreen.h"

#include "object_storyscreen.h"

static void updateGameOverScreen() {
	
	setCurrentObjectStoryDefinitionFile("assets/misc/object/story/GAMEOVER.def");
	setNewScreen(&ObjectStoryScreen);
}

Screen ObjectGameOverScreen = {
	.mUpdate = updateGameOverScreen,
};