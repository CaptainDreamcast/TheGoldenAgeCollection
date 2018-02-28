#pragma once

#include <prism/datastructures.h>
#include <prism/animation.h>

#include "fight_mugenassignment.h"

typedef enum {

	MUGEN_STATE_CONTROLLER_TYPE_NOT_HIT_BY,
	MUGEN_STATE_CONTROLLER_TYPE_CHANGE_STATE,
	MUGEN_STATE_CONTROLLER_TYPE_CHANGE_ANIMATION,
	MUGEN_STATE_CONTROLLER_TYPE_ASSERT_SPECIAL,
	MUGEN_STATE_CONTROLLER_TYPE_EXPLOD,
	MUGEN_STATE_CONTROLLER_TYPE_PLAY_SOUND,
	MUGEN_STATE_CONTROLLER_TYPE_SET_CONTROL,
	MUGEN_STATE_CONTROLLER_TYPE_HIT_DEFINITION,
	MUGEN_STATE_CONTROLLER_TYPE_WIDTH,
	MUGEN_STATE_CONTROLLER_TYPE_SPRITE_PRIORITY,
	MUGEN_STATE_CONTROLLER_TYPE_ADD_POSITION,
	MUGEN_STATE_CONTROLLER_TYPE_SET_VARIABLE,
	MUGEN_STATE_CONTROLLER_TYPE_BIND_TARGET,
	MUGEN_STATE_CONTROLLER_TYPE_TURN,
	MUGEN_STATE_CONTROLLER_TYPE_SET_TARGET_FACING,
	MUGEN_STATE_CONTROLLER_TYPE_ADD_TARGET_LIFE,
	MUGEN_STATE_CONTROLLER_TYPE_SET_TARGET_STATE,
	MUGEN_STATE_CONTROLLER_TYPE_CHANGE_ANIMATION_2,
	MUGEN_STATE_CONTROLLER_TYPE_SET_SELF_STATE,
	MUGEN_STATE_CONTROLLER_TYPE_ADD_VELOCITY,
	MUGEN_STATE_CONTROLLER_TYPE_SET_VELOCITY,
	MUGEN_STATE_CONTROLLER_TYPE_MULTIPLY_VELOCITY,
	MUGEN_STATE_CONTROLLER_TYPE_AFTER_IMAGE,
	MUGEN_STATE_CONTROLLER_TYPE_AFTER_IMAGE_TIME,
	MUGEN_STATE_CONTROLLER_TYPE_PALETTE_EFFECT,
	MUGEN_STATE_CONTROLLER_TYPE_SET_HIT_VELOCITY,
	MUGEN_STATE_CONTROLLER_TYPE_SCREEN_BOUND,
	MUGEN_STATE_CONTROLLER_TYPE_FREEZE_POSITION,
	MUGEN_STATE_CONTROLLER_TYPE_NULL,
	MUGEN_STATE_CONTROLLER_TYPE_SET_POSITION,
	MUGEN_STATE_CONTROLLER_TYPE_ENVIRONMENT_SHAKE,
	MUGEN_STATE_CONTROLLER_TYPE_REVERSAL_DEFINITION,
	MUGEN_STATE_CONTROLLER_TYPE_HIT_OVERRIDE,
	MUGEN_STATE_CONTROLLER_TYPE_PAUSE,
	MUGEN_STATE_CONTROLLER_TYPE_SUPER_PAUSE,
	MUGEN_STATE_CONTROLLER_TYPE_MAKE_DUST,
	MUGEN_STATE_CONTROLLER_TYPE_SET_STATE_TYPE,
	MUGEN_STATE_CONTROLLER_TYPE_FORCE_FEEDBACK,
	MUGEN_STATE_CONTROLLER_TYPE_SET_DEFENSE_MULTIPLIER,
	MUGEN_STATE_CONTROLLER_TYPE_ADD_VARIABLE,
	MUGEN_STATE_CONTROLLER_TYPE_FALL_ENVIRONMENT_SHAKE,
	MUGEN_STATE_CONTROLLER_TYPE_HIT_FALL_DAMAGE,
	MUGEN_STATE_CONTROLLER_TYPE_HIT_FALL_VELOCITY,
	MUGEN_STATE_CONTROLLER_TYPE_SET_HIT_FALL,
	MUGEN_STATE_CONTROLLER_TYPE_SET_VARIABLE_RANGE,
	MUGEN_STATE_CONTROLLER_TYPE_REMAP_PALETTE,
	MUGEN_STATE_CONTROLLER_TYPE_HIT_BY,
	MUGEN_STATE_CONTROLLER_TYPE_PLAYER_PUSH,
	MUGEN_STATE_CONTROLLER_TYPE_ADD_POWER,
	MUGEN_STATE_CONTROLLER_TYPE_HELPER,
	MUGEN_STATE_CONTROLLER_TYPE_STOP_SOUND,
	MUGEN_STATE_CONTROLLER_TYPE_REMOVE_EXPLOD,
	MUGEN_STATE_CONTROLLER_TYPE_DESTROY_SELF,
	MUGEN_STATE_CONTROLLER_TYPE_PALETTE_EFFECT_ALL,
	MUGEN_STATE_CONTROLLER_TYPE_ENVIRONMENT_COLOR,
	MUGEN_STATE_CONTROLLER_TYPE_VICTORY_QUOTE,
	MUGEN_STATE_CONTROLLER_TYPE_SET_ATTACK_MULTIPLIER,
	MUGEN_STATE_CONTROLLER_TYPE_ADD_LIFE,
	MUGEN_STATE_CONTROLLER_TYPE_PAN_SOUND,
	MUGEN_STATE_CONTROLLER_TYPE_DISPLAY_TO_CLIPBOARD,
	MUGEN_STATE_CONTROLLER_TYPE_APPEND_TO_CLIPBOARD,
	MUGEN_STATE_CONTROLLER_TYPE_GRAVITY,
	MUGEN_STATE_CONTROLLER_TYPE_SET_ATTACK_DISTANCE,
	MUGEN_STATE_CONTROLLER_TYPE_ADD_TARGET_POWER,
	MUGEN_STATE_CONTROLLER_TYPE_RESET_MOVE_HIT,
	MUGEN_STATE_CONTROLLER_TYPE_MODIFY_EXPLOD,
	MUGEN_STATE_CONTROLLER_TYPE_PALETTE_EFFECT_BACKGROUND,
	MUGEN_STATE_CONTROLLER_TYPE_ADD_HIT,
	MUGEN_STATE_CONTROLLER_TYPE_BIND_TO_ROOT,
	MUGEN_STATE_CONTROLLER_TYPE_TRANSPARENCY,
	MUGEN_STATE_CONTROLLER_TYPE_BIND_TO_PARENT,
	MUGEN_STATE_CONTROLLER_TYPE_SET_ANGLE,
	MUGEN_STATE_CONTROLLER_TYPE_ADD_ANGLE,
	MUGEN_STATE_CONTROLLER_TYPE_DRAW_ANGLE,
	MUGEN_STATE_CONTROLLER_TYPE_MAKE_GAME_ANIMATION,
	MUGEN_STATE_CONTROLLER_TYPE_SET_VARIABLE_RANDOM,
	MUGEN_STATE_CONTROLLER_TYPE_SET_PARENT_VARIABLE,
	MUGEN_STATE_CONTROLLER_TYPE_PROJECTILE,
	MUGEN_STATE_CONTROLLER_TYPE_SET_POWER,
	MUGEN_STATE_CONTROLLER_TYPE_SET_OFFSET,
} DreamMugenStateControllerType;

