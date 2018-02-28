#pragma once

#include <prism/actorhandler.h>

typedef struct {
	void(*mLoadLevel)();
	void(*mUnloadLevel)();
	void(*mUpdateLevel)();
	int(*mHasLost)();
	int(*mHasWon)();
} MirkLevel;