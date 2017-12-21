#include "bombx_levelhandler.h"

#include <stdlib.h>

#include <tari/file.h>
#include <tari/log.h>
#include <tari/system.h>
#include <tari/geometry.h>
#include <tari/animation.h>
#include <tari/screeneffect.h>

#include "bombx_bombxhandler.h"
#include "bombx_gamescreen.h"

static struct {
	int mCurrentLevel;

	Vector3DI mPlayerStartTile16;
	TextureData mTextures[20];
	
	int mID[32][64];

	int mHoleAmount;

	TextureData mIntroTextTexture;
	TextureData mResetTextTexture;
} gData;


static int gLevels[] = {
	1,
	3,
	5,
	6,
	7,
	11,
	12,
	13,
	14,
	15,
	16,
	17,
	21,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	33,
	10,
	18,
	19,
	34,
	20,
	35,
	36,
	22,
	37,
	4,
	8,
	9,
	38,
	39,
	40,
	2
};


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

static int checkIfBufferStillHasWord(BufferPointer p, char* tPattern) {
	int bytesRead;
	char nextWord[1024];
	nextWord[0] = '\0';

	while (strcmp(tPattern, nextWord)) {
		int items = sscanf(p, "%s%n", nextWord, &bytesRead);
		
		if (items != 1) {
			return 0;
		}

		p += bytesRead;
	}

	return 1;
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

static void readHoleAmountFromLayout() {
	gData.mHoleAmount = 0;
	
	int y, x;
	for (y = 0; y < 32-4; y++) {
		for (x = 0; x < 64 - 24; x++) {
			gData.mHoleAmount += gData.mID[y][x] == 0;
		}
	}

	gData.mHoleAmount /= 4;
}

static int stillHasBombx(BufferPointer p) {
	return checkIfBufferStillHasWord(p, "dc.w");
}

static void loadIntroText() {
	int level = gLevels[gData.mCurrentLevel];
	if (level != 1) return;

	gData.mIntroTextTexture = loadTexture("assets/bombx/ui/TUTORIAL.pkg");
	playOneFrameAnimationLoop(makePosition(69, 303, 4), &gData.mIntroTextTexture);
}

static void loadResetText() {
#ifdef DREAMCAST
	gData.mResetTextTexture = loadTexture("assets/bombx/ui/PRESS_RESET_DC.pkg");
#else
	gData.mResetTextTexture = loadTexture("assets/bombx/ui/PRESS_RESET_WIN.pkg");
#endif

	playOneFrameAnimationLoop(makePosition(96, 416, 4), &gData.mResetTextTexture);
}

static void loadLevelFromFile() {
	
	char path[1024];
	sprintf(path, "assets/bombx/levels/level%d.asm", gLevels[gData.mCurrentLevel]);
	Buffer b = fileToBuffer(path);
	BufferPointer p = getBufferPointer(b);
	
	readBombxLevelAddress(&p);
	
	int secondsGained;
	readBombxWord(&p, &secondsGained);
	readBombxWord(&p, &gData.mPlayerStartTile16.x);
	readBombxWord(&p, &gData.mPlayerStartTile16.y);

	int boxAmount;
	readBombxWord(&p, &boxAmount);

	boxAmount = 0;
	while (stillHasBombx(p)) {
		Vector3DI bombxTilePosition;
		int bombxTimerValue;

		readBombxWord(&p, &bombxTilePosition.x);
		readBombxWord(&p, &bombxTilePosition.y);
		readBombxWord(&p, &bombxTimerValue);

		addBombx(bombxTilePosition, bombxTimerValue);
	}

	freeBuffer(b);

	readHoleAmountFromLayout();

	loadIntroText();
	loadResetText();
}

static void loadLevelAssets() {
	loadConsecutiveTextures(gData.mTextures, "assets/bombx/bg/BG.pkg", 16);
}

static void loadBombxLevelHandler(void* tData) {
	(void)tData;

	loadLevelAssets();
	loadLevelFromFile();
}



ActorBlueprint BombxLevelHandler = {
	.mLoad = loadBombxLevelHandler,
};

void resetBombxLevels()
{
	gData.mCurrentLevel = 0;
}

Vector3DI getPlayerTileStartPosition()
{
	return gData.mPlayerStartTile16;
}

int isBombxLevelTileFree(Vector3DI tTilePosition)
{
	int x8 = tTilePosition.x * 2;
	int y8 = tTilePosition.y * 2;

	return gData.mID[y8][x8] == 3 && gData.mID[y8 + 1][x8] == 3 && gData.mID[y8][x8 + 1] == 3 && gData.mID[y8 + 1][x8 + 1] == 3;
}

int isBombxLevelTileHole(Vector3DI tTilePosition)
{
	int x8 = tTilePosition.x * 2;
	int y8 = tTilePosition.y * 2;

	return gData.mID[y8][x8] == 0 && gData.mID[y8 + 1][x8] == 0 && gData.mID[y8][x8 + 1] == 0 && gData.mID[y8 + 1][x8 + 1] == 0;
}



void fillBombxHole()
{
	gData.mHoleAmount--;
}

int areAllBombxHolesFilled()
{
	return gData.mHoleAmount == 0;
}

static void gotoGameScreen(void* tCaller) {
	(void)tCaller;
	setNewScreen(&BombxGameScreen);
}

void setBombxLevelWon()
{
	gData.mCurrentLevel++;
	addFadeOut(30, gotoGameScreen, NULL);
}

void resetBombxLevel()
{
	addFadeOut(30, gotoGameScreen, NULL);
}

int isFinalBombxLevel()
{
	int level = gLevels[gData.mCurrentLevel];
	return level == 2;
}
