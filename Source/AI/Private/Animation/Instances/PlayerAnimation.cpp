// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Instances/PlayerAnimation.h"

#include "AnimGraphRuntime/Public/KismetAnimationLibrary.h"
#include "GameFramework/Character.h"
#include "Interfaces/CharacterInterface.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (TryGetPawnOwner())
	{
		Character = Cast<ACharacter>(TryGetPawnOwner());
		if (!Character)
		{
			UE_LOG(LogTemp, Warning, TEXT("Couldn't cast pawn to character in %s"), *GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't get pawn in %s"), *GetName());
	}
}

void UPlayerAnimation::PlayTransition(FDynamicMontageParams Parameters)
{
	PlaySlotAnimationAsDynamicMontage(Parameters.Animation, FName("Grounded Slot"), Parameters.BlendInTime,
	                                  Parameters.BlendOutTime, Parameters.PlayRate, 1, .0f, Parameters.StartTime);
}

void UPlayerAnimation::PlayDynamicTransition(float ReTriggerDelay, FDynamicMontageParams Parameters)
{
	static float DelayTimer = 0;
	if (DelayTimer >= ReTriggerDelay)
	{
		PlaySlotAnimationAsDynamicMontage(Parameters.Animation, FName("Grounded Slot"), Parameters.BlendInTime,
		                                  Parameters.BlendOutTime, Parameters.PlayRate, 1, .0f, Parameters.StartTime);
		DelayTimer = 0;
	}
	DelayTimer += GetWorld()->GetDeltaSeconds();
}

void UPlayerAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	DeltaTimeX = DeltaSeconds;
	if (!(DeltaTimeX != 0)) return;
	if (!Character) return;

	UpdateCharacterInfo();
	UpdateAimingValues();
	UpdateLayerValues();
	UpdateFootIK();
	switch (MovementState)
	{
	case EMovementState::None: break;
	case EMovementState::Grounded:
		bShouldMove = ShouldMoveCheck();
		SetGroundValues(bShouldMove);
		break;
	case EMovementState::InAir:
		UpdateInAirValues();
		break;
	case EMovementState::Mantling: break;
	case EMovementState::Ragdoll:
		UpdateRagdollValues();
		break;
	default: ;
	}
}

void UPlayerAnimation::Jumped()
{
	IAnimationInterface::Jumped();
	bJumped = true;
	JumpPlayRate = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 600.f), FVector2D(1.2f, 1.5f), Speed);
	bJumped = false;
}

void UPlayerAnimation::SetGroundedEntryState(EGroundedEntryState NewGroundedEntryState)
{
	IAnimationInterface::SetGroundedEntryState(NewGroundedEntryState);
	GroundedEntryState = NewGroundedEntryState;
}

void UPlayerAnimation::SetOverlayOverrideState(EMovementType OverlayOverrideState)
{
	IAnimationInterface::SetOverlayOverrideState(OverlayOverrideState);
	MovementType = OverlayOverrideState;
}

void UPlayerAnimation::UpdateWhileMoving()
{
	UpdateMovementValues();
	UpdateRotationValues();
}

void UPlayerAnimation::UpdateWhileNotMoving()
{
	if (CanRotateInPlace())
	{
		RotateInPlaceCheck();
	}
	else
	{
		bRotateL = false;
		bRotateR = false;
	}
	if (CanTurnInPlace())
	{
		TurnInPlaceCheck();
	}
	else
	{
		ElapsedDelayTime = 0.f;
	}
	if (CanTransitionDynamically())
	{
		DynamicTransitionCheck();
	}
}

void UPlayerAnimation::UpdateWhenStartingToMove()
{
	ElapsedDelayTime = 0.f;
	bRotateL = false;
	bRotateR = false;
}

void UPlayerAnimation::UpdateCharacterInfo()
{
	EMovementMode EmptyMode;
	if (Character)
	{
		ICharacterInterface* CharacterInterface = Cast<ICharacterInterface>(Character);
		if (CharacterInterface)
		{
			CharacterInterface->GetEssentialValues(Velocity, Acceleration, MovementInput, bIsMoving, bHasMovementInput,
			                                       Speed, MovementInputAmount, AimingRotation, AimYawRate);
			CharacterInterface->GetCurrentStates(EmptyMode, MovementState, PrevMovementState, MovementAction,
			                                     RotationMode, Gait, Stance, ViewMode, MovementType);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Couldn't cast to character interface in %s"), *GetName());
		}
	}
}

