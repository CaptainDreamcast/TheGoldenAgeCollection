#pragma once

#include <prism/geometry.h>

void addMirkMirkling(double tSpeed);
void initMirkMirklings();
void updateMirkMirklings();

int addMirkMirklingManual(Position tPos, Vector3D tDir, double tSpeed);
void setMirkMirklingRouteHitCB(int tID, void(*tCB)(void* tCaller, void* tCollisionData), void* tCaller);
void setMirkMirklingSpawnY(double tY);
void setMirkMirklingsInvisible();
void setMirkMirklingsVisible();
void invertMirkMirklingsReality();
void setMirkMirklingsReal();
void setMirkMirklingsUnreal();
void addMirkMirklingDetraction(Position p, double tPower);


Position getLowestMirkMirklingPosition();