typedef struct {
	DreamMugenAssignment* mAssignment;

} DreamMugenStateControllerTrigger;

typedef struct {
	DreamMugenStateControllerType mType;
	DreamMugenStateControllerTrigger mTrigger;

	void* mData;
} DreamMugenStateController;

typedef struct {
	int mLife;
	int mPower;
	int mAttack;
	int mDefense;
	int mFallDefenseUp;
	Duration mLiedownTime;
	int mAirJugglePoints;
	int mIsSparkNoInPlayerFile;
	int mSparkNo;
	int mIsGuardSparkNoInPlayerFile;
	int mGuardSparkNo;
	int mKOEcho;
	int mVolume;

	int mIntPersistIndex;
	int mFloatPersistIndex;

} DreamMugenConstantsHeader;

typedef struct {
	Vector3D mScale;
	int mGroundBackWidth;
	int mGroundFrontWidth;
	int mAirBackWidth;
	int mAirFrontWidth;
	int mHeight;
	int mAttackDistance;
	int mProjectileAttackDistance;
	int mDoesScaleProjectiles;
	Vector3D mHeadPosition;
	Vector3D mMidPosition;
	int mShadowOffset;
	Vector3DI mDrawOffset;

} DreamMugenConstantsSizeData;

typedef struct {
	Vector3D mWalkForward;
	Vector3D mWalkBackward;

	Vector3D mRunForward;
	Vector3D mRunBackward;

	Vector3D mJumpNeutral;
	Vector3D mJumpBackward;
	Vector3D mJumpForward;

	Vector3D mRunJumpBackward;
	Vector3D mRunJumpForward;

	Vector3D mAirJumpNeutral;
	Vector3D mAirJumpBackward;
	Vector3D mAirJumpForward;

	Vector3D mAirGetHitGroundRecovery;
	Vector3D mAirGetHitAirRecoveryMultiplier;
	Vector3D mAirGetHitAirRecoveryOffset;

	double mAirGetHitExtraXWhenHoldingBackward;
	double mAirGetHitExtraXWhenHoldingForward;
	double mAirGetHitExtraYWhenHoldingUp;
	double mAirGetHitExtraYWhenHoldingDown;

} DreamMugenConstantsVelocityData;

