#pragma once

#include <prism/actorhandler.h>

#include "fight_mugenstatereader.h"
#include "fight_playerdefinition.h"

typedef enum {
	MUGEN_ATTACK_CLASS_NORMAL,
	MUGEN_ATTACK_CLASS_SPECIAL,
	MUGEN_ATTACK_CLASS_HYPER,
} MugenAttackClass;

typedef enum {
	MUGEN_ATTACK_TYPE_ATTACK,
	MUGEN_ATTACK_TYPE_THROW,
	MUGEN_ATTACK_TYPE_PROJECTILE,
} MugenAttackType;

typedef enum {
	MUGEN_ATTACK_HEIGHT_HIGH,
	MUGEN_ATTACK_HEIGHT_LOW,
	MUGEN_ATTACK_HEIGHT_TRIP,
	MUGEN_ATTACK_HEIGHT_HEAVY,
	MUGEN_ATTACK_HEIGHT_NONE,
} MugenAttackHeight;

typedef enum {
	MUGEN_HIT_ANIMATION_TYPE_LIGHT,
	MUGEN_HIT_ANIMATION_TYPE_MEDIUM,
	MUGEN_HIT_ANIMATION_TYPE_HARD,
	MUGEN_HIT_ANIMATION_TYPE_BACK,
	MUGEN_HIT_ANIMATION_TYPE_UP,
	MUGEN_HIT_ANIMATION_TYPE_DIAGONAL_UP,
	MUGEN_HIT_ANIMATION_TYPE_HEAVY,

} MugenHitAnimationType;

typedef enum {
	MUGEN_AFFECT_TEAM_BOTH,
	MUGEN_AFFECT_TEAM_ENEMY,
	MUGEN_AFFECT_TEAM_FRIENDLY,

} MugenAffectTeam;

typedef enum {
	MUGEN_HIT_PRIORITY_DODGE,
	MUGEN_HIT_PRIORITY_HIT,
	MUGEN_HIT_PRIORITY_MISS,
} MugenHitPriorityType;

extern ActorBlueprint HitDataHandler;

int initPlayerHitDataAndReturnID(DreamPlayer* tPlayer);
void removePlayerHitData(DreamPlayer* tPlayer);

void copyHitDataToActive(DreamPlayer* tPlayer, void* tHitData);

int isReceivedHitDataActive(void* tHitData);
int isHitDataActive(DreamPlayer* tPlayer);
void setHitDataActive(DreamPlayer* tPlayer);
void setReceivedHitDataInactive(void* tHitData);
void setHitDataInactive(DreamPlayer* tPlayer);

void* getPlayerHitDataReference(DreamPlayer* tPlayer);
DreamPlayer* getReceivedHitDataPlayer(void* tHitData);

DreamMugenStateType getHitDataType(DreamPlayer* tPlayer);
void setHitDataType(DreamPlayer* tPlayer, DreamMugenStateType tType);
MugenAttackClass getHitDataAttackClass(DreamPlayer* tPlayer);
void setHitDataAttackClass(DreamPlayer* tPlayer, MugenAttackClass tClass);

MugenAttackType getHitDataAttackType(DreamPlayer* tPlayer);
void setHitDataAttackType(DreamPlayer* tPlayer, MugenAttackType tType);

void setHitDataHitFlag(DreamPlayer* tPlayer, char* tFlag);
char* getActiveHitDataGuardFlag(DreamPlayer* tPlayer);
void setHitDataGuardFlag(DreamPlayer* tPlayer, char* tFlag);
void setHitDataAffectTeam(DreamPlayer* tPlayer, MugenAffectTeam tAffectTeam);

MugenHitAnimationType getActiveHitDataAnimationType(DreamPlayer* tPlayer);
MugenHitAnimationType getHitDataAnimationType(DreamPlayer* tPlayer);
void setHitDataAnimationType(DreamPlayer* tPlayer, MugenHitAnimationType tAnimationType);
MugenHitAnimationType getHitDataAirAnimationType(DreamPlayer* tPlayer);
void setHitDataAirAnimationType(DreamPlayer* tPlayer, MugenHitAnimationType tAnimationType);
void setHitDataFallAnimationType(DreamPlayer* tPlayer, MugenHitAnimationType tAnimationType);

void setHitDataPriority(DreamPlayer* tPlayer, int tPriority, MugenHitPriorityType tPriorityType);
int getActiveHitDataDamage(DreamPlayer* tPlayer);
int getActiveHitDataGuardDamage(DreamPlayer* tPlayer);
int getHitDataDamage(DreamPlayer* tPlayer);
void setHitDataDamage(DreamPlayer* tPlayer, int tDamage, int tGuardDamage);

