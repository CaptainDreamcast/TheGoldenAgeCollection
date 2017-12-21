#include "hank_texture.h"

#include <tari/log.h>

static TextureData gBackground;
static TextureData gCharacter[2];
static TextureData gEnemy[2];
static TextureData gExit;
static TextureData gPlatform;
static TextureData gTitle;
static TextureData gPressStart;

static void loadWorldTextures() {
  logg("Load world textures");
  gBackground = loadTexturePKG("assets/main/hank/sprites/background.pkg");
  gPlatform = loadTexturePKG("assets/main/hank/sprites/platform.pkg");
  gExit = loadTexturePKG("assets/main/hank/sprites/exit.pkg");
}

static void unloadWorldTextures() {
  logg("Unload world textures");
  unloadTexture(gBackground);
  unloadTexture(gPlatform);
  unloadTexture(gExit);
}

static void loadEnemyTextures() {
  logg("Load character textures");
  gEnemy[0] = loadTexturePKG("assets/main/hank/sprites/enemy1.pkg");
  gEnemy[1] = loadTexturePKG("assets/main/hank/sprites/enemy2.pkg");
}

static void unloadEnemyTextures() {
  logg("Unload enemy textures");
  unloadTexture(gEnemy[0]);
  unloadTexture(gEnemy[1]);
}

static void loadCharacterTextures() {
  logg("Load character textures");
  gCharacter[0] = loadTexturePKG("assets/main/hank/sprites/char1.pkg");
  gCharacter[1] = loadTexturePKG("assets/main/hank/sprites/char2.pkg");
}

static void unloadCharacterTextures() {
  logg("Unload character textures");
  unloadTexture(gCharacter[0]);
  unloadTexture(gCharacter[1]);
}

static void loadTitleTextures() {
  logg("Load title textures");
  gTitle = loadTexturePKG("assets/main/hank/sprites/title.pkg");
  gPressStart = loadTexturePKG("assets/main/hank/sprites/press_start.pkg");
}

static void unloadTitleTextures() {
  logg("Unoad title textures");
  unloadTexture(gTitle);
  unloadTexture(gPressStart);
}

void loadAllHankTextures() {
  loadWorldTextures();
  loadEnemyTextures();
  loadCharacterTextures();
  loadTitleTextures();
}

void unloadAllHankTextures() {
  unloadWorldTextures();
  unloadEnemyTextures();
  unloadCharacterTextures();
  unloadTitleTextures();
}

TextureData getHankBackgroundTexture() {
  return gBackground;
}

TextureData getHankCharacterTexture(int tFrame) {
  return gCharacter[tFrame];
}

TextureData getHankEnemyTexture(int tFrame) {
  return gEnemy[tFrame];
}

TextureData getHankExitTexture() {
  return gExit;
}

TextureData getHankPlatformTexture() {
  return gPlatform;
}

TextureData getHankTitleTexture() {
  return gTitle;
}

TextureData getHankPressStartTexture() {
  return gPressStart;
}
