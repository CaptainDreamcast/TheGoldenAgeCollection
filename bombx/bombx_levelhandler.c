#include "bombx_levelhandler.h"

#include <stdlib.h>

#include <tari/file.h>
#include <tari/log.h>
#include <tari/system.h>
#include <tari/geometry.h>
#include <tari/animation.h>

#include "bombx_bombxhandler.h"

static struct {
	int mCurrentLevel;

	Vector3DI mPlayerStartTile16;
	TextureData mTextures[20];
	
	int mID[32][64];


} gData;

static void readFromBufferUntilWord(BufferPointer* p, char* tPattern) {
	int bytesRead;
	char nextWord[1024];
	nextWord[0] = '\0';

	while (strcmp(tPattern, nextWord)) {
		int items = sscanf(*p, "%s%n", nextWord, &bytesRead);

		if (!items) {
			logError("Unable to find pattern.");
			logErrorString(tPattern);
			abortSystem();
		}

		*p += bytesRead;
	}
}

static void parseBombxLevelLayout(char* tBinaryFileName) {
	char path[1024];
	sprintf(path, "assets/bombx/levels/%s.bin", tBinaryFileName);

	Buffer b = fileToBuffer(path);
	BufferPointer p = getBufferPointer(b);

	int y, x;
	for (y = 0; y < 32; y++) {
		for (x = 0; x < 64; x++) {
			int mirror = (int)(*p);
			p++;

			gData.mID[y][x] = (int)(*p);
			p++;

			int animationID = playOneFrameAnimationLoop(makePosition(x*16, y*16, 1), &gData.mTextures[gData.mID[y][x]]);
			setAnimationCenter(animationID, makePosition(7.5, 7.5, 0));

			
			if (mirror & 8) {
				inverseAnimationVertical(animationID);
			}
			if (mirror & 16) {	
				inverseAnimationHorizontal(animationID);
			}
		}
	}

	freeBuffer(b);
}

static void readBombxWord(BufferPointer* p, int* mInteger) {
	readFromBufferUntilWord(p, "dc.w");

	char integerString[100];
	sscanf(*p, "%s", integerString);

	if (integerString[0] == '0' && integerString[1] == 'x') {
		*mInteger = strtol(integerString, NULL, 16);
	}
	else {
		*mInteger = strtol(integerString, NULL, 10);
	}
}

static void readBombxLevelAddress(BufferPointer* p) {
	char binaryFileName[1024];

	readFromBufferUntilWord(p, "dc.l");
	sscanf(*p, "%s", binaryFileName);

	parseBombxLevelLayout(binaryFileName);
}

static void loadLevelFromFile() {
	
	char path[1024];
	sprintf(path, "assets/bombx/levels/level3.asm");
	Buffer b = fileToBuffer(path);
	BufferPointer p = getBufferPointer(b);
	
	readBombxLevelAddress(&p);
	
	int secondsGained;
	readBombxWord(&p, &secondsGained);
	readBombxWord(&p, &gData.mPlayerStartTile16.x);
	readBombxWord(&p, &gData.mPlayerStartTile16.y);

	int boxAmount;
	readBombxWord(&p, &boxAmount);

	int i;
	for (i = 0; i < boxAmount; i++) {
		Vector3DI bombxTilePosition;
		int bombxTimerValue;

		readBombxWord(&p, &bombxTilePosition.x);
		readBombxWord(&p, &bombxTilePosition.y);
		readBombxWord(&p, &bombxTimerValue);

		addBombx(bombxTilePosition, bombxTimerValue);
	}

	freeBuffer(b);
}

static void loadLevelAssets() {
	loadConsecutiveTextures(gData.mTextures, "assets/bombx/bg/BG.pkg", 16);
}

static void loadBombxLevelHandler(void* tData) {
	(void)tData;

	gData.mCurrentLevel = 3; // TODO

	loadLevelAssets();
	loadLevelFromFile();
}



ActorBlueprint BombxLevelHandler = {
	.mLoad = loadBombxLevelHandler,
};