int getActiveHitDataPlayer1PauseTime(DreamPlayer* tPlayer);
int getActiveHitDataPlayer1GuardPauseTime(DreamPlayer* tPlayer);
int getHitDataPlayer1PauseTime(DreamPlayer* tPlayer);
int getActiveHitDataPlayer2PauseTime(DreamPlayer* tPlayer);
int getActiveHitDataPlayer2GuardPauseTime(DreamPlayer* tPlayer);
int getHitDataPlayer2PauseTime(DreamPlayer* tPlayer);
void setHitDataPauseTime(DreamPlayer* tPlayer, int tPlayer1PauseTime, int tPlayer2PauseTime);

void setHitDataGuardPauseTime(DreamPlayer* tPlayer, int tPlayer1PauseTime, int tPlayer2PauseTime);

int isActiveHitDataSparkInPlayerFile(DreamPlayer* tPlayer);
int isActiveHitDataGuardSparkInPlayerFile(DreamPlayer* tPlayer);
int isHitDataSparkInPlayerFile(DreamPlayer* tPlayer);
int getActiveHitDataSparkNumber(DreamPlayer* tPlayer);
int getActiveHitDataGuardSparkNumber(DreamPlayer* tPlayer);
int getHitDataGuardSparkNumber(DreamPlayer * tPlayer);
int getHitDataSparkNumber(DreamPlayer* tPlayer);
void setHitDataSparkNumber(DreamPlayer* tPlayer, int tIsInPlayerFile, int tNumber);
void setHitDataGuardSparkNumber(DreamPlayer* tPlayer, int tIsInPlayerFile, int tNumber);
Position getActiveHitDataSparkXY(DreamPlayer* tPlayer);
Position getHitDataSparkXY(DreamPlayer* tPlayer);
void setHitDataSparkXY(DreamPlayer* tPlayer, int tX, int tY);

void setHitDataHitSound(DreamPlayer* tPlayer, int tIsInPlayerFile, int tGroup, int tItem);
void setHitDataGuardSound(DreamPlayer* tPlayer, int tIsInPlayerFile, int tGroup, int tItem);

MugenAttackHeight getActiveHitDataGroundType(DreamPlayer* tPlayer);
MugenAttackHeight getHitDataGroundType(DreamPlayer* tPlayer);
void setHitDataGroundType(DreamPlayer* tPlayer, MugenAttackHeight tType);
MugenAttackHeight getActiveHitDataAirType(DreamPlayer* tPlayer);
MugenAttackHeight getHitDataAirType(DreamPlayer* tPlayer);
void setHitDataAirType(DreamPlayer* tPlayer, MugenAttackHeight tType);

int getActiveHitDataGroundHitTime(DreamPlayer* tPlayer);
int getHitDataGroundHitTime(DreamPlayer* tPlayer);
void setHitDataGroundHitTime(DreamPlayer* tPlayer, int tHitTime);
int getActiveHitDataGroundSlideTime(DreamPlayer* tPlayer);
int getHitDataGroundSlideTime(DreamPlayer* tPlayer);
void setHitDataGroundSlideTime(DreamPlayer* tPlayer, int tSlideTime);

int getActiveHitDataGuardHitTime(DreamPlayer* tPlayer);
int getHitDataGuardHitTime(DreamPlayer* tPlayer);
void setHitDataGuardHitTime(DreamPlayer* tPlayer, int tHitTime);
int getHitDataGuardSlideTime(DreamPlayer* tPlayer);
void setHitDataGuardSlideTime(DreamPlayer* tPlayer, int tSlideTime);

int getActiveHitDataAirHitTime(DreamPlayer* tPlayer);
void setHitDataAirHitTime(DreamPlayer* tPlayer, int tHitTime);
int getActiveHitDataGuardControlTime(DreamPlayer* tPlayer);
int getHitDataGuardControlTime(DreamPlayer* tPlayer);
void setHitDataGuardControlTime(DreamPlayer* tPlayer, int tControlTime);
int getHitDataGuardDistance(DreamPlayer* tPlayer);
void setHitDataGuardDistance(DreamPlayer* tPlayer, int tDistance);

double getActiveHitDataYAccel(DreamPlayer* tPlayer);
double getHitDataYAccel(DreamPlayer* tPlayer);
void setHitDataYAccel(DreamPlayer* tPlayer, double YAccel);

