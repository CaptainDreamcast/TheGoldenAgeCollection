#include "beyond_assignment.h"

#include <stdio.h>
#include <assert.h>

#include <prism/mugenassignmentevaluator.h>
#include <prism/math.h>

#include "beyond_boss.h"
#include "beyond_shothandler.h"
#include "beyond_enemyhandler.h"
#include "beyond_player.h"
#include "beyond_level.h"

static struct {
	int mRand1;
	int mRand2;
} gData;

static void loadGameAssignments();

static void loadAssignmentHandler(void* tData) {
	(void)tData;
	gData.mRand1 = 0;
	gData.mRand2 = 0;
	loadGameAssignments();
}

static void updateAssignmentHandler(void* tData) {
	(void)tData;
	gData.mRand1 = randfromInteger(0, 10000);
	gData.mRand2 = randfromInteger(0, 10000);
}

ActorBlueprint BeyondAssignmentHandler = {
	.mLoad = loadAssignmentHandler,
	.mUpdate = updateAssignmentHandler,
};

static void fetchRandFromValue(char* tDst, void* tCaller, char* tIndex) {
	(void)tCaller;
	double a, b;
	char comma[20];
	int items = sscanf(tIndex, "%lf %s %lf", &a, comma, &b);
	assert(items == 3);
	assert(!strcmp(",", comma));
	
	double val = randfrom(a, b);
	sprintf(tDst, "%f", val);
}

static void fetchRandFromIntegerValue(char* tDst, void* tCaller, char* tIndex) {
	(void)tCaller;
	int a, b;
	char comma[20];
	int items = sscanf(tIndex, "%d %s %d", &a, comma, &b);
	assert(items == 3);
	assert(!strcmp(",", comma));

	int val = randfromInteger(a, b);
	sprintf(tDst, "%d", val);
}


static void fetchIdentity(char* tDst, void* tCaller, char* tIndex) {
	(void)tCaller;
	sprintf(tDst, "%s", tIndex);
}

static void fetchRand1(char* tDst, void* tCaller) {
	(void)tCaller;
	sprintf(tDst, "%d", gData.mRand1);
}

static void fetchRand2(char* tDst, void* tCaller) {
	(void)tCaller;
	sprintf(tDst, "%d", gData.mRand2);
}

static void fetchPI(char* tDst, void* tCaller) {
	(void)tCaller;
	sprintf(tDst, "3.14159");
}

static void fetchInfinity(char* tDst, void* tCaller) {
	(void)tCaller;
	sprintf(tDst, "%d", INF);
}

static void loadGameAssignments()
{
	resetMugenAssignmentContext();
	
	addMugenAssignmentVariable("rand1", fetchRand1);
	addMugenAssignmentVariable("rand2", fetchRand2);
	addMugenAssignmentVariable("pi", fetchPI);
	addMugenAssignmentVariable("bosstime", fetchBeyondBossTimeVariable);
	addMugenAssignmentVariable("angletowardsplayer", getBeyondShotAngleTowardsPlayer);
	addMugenAssignmentVariable("inf", fetchInfinity);
	addMugenAssignmentVariable("curenemy", getBeyondCurrentEnemyIndex);
	addMugenAssignmentVariable("cursubshot", getBeyondCurrentSubShotIndex);
	addMugenAssignmentVariable("localdeathcount", getBeyondLocalDeathCountVariable);
	addMugenAssignmentVariable("localbombcount", getBeyondLocalBombCountVariable);
	addMugenAssignmentVariable("stageparttime", fetchBeyondStagePartTime);
	
	addMugenAssignmentVariable("bigbang", evaluateBeyondBigBangFunction);
	addMugenAssignmentVariable("bounce", evaluateBeyondBounceFunction); 
	addMugenAssignmentVariable("ackermann", evaluateBeyondAckermannFunction);
	addMugenAssignmentVariable("groovy", evaluateBeyondSwirlFunction);
	addMugenAssignmentVariable("blam", evaluateBeyondBlamFunction);
	addMugenAssignmentVariable("transience", evaluateBeyondTransienceFunction);


	addMugenAssignmentVariable("textaid", evaluateBeyondTextAidFunction);

	addMugenAssignmentArray("randfrom", fetchRandFromValue);
	addMugenAssignmentArray("randfrominteger", fetchRandFromIntegerValue);
	addMugenAssignmentArray("identity", fetchIdentity);
	
}
