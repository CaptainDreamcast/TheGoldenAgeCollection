#include "hank_gamescreen.h"

#include <tari/drawing.h>
#include <tari/log.h>
#include <tari/input.h>
#include <tari/system.h>

#include "hank_physics.h"
#include "hank_collision.h"
#include "hank_movement.h"
#include "hank_stagelogic.h"
#include "hank_animation.h"
#include "hank_drawing.h"
#include "hank_loadgame.h"
#include "hank_titlescreen.h"
#include "hank_roundscreen.h"
#include "hank_texture.h"

static HankWorldData gHankWorldData;
static HankCharacterData gCharacterData;


static void drawGameScreen() {
	drawHank(&gHankWorldData, &gCharacterData);
}
  

static void updateGameScreen() {
	updateSystem();
	updateInput();

	checkHankCollisionsPlatforms(&gHankWorldData, &gCharacterData);
	checkHankCollisionsEnemies(&gHankWorldData, &gCharacterData);

	checkHankJumpingCharacter(&gHankWorldData, &gCharacterData);
	checkHankRunningCharacter(&gHankWorldData, &gCharacterData);
	checkHankMovementEnemies(&gHankWorldData, &gCharacterData);

	handleHankPhysicsForCharacter(&gHankWorldData, &gCharacterData);
	handleHankPhysicsForEnemies(&gHankWorldData, &gCharacterData);

	checkHankGameOver(&gHankWorldData, &gCharacterData);
	checkHankExit(&gHankWorldData, &gCharacterData);
	handleHankScreenTilting(&gHankWorldData, &gCharacterData);

	HankGameReturnType currentGameReturnStatus = checkHankGameAbort(&gHankWorldData, &gCharacterData);
	debugInteger(currentGameReturnStatus);
	if (currentGameReturnStatus == HANK_RETURN_LOSS || currentGameReturnStatus == HANK_RETURN_TO_MENU) {
		setNewScreen(&HankTitleScreen);
	}
	else if (currentGameReturnStatus == HANK_RETURN_WON) {
		setNewScreen(&HankRoundScreen);
	}

	handleHankCharacterAnimation(&gHankWorldData, &gCharacterData);
	handleHankEnemyAnimation(&gHankWorldData, &gCharacterData);
}

static void loadGameScreen() {
	loadAllHankTextures();
	memset(&gHankWorldData, 0, sizeof gHankWorldData);
	memset(&gCharacterData, 0, sizeof gCharacterData);
	loadHankGame(&gHankWorldData, &gCharacterData);
}

Screen HankGameScreen = {
	.mLoad = loadGameScreen,
	.mUpdate = updateGameScreen,
	.mDraw = drawGameScreen,
};