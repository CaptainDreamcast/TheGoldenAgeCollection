#ifndef TEXTURE_H
#define TEXTURE_H

#include <tari/texture.h>

#include "hank_worlddata.h"
#include "hank_characterdata.h"
#include "hank_basetypes.h"

void loadAllHankTextures();
void unloadAllHankTextures();

TextureData getHankTitleTexture();
TextureData getHankPressStartTexture();
TextureData getHankBackgroundTexture();
TextureData getHankCharacterTexture(int tFrame);
TextureData getHankEnemyTexture(int tFrame);
TextureData getHankExitTexture();
TextureData getHankPlatformTexture();

#endif
