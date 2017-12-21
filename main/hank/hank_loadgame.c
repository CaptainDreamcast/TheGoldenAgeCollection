#include "hank_loadgame.h"

#include <tari/physics.h>
#include <tari/log.h>

#include "hank_generateLevel.h"
#include "hank_basedefinitions.h"
#include "hank_texture.h"
#include "hank_state.h"

#define CHARACTER_FRAME_AMOUNT_STANDING 1
#define CHARACTER_FRAME_AMOUNT_WALKING 2
#define CHARACTER_FRAME_AMOUNT_JUMPING 2
#define CHARACTER_FRAME_AMOUNT_DYING 2

#define CHARACTER_TICK_AMOUNT_STANDING 30
#define CHARACTER_TICK_AMOUNT_WALKING 5
#define CHARACTER_TICK_AMOUNT_JUMPING 5
#define CHARACTER_TICK_AMOUNT_DYING 5

static void loadCharacter(HankWorldData* tWorldData, HankCharacterData* tCharacterData) {
  logg("Load character");
  tCharacterData->physics.mPosition.x = HANK_CHARACTER_START_POSITION_X;
  tCharacterData->physics.mPosition.y = HANK_CHARACTER_START_POSITION_Y;
  tCharacterData->physics.mPosition.z = HANK_CHARACTER_POSITION_Z;
  tCharacterData->frameAmount[HANK_CHARACTER_STANDING] = CHARACTER_FRAME_AMOUNT_STANDING;
  tCharacterData->frameAmount[HANK_CHARACTER_WALKING] = CHARACTER_FRAME_AMOUNT_WALKING;
  tCharacterData->frameAmount[HANK_CHARACTER_JUMPING] = CHARACTER_FRAME_AMOUNT_JUMPING;
  tCharacterData->frameAmount[HANK_CHARACTER_DYING] = CHARACTER_FRAME_AMOUNT_DYING;
  tCharacterData->runAccel = HANK_CHARACTER_RUN_ACCEL;
  tCharacterData->animationDuration[HANK_CHARACTER_STANDING] = CHARACTER_TICK_AMOUNT_STANDING;
  tCharacterData->animationDuration[HANK_CHARACTER_WALKING] = CHARACTER_TICK_AMOUNT_WALKING;
  tCharacterData->animationDuration[HANK_CHARACTER_JUMPING] = CHARACTER_TICK_AMOUNT_JUMPING;
  tCharacterData->animationDuration[HANK_CHARACTER_DYING] = CHARACTER_TICK_AMOUNT_DYING;
  changeHankCharacterState(tCharacterData, HANK_CHARACTER_STANDING);
  resetAnimation(&tCharacterData->animation);
}

void loadHankGame(HankWorldData* worldData, HankCharacterData* characterData) {
  logg("Begin loading game");
  generateHankLevel(worldData);
  loadCharacter(worldData, characterData);
}
