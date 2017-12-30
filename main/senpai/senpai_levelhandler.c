#include "senpai_levelhandler.h"

#include <tari/animation.h>
#include <tari/math.h>
#include <tari/texthandler.h>
#include <tari/sound.h>

#include "senpai_player.h"
#include "senpai_enemyhandler.h"

static struct {
	TextureData mBGTextures[10];

	int mMaze[32][32];
	Vector3DI mSize;

	int mMazeIDs[32][32];
	int mEntryIDs[25][4];
	int mExitIDs[25][4];

	int mStartTileX;
	int mEndTileX;

	Position mStageBasePosition;

	int mCurrentLevel;
	int mCurrentLevelTextID;
} gData;

static void fillMazeMatrix() {
	int x, y;

	for (x = 0; x < gData.mSize.x; x++) {
		for (y = 0; y < gData.mSize.y; y++) {
			if (y == 0 && x == gData.mStartTileX) {
				gData.mMaze[y][x] = 0;
			}
			else if (y == 1 && x == gData.mStartTileX) {
				gData.mMaze[y][x] = 0;
			}
			else if (y == gData.mSize.y - 1 && x == gData.mEndTileX) {
				gData.mMaze[y][x] = 0;
			}
			else if (y == gData.mSize.y - 2 && x == gData.mEndTileX) {
				gData.mMaze[y][x] = 0;
			}
			else if (y == 0 || x == 0 || y == gData.mSize.y - 1 || x == gData.mSize.x - 1 || ((y % 2 == 0) && (x % 2 == 0))) {
				gData.mMaze[y][x] = 1;
			}
			else {
				gData.mMaze[y][x] = 0;
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------
	for (y = 2; y < gData.mSize.y - 1; y += 2) {
		int dx = 2;
		int dy = y;
		int rnd4;

		switch (rnd4 = rand() % 4) {
		case 0: dx++; break;
		case 1: dx--; break;
		case 2: dy++; break;
		case 3: dy--; break;
		}

		if (dy == 1 && dx == gData.mStartTileX) continue;
		if (dy == 2 && dx == gData.mStartTileX) continue;
		if (dy == gData.mSize.y - 2 && dx == gData.mEndTileX) continue;
		if (dy == gData.mSize.y - 1 && dx == gData.mEndTileX) continue;


		gData.mMaze[dy][dx] = 1;
	}
	for (x = 4; x < gData.mSize.x - 1; x += 2) {
		for (y = 2; y < gData.mSize.y - 1; y += 2) {
			int dx = x;
			int dy = y;
			int rnd3;

			switch (rnd3 = rand() % 3) {
			case 0: dy++; break;
			case 1: dy--; break;
			case 2: dx++; break;
			}

			if (dy == 1 && dx == gData.mStartTileX) continue;
			if (dy == 2 && dx == gData.mStartTileX) continue;
			if (dy == gData.mSize.y - 2 && dx == gData.mEndTileX) continue;
			if (dy == gData.mSize.y - 1 && dx == gData.mEndTileX) continue;

			gData.mMaze[dy][dx] = 1;
		}
	}


}

static void drawMazeMatrix() {
	int x, y;
	for (y = 0; y < gData.mSize.y; y++) {
		for (x = 0; x < gData.mSize.x; x++) {
			int index = gData.mMaze[y][x];
			printf("%d ", index);
			gData.mMazeIDs[y][x] = playOneFrameAnimationLoop(makePosition(x * 64, y * 64, 1), &gData.mBGTextures[index]);
			setAnimationBasePositionReference(gData.mMazeIDs[y][x], &gData.mStageBasePosition);
		}
		printf("\n");
	}
}

static void unloadMaze() {
	int x, y;
	for (y = 0; y < gData.mSize.y; y++) {
		for (x = 0; x < gData.mSize.x; x++) {
			removeHandledAnimation(gData.mMazeIDs[y][x]);
		}
	}
}

static void drawExitHallway() {
	int y;
	for (y = -20; y < 0; y++) {
		int x = gData.mStartTileX-1;
		gData.mExitIDs[-y][0] = playOneFrameAnimationLoop(makePosition(x * 64, y * 64, 1), &gData.mBGTextures[1]);
		setAnimationBasePositionReference(gData.mExitIDs[-y][0], &gData.mStageBasePosition);
		x++;
		gData.mExitIDs[-y][1] = playOneFrameAnimationLoop(makePosition(x * 64, y * 64, 1), &gData.mBGTextures[2]);
		setAnimationBasePositionReference(gData.mExitIDs[-y][1], &gData.mStageBasePosition);
		x++;
		gData.mExitIDs[-y][2] = playOneFrameAnimationLoop(makePosition(x * 64, y * 64, 1), &gData.mBGTextures[1]);
		setAnimationBasePositionReference(gData.mExitIDs[-y][2], &gData.mStageBasePosition);
	}
}

static void unloadExit() {
	int y, x;
	for (y = 1; y <= 20; y++) {
		for (x = 0; x < 3; x++) {
			removeHandledAnimation(gData.mExitIDs[y][x]);
		}
	}
}

static void drawEntryHallway() {
	int y, x, indexY;
	for (y = gData.mSize.y; y < gData.mSize.y + 20; y++) {
		x = gData.mEndTileX - 1;
		indexY = y - gData.mSize.y + 1;
		gData.mEntryIDs[indexY][0] = playOneFrameAnimationLoop(makePosition(x * 64, y * 64, 1), &gData.mBGTextures[1]);
		setAnimationBasePositionReference(gData.mEntryIDs[indexY][0], &gData.mStageBasePosition);
		x++;
		gData.mEntryIDs[indexY][1] = playOneFrameAnimationLoop(makePosition(x * 64, y * 64, 1), &gData.mBGTextures[2]);
		setAnimationBasePositionReference(gData.mEntryIDs[indexY][1], &gData.mStageBasePosition);
		x++;
		gData.mEntryIDs[indexY][2] = playOneFrameAnimationLoop(makePosition(x * 64, y * 64, 1), &gData.mBGTextures[1]);
		setAnimationBasePositionReference(gData.mEntryIDs[indexY][2], &gData.mStageBasePosition);
	}

	y = gData.mSize.y + 20;
	x = gData.mEndTileX - 1;
	indexY = y - gData.mSize.y + 1;
	gData.mEntryIDs[indexY][0] = playOneFrameAnimationLoop(makePosition(x * 64, y * 64, 1), &gData.mBGTextures[1]);
	setAnimationBasePositionReference(gData.mEntryIDs[indexY][0], &gData.mStageBasePosition);
	x++;
	gData.mEntryIDs[indexY][1] = playOneFrameAnimationLoop(makePosition(x * 64, y * 64, 1), &gData.mBGTextures[1]);
	setAnimationBasePositionReference(gData.mEntryIDs[indexY][1], &gData.mStageBasePosition);
	x++;
	gData.mEntryIDs[indexY][2] = playOneFrameAnimationLoop(makePosition(x * 64, y * 64, 1), &gData.mBGTextures[1]);
	setAnimationBasePositionReference(gData.mEntryIDs[indexY][2], &gData.mStageBasePosition);
	
}

static void unloadEntry() {
	int y, x;
	for (y = 1; y <= 21; y++) {
		for (x = 0; x < 3; x++) {
			removeHandledAnimation(gData.mEntryIDs[y][x]);
		}
	}
}

static void updateStageBasePosition();

static int gVis[33][33];

static void checkPositionRecursively(int x, int y) {
	if (x < 0 || y < 0 || x >= gData.mSize.x || y >= gData.mSize.y || gData.mMaze[y][x] || gVis[y][x]) return;

	gVis[y][x] = 1;

	checkPositionRecursively(x + 1, y);
	checkPositionRecursively(x - 1, y);
	checkPositionRecursively(x, y + 1);
	checkPositionRecursively(x, y - 1);
}

static int canPlayMaze() {
	memset(gVis, 0, sizeof gVis);
	checkPositionRecursively(gData.mStartTileX, 0);

	return gVis[gData.mSize.y - 1][gData.mEndTileX];
}


static void generateLevelLayout() {
	int i;
	for (i = 0; i < 100; i++) {
		gData.mSize = makeVector3DI(randfromInteger(10, 30), randfromInteger(10, 30), 1);

		gData.mStartTileX = randfromInteger(1, gData.mSize.x - 2);
		gData.mEndTileX = randfromInteger(1, gData.mSize.x - 2);

		fillMazeMatrix();

		if (canPlayMaze()) break;
	}
}

static int getRecommendedEnemyAmount() {
	int ret = (((gData.mSize.x / 10)*(gData.mSize.y / 10)) / 2) + 1;
	return ret;
}

static void addEnemies() {
	int amount = getRecommendedEnemyAmount();
	int i;
	for (i = 0; i < amount; i++) {
		addSenpaiEnemy();
	}
}

static void increaseLevel() {
	gData.mCurrentLevel++;

	char text[100];
	sprintf(text, "CURRENT LEVEL: %d", gData.mCurrentLevel);

	setHandledText(gData.mCurrentLevelTextID, text);
}

static void generateLevel() {
	
	generateLevelLayout();

	drawMazeMatrix();
	drawEntryHallway();
	drawExitHallway();

	addEnemies();

	setSenpaiPlayerToStart(makeVector3DI(gData.mEndTileX, gData.mSize.y + 10, 0));
	updateStageBasePosition();

	increaseLevel();
}

static void unloadLevel() {
	unloadMaze();
	unloadEntry();
	unloadExit();
	unloadSenpaiEnemies();
}

static void loadLevelHandler(void* tCaller) {
	loadConsecutiveTextures(gData.mBGTextures, "assets/main/senpai/level/BG.pkg", 3);

	gData.mCurrentLevel = 0;
	gData.mCurrentLevelTextID = addHandledText(makePosition(20, 20, 60), "CURRENT LEVEL: 1", 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, 0, 0), makePosition(INF, INF, INF), INF);
	
	generateLevel();
}

static void updateStageBasePosition() {
	Position p = getSenpaiPlayerPosition();
	p = vecSub(p, makePosition(320, 240, 0));
	p = vecAdd(p, makePosition(32, 32, 0));
	gData.mStageBasePosition = vecScale(p, -1);
}

static void updateLevelHandler(void* tCaller) {
	updateStageBasePosition();
}

ActorBlueprint SenpaiLevelHandler = {
	.mLoad = loadLevelHandler,
	.mUpdate = updateLevelHandler
};

int canSenpaiPlayerGoThere(Vector3DI tTilePosition)
{
	if (tTilePosition.y >= gData.mSize.y) {
		return tTilePosition.y < gData.mSize.y + 20 && tTilePosition.x == gData.mEndTileX;
	}
	else if (tTilePosition.y < 0) {
		return tTilePosition.x == gData.mStartTileX;
	}
	else {
		return !gData.mMaze[tTilePosition.y][tTilePosition.x];
	}
}

int canSenpaiEnemyGoThere(Vector3DI tTilePosition)
{
	if (tTilePosition.x < 0 || tTilePosition.y < 0 || tTilePosition.x >= gData.mSize.x || tTilePosition.y >= gData.mSize.y) return 0;

	return !gData.mMaze[tTilePosition.y][tTilePosition.x];
}

int getSenpaiLevelSizeX()
{
	return gData.mSize.x;
}

int getSenpaiLevelSizeY()
{
	return gData.mSize.y;
}

int getSenpaiCurrentLevel()
{
	return gData.mCurrentLevel;
}

Position * getSenpaiLevelBasePositionReference()
{
	return &gData.mStageBasePosition;
}

int isSenpaiPositionCrossroads(Vector3DI tTilePosition)
{
	int goAmount = 0;
	goAmount += canSenpaiEnemyGoThere(vecAddI(tTilePosition, makeVector3DI(1, 0, 0)));
	goAmount += canSenpaiEnemyGoThere(vecAddI(tTilePosition, makeVector3DI(0, 1, 0)));
	goAmount += canSenpaiEnemyGoThere(vecAddI(tTilePosition, makeVector3DI(-1, 0, 0)));
	goAmount += canSenpaiEnemyGoThere(vecAddI(tTilePosition, makeVector3DI(0, -1, 0)));

	return goAmount >= 3;
}

Vector3DI getSenpaiPositionCrossroadsContinueDirection(Vector3DI tTilePosition)
{
	int endPointer = 0;
	Vector3DI mDirections[6];

	if (canSenpaiEnemyGoThere(vecAddI(tTilePosition, makeVector3DI(1, 0, 0)))) {
		mDirections[endPointer++] = makeVector3DI(1, 0, 0);
	}
	if (canSenpaiEnemyGoThere(vecAddI(tTilePosition, makeVector3DI(-1, 0, 0)))) {
		mDirections[endPointer++] = makeVector3DI(-1, 0, 0);
	}
	if (canSenpaiEnemyGoThere(vecAddI(tTilePosition, makeVector3DI(0, 1, 0)))) {
		mDirections[endPointer++] = makeVector3DI(0, 1, 0);
	}
	if (canSenpaiEnemyGoThere(vecAddI(tTilePosition, makeVector3DI(0, -1, 0)))) {
		mDirections[endPointer++] = makeVector3DI(0, -1, 0);
	}

	if (endPointer == 0) return vecAddI(tTilePosition, makeVector3DI(1, 0, 0));

	int index = randfromInteger(0, endPointer - 1);
	return vecAddI(tTilePosition, mDirections[index]);
}

void resetSenpaiPlayerAfterGameOver()
{
	setSenpaiPlayerToStart(makeVector3DI(gData.mEndTileX, gData.mSize.y + 10, 0));
}

void gotoNextSenpaiLevel()
{
	unloadLevel();
	generateLevel();

	if (gData.mCurrentLevel == 11) {
		addSenpaiFinalBoss(makeVector3DI(gData.mEndTileX, gData.mSize.y, 0));
	}
	else if (gData.mCurrentLevel >= 2 && gData.mCurrentLevel <= 10) {
		int trackOffset = 4;
		int trackIndex = gData.mCurrentLevel - 2;
		playTrackOnce(trackOffset + trackIndex);
	}
}