void UPlayerAnimation::UpdateAimingValues()
{
	if (Character)
	{
		SmoothedAimingRotation = FMath::RInterpTo(SmoothedAimingRotation, AimingRotation, DeltaTimeX,
		                                          SmoothedAimingRotationInterpSpeed);
		FRotator NormalizedAimingRotation = AimingRotation - Character->GetActorRotation();
		NormalizedAimingRotation.Normalize();
		AimingAngle = FVector2D(NormalizedAimingRotation.Yaw, NormalizedAimingRotation.Pitch);
		FRotator NormalizedSmoothAimingRotation = SmoothedAimingRotation - Character->GetActorRotation();
		NormalizedSmoothAimingRotation.Normalize();
		SmoothedAimingAngle = FVector2D(NormalizedSmoothAimingRotation.Yaw, NormalizedSmoothAimingRotation.Pitch);
		switch (RotationMode)
		{
		case ERotationMode::VelocityDirection:
			if (bHasMovementInput)
			{
				FRotator NormalizedRotation = MovementInput.Rotation() - Character->GetActorRotation();
				NormalizedRotation.Normalize();
				float ClampedRotation = FMath::GetMappedRangeValueClamped(
					FVector2D(-180.f, 180.f), FVector2D(0.f, 1.f), NormalizedRotation.Yaw);
				InputYawOffsetTime = FMath::FInterpTo(InputYawOffsetTime, ClampedRotation, DeltaTimeX,
				                                      InputYawOffsetInterpSpeed);
			}
			break;
		case ERotationMode::LookingDirection:
			AimSweepTime =
				FMath::GetMappedRangeValueClamped(FVector2D(-90.f, 90.f), FVector2D(1.f, 0.f), AimingAngle.Y);
			SpineRotation = FRotator(0, AimingAngle.X * 0.25f, 0);
			break;
		case ERotationMode::Aiming:
			AimSweepTime =
				FMath::GetMappedRangeValueClamped(FVector2D(-90.f, 90.f), FVector2D(1.f, 0.f), AimingAngle.Y);
			SpineRotation = FRotator(0, AimingAngle.X * 0.25f, 0);
			break;
		}
		LeftYawTime = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 180.f), FVector2D(.5f, 0.f),
		                                                FMath::Abs(SmoothedAimingAngle.X));
		RightYawTime = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 180.f), FVector2D(.5f, 1.f),
		                                                 FMath::Abs(SmoothedAimingAngle.X));
		ForwardYawTime = FMath::GetMappedRangeValueClamped(FVector2D(-180.f, 180.f), FVector2D(0.f, 1.f),
		                                                   SmoothedAimingAngle.X);
	}
}

void UPlayerAnimation::UpdateLayerValues()
{
	EnableAimOffset = FMath::Lerp(1.f, 0.f, GetCurveValue(FName("Mask_AimOffset")));
	BasePoseN = GetCurveValue(FName("BasePose_N"));
	BasePoseCLF = GetCurveValue(FName("BasePose_CLF"));
	SpineAdd = GetCurveValue(FName("Layering_Spine_Add"));
	HeadAdd = GetCurveValue(FName("Layering_Head_Add"));
	ArmL_Add = GetCurveValue(FName("Layering_Arm_L_Add"));
	ArmR_Add = GetCurveValue(FName("Layering_Arm_R_Add"));
	HandR = GetCurveValue(FName("Layering_Hand_R"));
	HandL = GetCurveValue(FName("Layering_Hand_L"));
	EnableHandIKL = FMath::Lerp(0.f, GetCurveValue(FName("Enable_HandIK_L")), GetCurveValue(FName("Layering_Arm_L")));
	EnableHandIKR = FMath::Lerp(0.f, GetCurveValue(FName("Enable_HandIK_R")), GetCurveValue(FName("Layering_Arm_R")));
	ArmL_LS = GetCurveValue(FName("Layering_Arm_L_LS"));
	ArmL_MS = 1 - FMath::Floor(ArmL_LS);
	ArmR_LS = GetCurveValue(FName("Layering_Arm_R_LS"));
	ArmR_MS = 1 - FMath::Floor(ArmR_LS);
}