double getActiveHitDataGroundVelocityX(DreamPlayer* tPlayer);
double getHitDataGroundVelocityX(DreamPlayer* tPlayer);
double getActiveHitDataGroundVelocityY(DreamPlayer* tPlayer);
double getHitDataGroundVelocityY(DreamPlayer* tPlayer);
void setHitDataGroundVelocity(DreamPlayer* tPlayer, double tX, double tY);
double getActiveHitDataGuardVelocity(DreamPlayer* tPlayer);
double getHitDataGuardVelocity(DreamPlayer* tPlayer);
void setHitDataGuardVelocity(DreamPlayer* tPlayer, double tX);
double getActiveHitDataAirVelocityX(DreamPlayer* tPlayer);
double getHitDataAirVelocityX(DreamPlayer* tPlayer);
double getActiveHitDataAirVelocityY(DreamPlayer* tPlayer);
double getHitDataAirVelocityY(DreamPlayer* tPlayer);
void setHitDataAirVelocity(DreamPlayer* tPlayer, double tX, double tY);
void setHitDataAirGuardVelocity(DreamPlayer* tPlayer, double tX, double tY);

double getGroundCornerPushVelocityOffset(DreamPlayer* tPlayer);
void setGroundCornerPushVelocityOffset(DreamPlayer* tPlayer, double tX);
void setAirCornerPushVelocityOffset(DreamPlayer* tPlayer, double tX);
void setDownCornerPushVelocityOffset(DreamPlayer* tPlayer, double tX);
double getGuardCornerPushVelocityOffset(DreamPlayer* tPlayer);
void setGuardCornerPushVelocityOffset(DreamPlayer* tPlayer, double tX);
void setAirGuardCornerPushVelocityOffset(DreamPlayer* tPlayer, double tX);

int getActiveHitDataAirGuardControlTime(DreamPlayer* tPlayer);
void setHitDataAirGuardControlTime(DreamPlayer* tPlayer, int tControlTime);
void setHitDataAirJuggle(DreamPlayer* tPlayer, int tJuggle);

void setHitDataMinimumDistanceInactive(DreamPlayer* tPlayer);
void setHitDataMinimumDistance(DreamPlayer* tPlayer, int x, int y);

void setHitDataMaximumDistanceInactive(DreamPlayer* tPlayer);
void setHitDataMaximumDistance(DreamPlayer* tPlayer, int x, int y);

void setHitDataSnapInactive(DreamPlayer* tPlayer);
void setHitDataSnap(DreamPlayer* tPlayer, int x, int y);

void setHitDataPlayer1SpritePriority(DreamPlayer* tPlayer, int tPriority);
void setHitDataPlayer2SpritePriority(DreamPlayer* tPlayer, int tPriority);
void setHitDataPlayer1FaceDirection(DreamPlayer* tPlayer, int tFaceDirection);
void setHitDataPlayer1ChangeFaceDirectionRelativeToPlayer2(DreamPlayer* tPlayer, int tFaceDirection);
void setHitDataPlayer2ChangeFaceDirectionRelativeToPlayer1(DreamPlayer* tPlayer, int tFaceDirection);

int getActiveHitDataPlayer1StateNumber(DreamPlayer* tPlayer);
void setPlayer1StateNumber(DreamPlayer* tPlayer, int tStateNumber);
int getActiveHitDataPlayer2StateNumber(DreamPlayer* tPlayer);
void setPlayer2StateNumber(DreamPlayer* tPlayer, int tStateNumber);
int getActiveHitDataPlayer2CapableOfGettingPlayer1State(DreamPlayer* tPlayer);
void setHitDataPlayer2CapableOfGettingPlayer1State(DreamPlayer* tPlayer, int tVal);
void setHitDataForceStanding(DreamPlayer* tPlayer, int tIsForcedToStand);

int getActiveHitDataFall(DreamPlayer* tPlayer);
int getHitDataFall(DreamPlayer* tPlayer);
void setActiveHitDataFall(DreamPlayer* tPlayer, int tIsCausingPlayer2ToFall);
void setHitDataFall(DreamPlayer* tPlayer, int tIsCausingPlayer2ToFall);
double getActiveHitDataFallXVelocity(DreamPlayer* tPlayer);
double getHitDataFallXVelocity(DreamPlayer* tPlayer);
void setActiveHitDataFallXVelocity(DreamPlayer* tPlayer, double tX);
void setHitDataFallXVelocity(DreamPlayer* tPlayer, double tX);
double getActiveHitDataFallYVelocity(DreamPlayer* tPlayer);
double getHitDataFallYVelocity(DreamPlayer* tPlayer);
void setActiveHitDataFallYVelocity(DreamPlayer* tPlayer, double tY);
void setHitDataFallYVelocity(DreamPlayer* tPlayer, double tY);
int getActiveHitDataFallRecovery(DreamPlayer* tPlayer);
int getHitDataFallRecovery(DreamPlayer* tPlayer);
void setHitDataFallRecovery(DreamPlayer* tPlayer, int tCanRecover);
void setHitDataFallRecoveryTime(DreamPlayer* tPlayer, int tRecoverTime);
void setHitDataFallDamage(DreamPlayer* tPlayer, int tDamage);
int getActiveHitDataAirFall(DreamPlayer* tPlayer);
void setHitDataAirFall(DreamPlayer* tPlayer, int tIsCausingPlayer2ToFall);
void setHitDataForceNoFall(DreamPlayer* tPlayer, int tForcePlayer2NotToFall);

