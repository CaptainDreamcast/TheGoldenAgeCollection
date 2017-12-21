#pragma once

#include <tari/geometry.h>
#include <tari/drawing.h>

void loadMirkParticles();
void addMirkBloodParticles(Position p, Color c);
void updateMirkParticles();

void invertMirkParticleReality();
void setMirkParticlesReal();
void setMirkParticlesUnreal();