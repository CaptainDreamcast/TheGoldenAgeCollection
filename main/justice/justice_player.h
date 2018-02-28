#ifndef PLAYER_H
#define PLAYER_H

#include <prism/geometry.h>

void loadJusticePlayer();
void addJusticePlayerShadow();
void updateJusticePlayer();
void freezeJusticePlayer();
void unfreezeJusticePlayer();

Position getJusticePlayerPosition();
void setJusticePlayerScreenPositionReference(Position* p);
int getJusticePlayerHealth();

#endif
