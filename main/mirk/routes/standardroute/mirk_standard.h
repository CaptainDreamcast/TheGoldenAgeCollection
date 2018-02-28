#pragma once 

#include <prism/geometry.h>

void loadMirkStandard();
void updateMirkStandard();

int hasMirkLostStandard();
int hasMirkWonStandard();

void setMirkStandardWaveText(char* tText);
void setMirkStandardFunnyText(char* tText);
void setMirkStandardLevelMirklingAmount(int tMirklings);
void setMirkStandardFunnyTextPositionAfterLoad(Position tPos);
void setMirkStandardStoppedShowingWaveScreenCB(void(*tCB)(void*), void* tCaller);
void setMirkStandardKeepPlayerShotPaused();

int getMirkStandardGeneratedMirklingAmount();

void setMirkGameUnreal();
void setMirkGameReal();