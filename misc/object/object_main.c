#include "object_main.h"

#include "object_storyscreen.h"

void startObjectification()
{
	setCurrentObjectStoryDefinitionFile("assets/misc/object/story/PROLOGUE.def");
	setNewScreen(&ObjectStoryScreen);
}
