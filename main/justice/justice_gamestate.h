#ifndef GAMESTATE_H
#define GAMESTATE_H

void resetJusticeGameState();
void resetJusticeLevelState();
int hasClearedJusticeLevel();
void setJusticeLevelCleared(int remainingHealth);
int hasJusticeLost();
void setJusticeLost();
int hasClearedJusticeGame();
void setJusticeGameCleared();
int getRemainingJusticeHealth();
void resetJusticeHealth();

void setCurrentJusticeLevelName(char* name);
char* getCurrentJusticeLevelName();

#endif
