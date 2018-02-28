#pragma once

#include <prism/geometry.h>
#include <prism/drawing.h>

void loadMirkParticles();
void addMirkBloodParticles(Position p, Color c);
void updateMirkParticles();

void invertMirkParticleReality();
void setMirkParticlesReal();
void setMirkParticlesUnreal();