void UPlayerAnimation::UpdateFootIK()
{
	FVector FootOffset_L_Target;
	FVector FootOffset_R_Target;

	SetFootLocking(FName("Enable_FootIK_L"), FName("FootLock_L"), FName("ik_foot_l"), FootLock_L_Alpha,
	               FootLock_L_Location, FootLock_L_Rotation);
	SetFootLocking(FName("Enable_FootIK_R"), FName("FootLock_R"), FName("ik_foot_r"), FootLock_R_Alpha,
	               FootLock_R_Location, FootLock_R_Rotation);

	switch (MovementState)
	{
	case EMovementState::None:
		SetFootOffsets(FName("Enable_FootIK_L"), FName("ik_foot_l"), FName("root"), FootOffset_L_Target,
		               FootOffset_L_Location, FootOffset_L_Rotation);
		SetFootOffsets(FName("Enable_FootIK_R"), FName("ik_foot_r"), FName("root"), FootOffset_R_Target,
		               FootOffset_R_Location, FootOffset_R_Rotation);
		SetPelvisIKOffset(FootOffset_L_Target, FootOffset_R_Target);
		break;
	case EMovementState::Grounded:
		SetFootOffsets(FName("Enable_FootIK_L"), FName("ik_foot_l"), FName("root"), FootOffset_L_Target,
		               FootOffset_L_Location, FootOffset_L_Rotation);
		SetFootOffsets(FName("Enable_FootIK_R"), FName("ik_foot_r"), FName("root"), FootOffset_R_Target,
		               FootOffset_R_Location, FootOffset_R_Rotation);
		SetPelvisIKOffset(FootOffset_L_Target, FootOffset_R_Target);
		break;
	case EMovementState::InAir:
		SetPelvisIKOffset(FVector::ZeroVector, FVector::ZeroVector);
		ResetIKOffsets();
		break;
	case EMovementState::Mantling:
		SetFootOffsets(FName("Enable_FootIK_L"), FName("ik_foot_l"), FName("root"), FootOffset_L_Target,
		               FootOffset_L_Location, FootOffset_L_Rotation);
		SetFootOffsets(FName("Enable_FootIK_R"), FName("ik_foot_r"), FName("root"), FootOffset_R_Target,
		               FootOffset_R_Location, FootOffset_R_Rotation);
		SetPelvisIKOffset(FootOffset_L_Target, FootOffset_R_Target);
		break;
	case EMovementState::Ragdoll: break;
	}
}

void UPlayerAnimation::UpdateMovementValues()
{
	VelocityBlend = InterpVelocityBlend(VelocityBlend, CalculateVelocityBlend(), VelocityBlendInterpSpeed, DeltaTimeX);
	DiagonalScaleAmount = CalculateDiagonalScaleAmount();
	RelativeAccelerationAmount = CalculateRelativeAccelerationAmount();
	FLeanAmount TargetLeanAmount;
	TargetLeanAmount.LR = RelativeAccelerationAmount.Y;
	TargetLeanAmount.FB = RelativeAccelerationAmount.X;
	LeanAmount = InterpLeanAmount(LeanAmount, TargetLeanAmount, GroundedLeanInterpSpeed, DeltaTimeX);
	WalkRunBlend = CalculateWalkRunBlend();
	StrideBlend = CalculateStrideBlend();
	StandingPlayRate = CalculateStandingPlayRate();
	CrouchingPlayRate = CalculateCrouchingPlayRate();
}

void UPlayerAnimation::UpdateRotationValues()
{
	MovementDirection = CalculateMovementDirection();
	FRotator NormalizedRotation = Velocity.Rotation() - Character->GetControlRotation();
	NormalizedRotation.Normalize();
	FVector FBValue = YawOffset_FB->GetVectorValue(NormalizedRotation.Yaw);
	FYaw = FBValue.X;
	BYaw = FBValue.Y;
	FVector LRValue = YawOffset_FB->GetVectorValue(NormalizedRotation.Yaw);
	LYaw = LRValue.X;
	RYaw = LRValue.Y;
}

void UPlayerAnimation::UpdateInAirValues()
{
	FallSpeed = Velocity.Z;
	LandPrediction = CalculateLandPrediction();
	LeanAmount = InterpLeanAmount(LeanAmount, CalculateInAirLeanAmount(), InAirLeanInterpSpeed, DeltaTimeX);
}

void UPlayerAnimation::UpdateRagdollValues()
{
	float PhysicsLinearVelocityLength = GetOwningComponent()->GetPhysicsLinearVelocity().Size();
	FlailRate = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 1000.f), FVector2D(0.f, 1.f),
	                                              PhysicsLinearVelocityLength);
}

