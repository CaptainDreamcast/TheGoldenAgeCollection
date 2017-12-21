#pragma once

#include <tari/geometry.h>

void setupCaneBulletHandling();
void shutdownCaneBulletHandling();
void updateCaneBulletHandling();

void addCaneBullet(Position pos, double angle);
void reloadCaneBullet();
int getCaneBulletAmount();