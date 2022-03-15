// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterData/CharacterDataStructs.h"
#include "Interfaces/AnimationInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerAnimation.generated.h"

/**
 * 
 */
UCLASS()
class AI_API UPlayerAnimation : public UAnimInstance, IAnimationInterface
{
	GENERATED_BODY()
protected:
#pragma region References
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	ACharacter* Character = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	float DeltaTimeX = 0.f;
#pragma endregion References

#pragma region CharacterInfo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	FRotator AimingRotation = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	FVector Velocity = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	FVector RelativeVelocityDirection = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	FVector Acceleration = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	FVector MovementInput = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	bool bIsMoving = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	bool bHasMovementInput = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	float Speed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	float MovementInputAmount = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	float AimYawRate = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	float ZoomAmount = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	EMovementState MovementState = EMovementState::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	EMovementState PrevMovementState = EMovementState::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	EMovementAction MovementAction = EMovementAction::LowMantle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	ERotationMode RotationMode = ERotationMode::VelocityDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	EGait Gait = EGait::Walking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	EStance Stance = EStance::Standing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	EViewMode ViewMode = EViewMode::ThirdPerson;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	EMovementType MovementType = EMovementType::Default;
#pragma endregion CharacterInfo

#pragma region Grounded
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	EGroundedEntryState GroundedEntryState = EGroundedEntryState::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	EMovementDirection MovementDirection = EMovementDirection::Forward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	EHipsDirection TrackedHipsDirection = EHipsDirection::F;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	FVector RelativeAccelerationAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	bool bShouldMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	bool bRotateL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	bool bRotateR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	bool bPivot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	float RotateRate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	float RotationScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	float DiagonalScaleAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	float WalkRunBlend;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	float StandingPlayRate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	float CrouchingPlayRate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	float StrideBlend;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	FVelocityBlend VelocityBlend;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	FLeanAmount LeanAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	float FYaw = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	float BYaw = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	float LYaw = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground")
	float RYaw = 0.f;
#pragma endregion Grounded

#pragma region InAir
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InAir")
	bool bJumped;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InAir")
	float JumpPlayRate = 1.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InAir")
	float FallSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InAir")
	float LandPrediction = 1.f;
#pragma endregion InAir

#pragma region AimingValues
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimingValues")
	FRotator SmoothedAimingRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimingValues")
	FRotator SpineRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimingValues")
	FVector2D AimingAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimingValues")
	FVector2D SmoothedAimingAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimingValues")
	float AimSweepTime = .5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimingValues")
	float InputYawOffsetTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimingValues")
	float ForwardYawTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimingValues")
	float LeftYawTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimingValues")
	float RightYawTime;
#pragma endregion AimingValues

#pragma region Ragdoll
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ragdoll")
	float FlailRate;
#pragma endregion Ragdoll

#pragma region LayerBlending
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	int32 OverlayOverrideState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float EnableAimOffset = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float BasePoseN = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float BasePoseCLF;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float ArmL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float ArmL_Add;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float ArmL_LS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float ArmL_MS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float ArmR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float ArmR_Add;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float ArmR_LS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float ArmR_MS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float HandL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float HandR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float Legs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float LegsAdd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float Pelvis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float PelvisAdd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float Spine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float SpineAdd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float Head;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float HeadAdd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float EnableHandIKL = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LayerBlending")
	float EnableHandIKR = 1.f;
#pragma endregion LayerBlending

#pragma region FootIK
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foot IK")
	float FootLock_L_Alpha = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foot IK")
	float FootLock_R_Alpha = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foot IK")
	FVector FootLock_L_Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foot IK")
	FVector FootLock_R_Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foot IK")
	FRotator FootLock_L_Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foot IK")
	FRotator FootLock_R_Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foot IK")
	FVector FootOffset_L_Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foot IK")
	FVector FootOffset_R_Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foot IK")
	FRotator FootOffset_L_Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foot IK")
	FRotator FootOffset_R_Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foot IK")
	FVector PelvisOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foot IK")
	float PelvisAlpha;
#pragma endregion FootIK

#pragma region TurnInPlace
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn In Place")
	float TurnCheckMinAngle=45.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn In Place")
	float Turn180Threshold = 130.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn In Place")
	float AimYawRateLimit = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn In Place")
	float ElapsedDelayTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn In Place")
	float MinAngleDelay = .75f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn In Place")
	float MaxAngleDelay = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn In Place")
	FTurnInPlaceAsset N_TurnIP_L90;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn In Place")
	FTurnInPlaceAsset N_TurnIP_R90;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn In Place")
	FTurnInPlaceAsset N_TurnIP_L180;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn In Place")
	FTurnInPlaceAsset N_TurnIP_R180;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn In Place")
	FTurnInPlaceAsset CLF_TurnIP_L90;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn In Place")
	FTurnInPlaceAsset CLF_TurnIP_R90;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn In Place")
	FTurnInPlaceAsset CLF_TurnIP_L180;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn In Place")
	FTurnInPlaceAsset CLF_TurnIP_R180;
#pragma endregion TurnInPlace

#pragma region RotateInPlace
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate In Place")
	float RotateMinThreshold = -50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate In Place")
	float RotateMaxThreshold = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate In Place")
	float AimYawRateMinRange = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate In Place")
	float AimYawRateMaxRange = 270.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate In Place")
	float MinPlayRate = 1.15f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate In Place")
	float MaxPlayRate = 3.f;
#pragma endregion RotateInPlace
	
#pragma region BlendCurves
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blend Curves")
	UCurveFloat* DiagonalScaleAmountCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlendCurves")
	UCurveFloat* StrideBlend_N_Walk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlendCurves")
	UCurveFloat* StrideBlend_N_Run;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlendCurves")
	UCurveFloat* StrideBlend_C_Walk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlendCurves")
	UCurveFloat* LandPredictionCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlendCurves")
	UCurveFloat* LeanInAirCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlendCurves")
	UCurveVector* YawOffset_FB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlendCurves")
	UCurveVector* YawOffset_LR;
#pragma endregion BlendCurves

#pragma region Config
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float AnimatedWalkSpeed = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float AnimatedRunSpeed = 350.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float AnimatedSprintSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float AnimatedCrouchSpeed = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float VelocityBlendInterpSpeed = 12.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float GroundedLeanInterpSpeed = 4.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float InAirLeanInterpSpeed = 4.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float SmoothedAimingRotationInterpSpeed = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float InputYawOffsetInterpSpeed = 8.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float TriggerPivotSpeedLimit = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float FootHeight = 13.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float IK_TraceDistanceAboveFoot = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float IK_TraceDistanceBelowFoot = 45.f;
#pragma endregion Config

protected:
#pragma region  Events
	virtual void NativeInitializeAnimation() override;
	void PlayTransition(FDynamicMontageParams Parameters);
	void PlayDynamicTransition(float ReTriggerDelay, FDynamicMontageParams Parameters);
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void Jumped() override;
	virtual void SetGroundedEntryState(EGroundedEntryState NewGroundedEntryState) override;
	virtual void SetOverlayOverrideState(EMovementType OverlayOverrideState) override;
	UFUNCTION(BlueprintImplementableEvent)
	void SetGroundValues(bool ValueToEvaluate);
	UFUNCTION(BlueprintCallable)
	void UpdateWhileMoving();
	UFUNCTION(BlueprintCallable)
	void UpdateWhileNotMoving();
	UFUNCTION()
	void UpdateWhenStartingToMove();
	
	
#pragma endregion Events
	
public:
#pragma region Functions
	UFUNCTION(BlueprintCallable, Category = "Update Values")
	void UpdateCharacterInfo();
	UFUNCTION(BlueprintCallable, Category = "Update Values")
	void UpdateAimingValues();
	UFUNCTION(BlueprintCallable, Category = "Update Values")
	void UpdateLayerValues();
	UFUNCTION(BlueprintCallable, Category = "Update Values")
	void UpdateFootIK();
	UFUNCTION(BlueprintCallable, Category = "Update Values")
	void UpdateMovementValues();
	UFUNCTION(BlueprintCallable, Category = "Update Values")
	void UpdateRotationValues();
	UFUNCTION(BlueprintCallable, Category = "Update Values")
	void UpdateInAirValues();
	UFUNCTION(BlueprintCallable, Category = "Update Values")
	void UpdateRagdollValues();