bool UPlayerAnimation::ShouldMoveCheck()
{
	return (bIsMoving && bHasMovementInput) || (Speed > 150.f);
}

bool UPlayerAnimation::CanTurnInPlace()
{
	return RotationMode == ERotationMode::LookingDirection && ViewMode == EViewMode::ThirdPerson && (GetCurveValue(
		FName("Enable_Transition")) > 0.99f);
}

bool UPlayerAnimation::CanRotateInPlace()
{
	return RotationMode == ERotationMode::Aiming || ViewMode == EViewMode::FirstPerson;
}

bool UPlayerAnimation::CanTransitionDynamically()
{
	return GetCurveValue(FName("Enable_Transition")) == 1.f;
}

bool UPlayerAnimation::CanTransitionMovementType()
{
	return Stance == EStance::Standing && !bShouldMove;
}

void UPlayerAnimation::TurnInPlace(FRotator Rotation, float NewPlayRateScale, float NewStartTime, bool OverrideCurrent)
{
	FTurnInPlaceAsset TargetTurnAsset;
	//Set Turn Angle
	FRotator TurnAngleNormalized = Rotation - Character->GetActorRotation();
	TurnAngleNormalized.Normalize();
	float TurnAngle = TurnAngleNormalized.Yaw;
	//Choose Turn Asset based on turn angle and stance
	if (FMath::Abs(TurnAngle) < Turn180Threshold)
	{
		if (TurnAngle < 0.f)
		{
			switch (Stance)
			{
			case EStance::Standing:
				TargetTurnAsset = N_TurnIP_L90;
				break;
			case EStance::Crouching:
				TargetTurnAsset = CLF_TurnIP_L90;
				break;
			}
		}
		else
		{
			switch (Stance)
			{
			case EStance::Standing:
				TargetTurnAsset = N_TurnIP_R90;
				break;
			case EStance::Crouching:
				TargetTurnAsset = CLF_TurnIP_R90;
				break;
			}
		}
	}
	else
	{
		if (TurnAngle < 0.f)
		{
			switch (Stance)
			{
			case EStance::Standing:
				TargetTurnAsset = N_TurnIP_L180;
				break;
			case EStance::Crouching:
				TargetTurnAsset = CLF_TurnIP_L180;
				break;
			}
		}
		else
		{
			switch (Stance)
			{
			case EStance::Standing:
				TargetTurnAsset = N_TurnIP_R180;
				break;
			case EStance::Crouching:
				TargetTurnAsset = CLF_TurnIP_R180;
				break;
			}
		}
	}
	//If Target Turn Animation is not playing or set to be overriden, play the turn animation as a dynamic montage
	if (TargetTurnAsset.Animation != nullptr)
	{
		if (OverrideCurrent || IsPlayingSlotAnimation(TargetTurnAsset.Animation, TargetTurnAsset.SlotName))
		{
			PlaySlotAnimationAsDynamicMontage(TargetTurnAsset.Animation, TargetTurnAsset.SlotName, .2f, .2f,
			                                  TargetTurnAsset.PlayRate, 1, .0f, NewStartTime);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No valid animation in TurnInPlace function"));
	}
	// Scale the rotation amount to compensate for turn angle and play rate
	if (TargetTurnAsset.bScaleTurnAngle)
	{
		RotationScale = (TurnAngle / TargetTurnAsset.AnimatedAngle) * TargetTurnAsset.PlayRate * NewPlayRateScale;
	}
	else
	{
		RotationScale = TargetTurnAsset.PlayRate * NewPlayRateScale;
	}
}

void UPlayerAnimation::TurnInPlaceCheck()
{
	//Check if aiming angle is outside of the turn check min angle and if the yaw rate is below the aim yaw rate limit
	//if so, begin counting the elapsed delay time, else reset the elapsed delay time
	if (FMath::Abs(AimingAngle.X) > TurnCheckMinAngle && AimYawRate < AimYawRateLimit)
	{
		ElapsedDelayTime += DeltaTimeX;
		//check if the elapsed delay time exceeds the set delay, if so, trigger a turn in placeç
		if (ElapsedDelayTime > FMath::GetMappedRangeValueClamped(FVector2D(TurnCheckMinAngle, 100.f),
		                                                         FVector2D(MinAngleDelay, MaxAngleDelay),
		                                                         FMath::Abs(AimingAngle.X)))
		{
			TurnInPlace(FRotator(0.f, AimingRotation.Yaw, 0.f), 1.0f, 0.f, false);
		}
	}
	else
	{
		ElapsedDelayTime = 0;
	}
}

void UPlayerAnimation::RotateInPlaceCheck()
{
	//Check if the character should rotate left or right by checking if the aiming angle exceeds the threshold
	bRotateL = AimingAngle.X < RotateMinThreshold;
	bRotateR = AimingAngle.X > RotateMaxThreshold;
	//if the character should be rotating, set the rotate rate to scale with the aim yaw rate, so the character rotate faster when moving the camera faster
	if (bRotateR || bRotateL)
	{
		RotateRate = FMath::GetMappedRangeValueClamped(FVector2D(AimYawRateMinRange, AimYawRateMaxRange),
		                                               FVector2D(MinPlayRate, MaxPlayRate), AimYawRate);
	}
}

void UPlayerAnimation::DynamicTransitionCheck()
{
	// Check each foot to see if the location difference between the IK_Foot bone and its desired / target location exceeds a threshold, if it does, play an additive transition animation on that foot.
	//The currently set transition plays the second half of a 2 foot transition animation, so that only a single foot moves Because only the IK_Foot bone can be locked, the separate virtual bone allows the system to know its desired location when lockedç
	float DistanceBetweenLSockets = UKismetAnimationLibrary::K2_DistanceBetweenTwoSocketsAndMapRange(
		GetOwningComponent(), FName("ik_foot_l"), ERelativeTransformSpace::RTS_Component, FName("VB foot_target_l"),
		ERelativeTransformSpace::RTS_Component, false, 0, 0, 0, 0);
	float DistanceBetweenRSockets = UKismetAnimationLibrary::K2_DistanceBetweenTwoSocketsAndMapRange(
		GetOwningComponent(), FName("ik_foot_r"), ERelativeTransformSpace::RTS_Component, FName("VB foot_target_r"),
		ERelativeTransformSpace::RTS_Component, false, 0, 0, 0, 0);
	
	FDynamicMontageParams RightTransitionParams;
	RightTransitionParams.Animation = RightTransition;
	RightTransitionParams.BlendInTime = .2f;
	RightTransitionParams.BlendOutTime = .2f;
	RightTransitionParams.PlayRate = 1.5f;
	RightTransitionParams.StartTime = .8f;
	
	FDynamicMontageParams LeftTransitionParams;
	LeftTransitionParams.Animation = LeftTransition;
	LeftTransitionParams.BlendInTime = .2f;
	LeftTransitionParams.BlendOutTime = .2f;
	LeftTransitionParams.PlayRate = 1.5f;
	LeftTransitionParams.StartTime = .8f;
	
	FDynamicMontageParams LeftTransition;
	if (DistanceBetweenLSockets > 8.f)
	{
		PlayDynamicTransition(.1f,  RightTransitionParams);
	}
	if (DistanceBetweenRSockets > 8.f)
	{
		PlayDynamicTransition(.1f,  LeftTransitionParams);
	}

	
}

FVelocityBlend UPlayerAnimation::CalculateVelocityBlend()
{
	FVector NormalizedVelocity = Velocity;
	NormalizedVelocity.Normalize(0.1f);
	FRotator  ActorRotation = Character->GetActorRotation();
	FVector LocationRelativeVelocityDirection = ActorRotation.UnrotateVector(NormalizedVelocity);
	float Sum = FMath::Abs(LocationRelativeVelocityDirection.X) + FMath::Abs(LocationRelativeVelocityDirection.Y) + FMath::Abs(LocationRelativeVelocityDirection.Z);
	FVector RelativeDirection = LocationRelativeVelocityDirection / Sum;
	FVelocityBlend NewVelocityBlend;
	NewVelocityBlend.F = FMath::Clamp(RelativeDirection.X, 0.f,1.f);
	NewVelocityBlend.B = FMath::Abs(FMath::Clamp(RelativeDirection.X, -1.f,0.f));
	NewVelocityBlend.L = FMath::Abs(FMath::Clamp(RelativeDirection.Y, -1.f,0.f));
	NewVelocityBlend.R = FMath::Clamp(RelativeDirection.Y, 0.f,1.f);
	return NewVelocityBlend;
}
