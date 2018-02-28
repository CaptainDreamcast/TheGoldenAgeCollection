#include "fight_fightscreen.h"

#include <stdio.h>

#include <prism/input.h>
#include <prism/stagehandler.h>
#include <prism/collisionhandler.h>
#include <prism/system.h>
#include <prism/mugenanimationreader.h>

#include "fight_stage.h"
#include "fight_mugencommandreader.h"
#include "fight_mugenstatereader.h"
#include "fight_playerdefinition.h"
#include "fight_mugenanimationhandler.h"
#include "fight_mugencommandhandler.h"
#include "fight_mugenstatehandler.h"
#include "fight_collision.h"
#include "fight_fightui.h"
#include "fight_mugenexplod.h"
#include "fight_gamelogic.h"
#include "fight_config.h"
#include "fight_playerhitdata.h"
#include "fight_titlescreen.h"
#include "fight_projectile.h"
#include "fight_ai.h"
#include "fight_titlescreen.h"

static Screen* getNextFightScreenScreen() {
	if (hasPressedAbortFlank()) {
		return &DreamTitleScreen;
	}

	return NULL;
}

static void loadFightScreen() {
	setupDreamGameCollisions();
	
	instantiateActor(DreamAIHandler);
	instantiateActor(DreamMugenConfig);
	instantiateActor(HitDataHandler);
	instantiateActor(ProjectileHandler);
	instantiateActor(DreamMugenGameAnimationHandler);
	instantiateActor(DreamMugenCommandHandler);
	instantiateActor(DreamMugenStateHandler);
	instantiateActor(DreamExplodHandler);

	setDreamStageMugenDefinition("assets/main/fight/stages/stage0.def");
	instantiateActor(DreamStageBP);

	
	//setPlayerDefinitionPath(0, "assets/main/fight/Mima_RP/Mima_RP.def");
	//setPlayerDefinitionPath(0, "assets/main/fight/Ryu/Ryu.def");
	//setPlayerDefinitionPath(0, "assets/main/fight/Beat/Beat.def");
	//setPlayerDefinitionPath(0, "assets/main/fight/kfm/kfm.def");
	//setPlayerDefinitionPath(0, "assets/main/fight/SonicV2/SonicV2.def");
	//setPlayerDefinitionPath(0, "assets/main/fight/Sonicth/Sonicth.def");

	//setPlayerDefinitionPath(1, "assets/main/fight/Ryu/Ryu.def");
	//setPlayerDefinitionPath(1, "assets/main/fight/liukang/liukang.def");
	//setPlayerDefinitionPath(1, "assets/main/fight/Beat/Beat.def");
	//setPlayerDefinitionPath(1, "assets/main/fight/kfm/kfm.def");
	//setPlayerDefinitionPath(1, "assets/main/fight/SonicV2/SonicV2.def");


	loadPlayers();

	instantiateActor(DreamFightUIBP);
	instantiateActor(DreamGameLogic);
	
	// activateCollisionHandlerDebugMode();

}

static void updateFightScreen() {
	updatePlayers();
}

Screen DreamFightScreen = {
	.mLoad = loadFightScreen,
	.mGetNextScreen = getNextFightScreenScreen,
	.mUpdate = updateFightScreen,
};
