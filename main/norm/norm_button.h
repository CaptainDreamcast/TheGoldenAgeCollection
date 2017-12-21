#pragma once

void loadNormButtons();

int addNormButton();
void removeNormButton(int tID);

int hasPressedNormButton(int tID);
int isNormButtonBelowRange(int tID);

void setNormButtonSpeed(double x);