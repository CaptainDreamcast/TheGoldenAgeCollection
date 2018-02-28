#include "cane_gamelogic.h"

static struct {
	int isLevelLostFlag;

} gData;

void loadCaneGameLogic() {
	gData.isLevelLostFlag = 0;
}

void setCaneLevelLost() {
	gData.isLevelLostFlag = 1;
}

int isCaneLevelLost() {
	return gData.isLevelLostFlag;
}