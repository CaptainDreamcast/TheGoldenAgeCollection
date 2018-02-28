#include "justice_system.h"

#include <prism/file.h>

static struct {
	char fs[100];
	
} gData;

void setJusticeGameBaseFileSystem(char* fs) {
	strcpy(gData.fs, fs);
}

void resetToJusticeGameBaseFileSystem() {
	setFileSystem(gData.fs);
}