	UFUNCTION(BlueprintCallable, Category = "Grounded")
	bool ShouldMoveCheck();
	UFUNCTION(BlueprintCallable, Category = "Grounded")
	bool CanTurnInPlace();
	UFUNCTION(BlueprintCallable, Category = "Grounded")
	bool CanRotateInPlace();
	UFUNCTION(BlueprintCallable, Category = "Grounded")
	bool CanTransitionDynamically();
	UFUNCTION(BlueprintCallable, Category = "Grounded")
	bool CanTransitionMovementType();
	UFUNCTION(BlueprintCallable, Category = "Grounded")
	void TurnInPlace(FRotator Rotation, float NewPlayRateScale, float NewStartTime, bool OverrideCurrent);
	UFUNCTION(BlueprintCallable, Category = "Grounded")
	void TurnInPlaceCheck();
	UFUNCTION(BlueprintCallable, Category = "Grounded")
	void RotateInPlaceCheck();
	UFUNCTION(BlueprintCallable, Category = "Grounded")
	void DynamicTransitionCheck();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FVelocityBlend CalculateVelocityBlend();
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float CalculateDiagonalScaleAmount();
	UFUNCTION(BlueprintCallable, Category = "Movement")
	FVector CalculateRelativeAccelerationAmount();
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float CalculateWalkRunBlend();
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float CalculateStrideBlend();
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float CalculateStandingPlayRate();
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float CalculateCrouchingPlayRate();

