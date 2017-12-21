#ifndef ENEMIES_H
#define ENEMIES_H

#include <tari/geometry.h>

void loadJusticeEnemies();
void updateJusticeEnemies();
void freezeJusticeEnemies();
void unfreezeJusticeEnemies();

int getActiveJusticeEnemyAmount();
void spawnJusticeEnemy(int type, Position pos);
void setJusticeEnemiesScreenPositionReference(Position* p);


#endif