void setHitDataDownVelocity(DreamPlayer* tPlayer, double tX, double tY);
void setHitDataDownHitTime(DreamPlayer* tPlayer, int tHitTime);
void setHitDataDownBounce(DreamPlayer* tPlayer, int tDoesBounce);

void setHitDataHitID(DreamPlayer* tPlayer, int tID);
void setHitDataChainID(DreamPlayer* tPlayer, int tID);
void setHitDataNoChainID(DreamPlayer* tPlayer, int tID1, int tID2);

void setHitDataHitOnce(DreamPlayer* tPlayer, int tIsOnlyAffectingOneOpponent);
void setHitDataKill(DreamPlayer* tPlayer, int tCanKill);
void setHitDataGuardKill(DreamPlayer* tPlayer, int tCanKill);
void setHitDataFallKill(DreamPlayer* tPlayer, int tCanKill);
void setHitDataNumberOfHits(DreamPlayer* tPlayer, int tNumberOfHits);
int getActiveHitDataPlayer1PowerAdded(DreamPlayer* tPlayer);
int getActiveHitDataPlayer1GuardPowerAdded(DreamPlayer* tPlayer);
int getHitDataPlayer1PowerAdded(DreamPlayer* tPlayer);
void setHitDataGetPower(DreamPlayer* tPlayer, int tPlayer1PowerAdded, int tPlayer1PowerAddedWhenGuarded);
int getActiveHitDataPlayer2PowerAdded(DreamPlayer* tPlayer);
int getActiveHitDataPlayer2GuardPowerAdded(DreamPlayer* tPlayer);
int getHitDataPlayer2PowerAdded(DreamPlayer* tPlayer);
void setHitDataGivePower(DreamPlayer* tPlayer, int tPlayer2PowerAdded, int tPlayer2PowerAddedWhenGuarded);

void setHitDataPaletteEffectTime(DreamPlayer* tPlayer, int tEffectTime);
void setHitDataPaletteEffectMultiplication(DreamPlayer* tPlayer, int tR, int tG, int tB);
void setHitDataPaletteEffectAddition(DreamPlayer* tPlayer, int tR, int tG, int tB);

void setHitDataEnvironmentShakeTime(DreamPlayer* tPlayer, int tTime);
void setHitDataEnvironmentShakeFrequency(DreamPlayer* tPlayer, double tFrequency);
void setHitDataEnvironmentShakeAmplitude(DreamPlayer* tPlayer, int tAmplitude);
void setHitDataEnvironmentShakePhase(DreamPlayer* tPlayer, double tPhase);

void setHitDataFallEnvironmentShakeTime(DreamPlayer* tPlayer, int tTime);
void setHitDataFallEnvironmentShakeFrequency(DreamPlayer* tPlayer, double tFrequency);
void setHitDataFallEnvironmentShakeAmplitude(DreamPlayer* tPlayer, int tAmplitude);
void setHitDataFallEnvironmentShakePhase(DreamPlayer* tPlayer, double tPhase);

double getActiveHitDataVelocityX(DreamPlayer* tPlayer);
double getHitDataVelocityX(DreamPlayer* tPlayer);
void setActiveHitDataVelocityX(DreamPlayer* tPlayer, double x);
void setHitDataVelocityX(DreamPlayer* tPlayer, double x);
double getActiveHitDataVelocityY(DreamPlayer* tPlayer);
double getHitDataVelocityY(DreamPlayer* tPlayer);
void setActiveHitDataVelocityY(DreamPlayer* tPlayer, double y);
void setHitDataVelocityY(DreamPlayer* tPlayer, double y);

int getActiveHitDataIsFacingRight(DreamPlayer* tPlayer);
void setHitDataIsFacingRight(DreamPlayer* tPlayer, int tIsFacingRight);

void resetHitDataReversalDef(DreamPlayer* tPlayer);
void setHitDataReversalDefFlag1(DreamPlayer* tPlayer, char* tFlag);
void addHitDataReversalDefFlag2(DreamPlayer* tPlayer, char* tFlag);