	UFUNCTION(BlueprintCallable, Category = "In Air")
	float CalculateLandPrediction();
	UFUNCTION(BlueprintCallable, Category = "In Air")
	FLeanAmount CalculateInAirLeanAmount();

	UFUNCTION(BlueprintCallable, Category = "Foot IK")
	void SetFootOffsets(FName Enable_FootIK_Curve,  FName IKFootBone, FName RootBone, FVector CurrentLocationTarget, FVector CurrentLocationOffset, FRotator CurrentRotationOffset);
	UFUNCTION(BlueprintCallable, Category = "Foot IK")
	void SetPelvisIKOffset(FVector FootOffset_L_Target, FVector FootOffset_R_Target);
	UFUNCTION(BlueprintCallable, Category = "Foot IK")
	void SetFootLocking(FName Enable_FootIK_Curve, FName FootLockCurve, FName IKFootBone, float CurrentFootLockAlpha, FVector CurrentFootLockVector, FRotator CurrentFootLocKRotation);
	UFUNCTION(BlueprintCallable, Category = "Foot IK")
	void SetFootLockOffsets(FVector LocalLocation, FRotator LocalRotation);
	
	UFUNCTION(BlueprintCallable, Category = "Rotation")
	EMovementDirection CalculateMovementDirection();
	UFUNCTION(BlueprintCallable, Category = "Rotation")
	EMovementDirection CalculateQuadrant(EMovementDirection Current, float FRThreshold, float FLThreshold, float BRThreshold, float BLThreshold, float Buffer, float Angle);
	UFUNCTION(BlueprintCallable, Category = "Rotation")
	bool AngleInRange(float Angle, float MinAngle, float MaxAngle, float Buffer, bool IncreaseBuffer);

	UFUNCTION(BlueprintCallable, Category = "Interpolation")
	FVelocityBlend InterpVelocityBlend(FVelocityBlend Current, FVelocityBlend Target, float InterpSpeed, float DeltaTime);
	UFUNCTION(BlueprintCallable, Category = "Interpolation")
	FLeanAmount InterpLeanAmount(FLeanAmount Current, FLeanAmount Target, float InterpSpeed, float DeltaTime);
	
	UFUNCTION(BlueprintCallable, Category = "Debug")
	EDrawDebugTrace::Type GetDebugTraceType(EDrawDebugTrace::Type ShowTraceType);
	
	UFUNCTION(BlueprintCallable)
	void ResetIKOffsets();
#pragma endregion Functions


};
