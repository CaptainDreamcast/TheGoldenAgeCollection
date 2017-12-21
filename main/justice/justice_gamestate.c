#include "justice_gamestate.h"

#include <stdlib.h>
#include <string.h>

static struct {
	int health;
	
	int isGameCleared;
	int isLevelCleared;
	int hasLost;
	char currentLevelName[100];

} gData;

void resetJusticeGameState() {
	resetJusticeHealth();
	gData.isLevelCleared = 0;
	gData.isGameCleared = 0;
	gData.hasLost = 0;
}

void resetJusticeLevelState() {
	gData.isLevelCleared = 0;
	gData.hasLost = 0;
}

int hasClearedJusticeLevel() {
	return gData.isLevelCleared;
}

void setJusticeLevelCleared(int remainingHealth) {
	gData.isLevelCleared = 1;
	gData.health = remainingHealth;
}

int hasClearedJusticeGame() {
	return gData.isGameCleared;
}

void setJusticeGameCleared() {
	gData.isGameCleared = 1;
}

int hasJusticeLost() {
	return gData.hasLost;
}

void setJusticeLost() {
	gData.hasLost = 1;
}

void resetJusticeHealth() {
	gData.health = 1000;
}

int getRemainingJusticeHealth(){
	return gData.health;
}

void setCurrentJusticeLevelName(char* name) {
	strcpy(gData.currentLevelName, name);
}

char* getCurrentJusticeLevelName() {
	return gData.currentLevelName;
}
