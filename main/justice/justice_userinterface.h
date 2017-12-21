#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <tari/geometry.h>

void loadJusticeUserInterface();
void updateJusticeUserInterface();
void resumeJusticeGame();

int addJusticeShadow(Position* pos, Position center, double scaleX);
void removeJusticeShadow(int id);
void setJusticeUserInterfaceScreenPositionReference(Position* pos);
void setJusticeHealthBarPercentage(double t);
int isJusticeGamePaused();

#endif