typedef struct {
	int mAllowedAirJumpAmount;
	int mAirJumpMinimumHeight;
	double mVerticalAcceleration;
	double mStandFiction;
	double mCrouchFriction;
	double mStandFrictionThreshold;
	double mCrouchFrictionThreshold;

	int mAirGetHitGroundLevelY;
	int mAirGetHitGroundRecoveryGroundYTheshold;
	int mAirGetHitGroundRecoveryGroundGoundLevelY;
	double mAirGetHitAirRecoveryVelocityYThreshold;
	double mAirGetHitAirRecoveryVerticalAcceleration;

	int mAirGetHitTripGroundLevelY;
	Vector3D mBounceOffset;
	double mVerticalBounceAcceleration;
	int mBounceGroundLevel;
	double mLyingDownFrictionThreshold;
} DreamMugenConstantsMovementData;

typedef struct {
	char mVictory[101][1024];
} DreamMugenConstantsQuoteData;

typedef enum {
	MUGEN_STATE_TYPE_UNCHANGED,
	MUGEN_STATE_TYPE_STANDING,
	MUGEN_STATE_TYPE_CROUCHING,
	MUGEN_STATE_TYPE_AIR,
	MUGEN_STATE_TYPE_LYING,

} DreamMugenStateType;

typedef enum {
	MUGEN_STATE_MOVE_TYPE_UNCHANGED,
	MUGEN_STATE_MOVE_TYPE_ATTACK,
	MUGEN_STATE_MOVE_TYPE_IDLE,
	MUGEN_STATE_MOVE_TYPE_BEING_HIT,
} DreamMugenStateMoveType;

typedef enum {
	MUGEN_STATE_PHYSICS_UNCHANGED,
	MUGEN_STATE_PHYSICS_STANDING,
	MUGEN_STATE_PHYSICS_CROUCHING,
	MUGEN_STATE_PHYSICS_AIR,
	MUGEN_STATE_PHYSICS_NONE,

} DreamMugenStatePhysics;

typedef struct {
	Vector mControllers;
	int mID;
	DreamMugenStateType mType;
	DreamMugenStateMoveType mMoveType;
	DreamMugenStatePhysics mPhysics;

	int mIsChangingAnimation;
	DreamMugenAssignment* mAnimation;

	int mIsSettingVelocity;
	DreamMugenAssignment* mVelocity;

	int mIsChangingControl;
	DreamMugenAssignment* mControl;

	int mIsChangingSpritePriority;
	DreamMugenAssignment* mSpritePriority;

	int mIsAddingPower;
	DreamMugenAssignment* mPowerAdd;

	int mDoesRequireJuggle;
	DreamMugenAssignment* mJuggleRequired;

	int mDoHitDefinitionsPersist;
	int mDoMoveHitInfosPersist;
	int mDoesHitCountPersist;

	int mHasFacePlayer2Info;
	DreamMugenAssignment* mDoesFacePlayer2;

	int mHasPriority;
	DreamMugenAssignment* mPriority;
} DreamMugenState;

typedef struct {
	IntMap mStates;
} DreamMugenStates;

typedef struct {
	DreamMugenConstantsHeader mHeader;
	DreamMugenConstantsSizeData mSizeData;
	DreamMugenConstantsVelocityData mVelocityData;
	DreamMugenConstantsMovementData mMovementData;

	DreamMugenStates mStates;
} DreamMugenConstants;

DreamMugenConstants loadDreamMugenConstantsFile(char* tPath);
void loadDreamMugenStateDefinitionsFromFile(DreamMugenStates* tStates, char* tPath);