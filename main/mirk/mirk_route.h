#pragma once


typedef struct {
	int(*mCanBePlayed)();
	void(*mReset)();
	int(*mHasLost)();
	void(*mLoadRoute)();
	void(*mUpdate)();
	void(*mSetToContinue)();
} MirkRoute;

