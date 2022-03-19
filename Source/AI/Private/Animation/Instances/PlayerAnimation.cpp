// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Instances/PlayerAnimation.h"
#include "KismetAnimationLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/CharacterInterface.h"
#include "Interfaces/ControllerInterface.h"
#include "Kismet/GameplayStatics.h"
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

void UPlayerAnimation::SetOverlayOverrideState(EMovementType NewOverlayOverrideState)
{
	IAnimationInterface::SetOverlayOverrideState(NewOverlayOverrideState);
	MovementType = NewOverlayOverrideState;
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

	if (DistanceBetweenLSockets > 8.f)
	{
		PlayDynamicTransition(.1f, RightTransitionParams);
	}
	if (DistanceBetweenRSockets > 8.f)
	{
		PlayDynamicTransition(.1f, LeftTransitionParams);
	}
}

FVelocityBlend UPlayerAnimation::CalculateVelocityBlend()
{
	FVector NormalizedVelocity = Velocity;
	NormalizedVelocity.Normalize(0.1f);
	FRotator ActorRotation = Character->GetActorRotation();
	FVector LocationRelativeVelocityDirection = ActorRotation.UnrotateVector(NormalizedVelocity);
	float Sum = FMath::Abs(LocationRelativeVelocityDirection.X) + FMath::Abs(LocationRelativeVelocityDirection.Y) +
		FMath::Abs(LocationRelativeVelocityDirection.Z);
	FVector RelativeDirection = LocationRelativeVelocityDirection / Sum;
	FVelocityBlend NewVelocityBlend;
	NewVelocityBlend.F = FMath::Clamp(RelativeDirection.X, 0.f, 1.f);
	NewVelocityBlend.B = FMath::Abs(FMath::Clamp(RelativeDirection.X, -1.f, 0.f));
	NewVelocityBlend.L = FMath::Abs(FMath::Clamp(RelativeDirection.Y, -1.f, 0.f));
	NewVelocityBlend.R = FMath::Clamp(RelativeDirection.Y, 0.f, 1.f);
	return NewVelocityBlend;
}

float UPlayerAnimation::CalculateDiagonalScaleAmount()
{
	/* Calculate the Diagnal Scale Amount. This value is used to scale the Foot IK Root bone to make the Foot IK bones cover more distance on the diagonal blends.
	* Without scaling, the feet would not move far enough on the diagonal direction due to the linear translational blending of the IK bones.
	* The curve is used to easily map the value.*/
	if (!Character) return 0.f;

	float ScaleAmount = DiagonalScaleAmountCurve->GetFloatValue(FMath::Abs(VelocityBlend.F + VelocityBlend.B));
	return ScaleAmount;
}

FVector UPlayerAnimation::CalculateRelativeAccelerationAmount()
{
	//Calculate the Relative Acceleration Amount.
	//This value represents the current amount of acceleration / deceleration relative to the actor rotation. It is normalized to a range of -1 to 1 so that -1 equals the Max Braking Deceleration, and 1 equals the Max Acceleration of the Character Movement Component.
	if (!Character) return FVector::ZeroVector;

	FVector ClampedAcceleration;
	FRotator CharacterRotation;
	FVector FinalAcceleration;

	if (FVector::DotProduct(Acceleration, Velocity) > 0.f)
	{
		ClampedAcceleration = Acceleration.GetClampedToMaxSize(Character->GetCharacterMovement()->GetMaxAcceleration())
			/ Character->GetCharacterMovement()->GetMaxAcceleration();
		CharacterRotation = Character->GetActorRotation();
		FinalAcceleration = CharacterRotation.UnrotateVector(ClampedAcceleration);
	}
	else
	{
		ClampedAcceleration = Acceleration.GetClampedToMaxSize(
				Character->GetCharacterMovement()->GetMaxBrakingDeceleration()) / Character->GetCharacterMovement()->
			GetMaxBrakingDeceleration();
		CharacterRotation = Character->GetActorRotation();
		FinalAcceleration = CharacterRotation.UnrotateVector(ClampedAcceleration);
	}
	return FinalAcceleration;
}

float UPlayerAnimation::CalculateWalkRunBlend()
{
	//Calculate the Walk Run Blend. This value is used within the Blendspaces to blend between walking and running.
	switch (Gait)
	{
	case EGait::Running:
		return 1.f;
	case EGait::Sprinting:
		return 1.f;
	case EGait::Walking:
		return 0.f;
	default:
		return 0.f;
		break;
	}
}

float UPlayerAnimation::CalculateStrideBlend()
{
	//Calculate the Stride Blend.
	//This value is used within the blendspaces to scale the stride (distance feet travel) so that the character can walk or run at different movement speeds.
	//It also allows the walk or run gait animations to blend independently while still matching the animation speed to the movement speed, preventing the character from needing to play a half walk+half run blend.
	//The curves are used to map the stride amount to the speed for maximum control.
	float BlendedWalkStride = 0.f;
	float BlendedRunStride = 0.f;
	float LerpedBlend = FMath::Lerp(BlendedWalkStride, BlendedRunStride,
	                                GetAnimCurveClamped(FName("Weight_Gait"), -1.f, 0.f, 1.f));
	float FinalResult = FMath::Lerp(LerpedBlend, StrideBlend_C_Walk->GetFloatValue(Speed),
	                                GetCurveValue(FName("BasePose_CLF")));

	return FinalResult;
}

float UPlayerAnimation::CalculateStandingPlayRate()
{
	//Calculate the Play Rate by dividing the Character's speed by the Animated Speed for each gait.
	//The lerps are determined by the "Weight_Gait" anim curve that exists on every locomotion cycle so that the play rate is always in sync with the currently blended animation.
	//The value is also divided by the Stride Blend and the mesh scale so that the play rate increases as the stride or scale gets smaller.

	float WalkSpeed = Speed / AnimatedWalkSpeed;
	float RunSpeed = Speed / AnimatedRunSpeed;
	float SprintSpeed = Speed / AnimatedSprintSpeed;

	float WalkRun = FMath::Lerp(WalkSpeed, RunSpeed, GetAnimCurveClamped(FName("Weight_Gait"), -1.f, 0.f, 1.f));
	float MoveSprintBlend = FMath::Lerp(WalkRun, SprintSpeed,
	                                    GetAnimCurveClamped(FName("Weight_Gait"), -2.f, 0.f, 1.f));

	float Blend = (MoveSprintBlend / StrideBlend) / GetOwningComponent()->GetComponentScale().Z;
	float StandingPlayRateValue = FMath::Clamp(Blend, 0.f, 3.f);

	return StandingPlayRateValue;
}

float UPlayerAnimation::CalculateCrouchingPlayRate()
{
	float CrouchSpeed = Speed / AnimatedCrouchSpeed;
	float CrouchStride = CrouchSpeed / StrideBlend;
	float Blend = CrouchStride / GetOwningComponent()->GetComponentScale().Z;
	float CrouchPlayRate = FMath::Clamp(Blend, 0.f, 2.f);
	return CrouchPlayRate;
}

float UPlayerAnimation::CalculateLandPrediction()
{
	//Calculate the land prediction weight by tracing in the velocity direction to find a walkable surface the character is falling toward, and getting the 'Time' (range of 0-1, 1 being maximum, 0 being about to land) till impact.
	//The Land Prediction Curve is used to control how the time affects the final weight for a smooth blend.

	if (!Character || !(FallSpeed < -200.f)) return 0.f;

	FVector Start = Character->GetCapsuleComponent()->GetComponentLocation();
	FVector EndOffset = FVector(Velocity.X, Velocity.Y, FMath::Clamp(Velocity.Z, -4000.f, -200.f));
	float ClampedVelocityInZ = FMath::GetMappedRangeValueClamped(FVector2D(0, -4000.f), FVector2D(50.f, 2000.f),
	                                                             Velocity.Z);
	FVector End = Start + (EndOffset * ClampedVelocityInZ);
	float Radius = Character->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	float HalfHeight = Character->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	FHitResult Hit;
	bool bTraceResult = GetWorld()->SweepSingleByProfile(Hit, Start, End, FQuat::Identity, FName("ALS_Character"),
	                                                     FCollisionShape::MakeCapsule(Radius, HalfHeight));
	bool bIsWalkable = Character->GetCharacterMovement()->IsWalkable(Hit);
	bool bIsHitBlocked = Hit.bBlockingHit;
	if (bIsWalkable && bIsHitBlocked)
	{
		float LandPredictionCurveValue = LandPredictionCurve->GetFloatValue(Hit.Time);
		float LandPredictionValue = FMath::Lerp(LandPredictionCurveValue, 0.f,
		                                        GetCurveValue(FName("Mask_LandPrediction")));

		return LandPredictionValue;
	}

	return 0.f;
}

FLeanAmount UPlayerAnimation::CalculateInAirLeanAmount()
{
	//Use the relative Velocity direction and amount to determine how much the character should lean while in air.
	//The Lean In Air curve gets the Fall Speed and is used as a multiplier to smoothly reverse the leaning direction when transitioning from moving upwards to moving downwards.
	FRotator CharacterRotation = Character->GetActorRotation();
	FVector UnrotatedVelocity = CharacterRotation.UnrotateVector(Velocity) / 350.f;
	FVector2D Lean = FVector2D(UnrotatedVelocity.Y, UnrotatedVelocity.X);
	float FallValue = LeanInAirCurve->GetFloatValue(FallSpeed);

	FLeanAmount NewLean;
	NewLean.LR = (Lean * FallValue).X;
	NewLean.FB = (Lean * FallValue).Y;

	return NewLean;
}

void UPlayerAnimation::SetFootOffsets(FName Enable_FootIK_Curve, FName IKFootBone, FName RootBone,
                                      FVector& CurrentLocationTarget, FVector& CurrentLocationOffset,
                                      FRotator& CurrentRotationOffset)
{
	//Only update Foot IK offset values if the Foot IK curve has a weight. If it equals 0, clear the offset values.
	if (GetCurveValue(Enable_FootIK_Curve) > 0.f)
	{
		//Step 1: Trace downward from the foot location to find the geometry. If the surface is walkable, save the Impact Location and Normal.
		FVector FootSocketLocation = GetOwningComponent()->GetSocketLocation(IKFootBone);
		FVector RootSocketLocation = GetOwningComponent()->GetSocketLocation(RootBone);
		FVector IKFootFloorLocation = FVector(FootSocketLocation.X, FootSocketLocation.Y, RootSocketLocation.Z);
		FVector ImpactPoint;
		FVector ImpactNormal;
		FRotator TargetRotationOffset;

		FVector Start = IKFootFloorLocation + FVector(0.f, 0.f, IK_TraceDistanceAboveFoot);
		FVector End = IKFootFloorLocation + FVector(0.f, 0.f, IK_TraceDistanceBelowFoot);
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);
		bool bIsWalkable = Character->GetCharacterMovement()->IsWalkable(Hit);
		if (bIsWalkable)
		{
			ImpactPoint = Hit.ImpactPoint;
			ImpactNormal = Hit.ImpactNormal;
			//Step 1.1: Find the difference in location from the Impact point and the expected (flat) floor location.
			//These values are offset by the nomrmal multiplied by the foot height to get better behavior on angled surfaces.
			CurrentLocationTarget = (ImpactNormal + (ImpactNormal * FootHeight)) - (IKFootFloorLocation + (
				FVector(0.f, 0.f, 1.f) * FootHeight));
			//Step 1.2: Calculate the Rotation offset by getting the Atan2 of the Impact Normal.
			TargetRotationOffset = FRotator(FMath::Atan2(ImpactNormal.X, ImpactNormal.Y) * -1.f, 0.f,
			                                FMath::Atan2(ImpactNormal.Y, ImpactNormal.Z));
		}
		//Step 2: Interp the Current Location Offset to the new target value. Interpolate at different speeds based on whether the new target is above or below the current one.
		if (CurrentLocationOffset.Z > CurrentLocationTarget.Z)
		{
			CurrentLocationOffset = FMath::VInterpTo(CurrentLocationOffset, CurrentLocationTarget, DeltaTimeX, 30.f);
		}
		else
		{
			CurrentLocationOffset = FMath::VInterpTo(CurrentLocationOffset, CurrentLocationTarget, DeltaTimeX, 15.f);
		}
		//Step 3: Interp the Current Rotation Offset to the new target value.
		CurrentRotationOffset = FMath::RInterpTo(CurrentRotationOffset, TargetRotationOffset, DeltaTimeX, 30.f);
	}
	else
	{
		CurrentLocationOffset = FVector::ZeroVector;
		CurrentRotationOffset = FRotator::ZeroRotator;
	}
}

void UPlayerAnimation::SetPelvisIKOffset(FVector FootOffset_L_Target, FVector FootOffset_R_Target)
{
	//Calculate the Pelvis Alpha by finding the average Foot IK weight. If the alpha is 0, clear the offset.
	FVector PelvisTarget;
	PelvisAlpha = (GetCurveValue(FName("Enable_FootIK_L")) + GetCurveValue(FName("Enable_FootIK_R"))) / 2.f;
	if (!(PelvisAlpha > 0.f)) PelvisOffset = FVector::ZeroVector;

	//Step 1: Set the new Pelvis Target to be the lowest Foot Offset
	if (FootOffset_L_Target.Z < FootOffset_R_Target.Z)
	{
		PelvisTarget = FootOffset_L_Target;
	}
	else
	{
		PelvisTarget = FootOffset_R_Target;
	}

	//Step 2: Interp the Current Pelvis Offset to the new target value.
	//Interpolate at different speeds based on whether the new target is above or below the current one.
	if (PelvisTarget.Z > PelvisOffset.Z)
	{
		PelvisOffset = FMath::VInterpTo(PelvisOffset, PelvisTarget, DeltaTimeX, 10.f);
	}
	else
	{
		PelvisOffset = FMath::VInterpTo(PelvisOffset, PelvisTarget, DeltaTimeX, 15.f);
	}
}

void UPlayerAnimation::SetFootLocking(FName Enable_FootIK_Curve, FName FootLockCurve, FName IKFootBone,
                                      float& CurrentFootLockAlpha, FVector& CurrentFootLockLocation,
                                      FRotator& CurrentFootLockRotation)
{
	//Only update values if FootIK curve has a weight.
	if (!(GetCurveValue(Enable_FootIK_Curve) > 0)) return;
	//Step 1: Set Local FootLock Curve value
	float FootLockCurveValue = GetCurveValue(FootLockCurve);
	//Step 2: Only update the FootLock Alpha if the new value is less than the current, or it equals 1.
	//This makes it so that the foot can only blend out of the locked position or lock to a new position, and never blend in.
	if (FootLockCurveValue >= .99f || FootLockCurveValue < CurrentFootLockAlpha)
	{
		CurrentFootLockAlpha = FootLockCurveValue;
	}
	//Step 3: If the Foot Lock curve equals 1, save the new lock location and rotation in component space.
	if (FootLockCurveValue >= .99)
	{
		FTransform FootBoneSocket = GetOwningComponent()->GetSocketTransform(IKFootBone, RTS_Component);
		CurrentFootLockLocation = FootBoneSocket.GetLocation();
		CurrentFootLockRotation = FootBoneSocket.GetRotation().Rotator();
	}
	//Step 4: If the Foot Lock Alpha has a weight, update the Foot Lock offsets to keep the foot planted in place while the capsule moves.
	if (FootLockCurveValue > 0.f)
	{
		SetFootLockOffsets(CurrentFootLockLocation, CurrentFootLockRotation);
	}
}

void UPlayerAnimation::SetFootLockOffsets(FVector& LocalLocation, FRotator& LocalRotation)
{
	if (!Character) return;
	FRotator RotationDifference;
	FVector LocationDifference;
	//Use the delta between the current and last updated rotation to find how much the foot should be rotated to remain planted on the ground.
	if (Character->GetCharacterMovement()->IsMovingOnGround())
	{
		FRotator CharacterRotationNormalized = Character->GetActorRotation() - Character->GetCharacterMovement()->
			GetLastUpdateRotation();
		CharacterRotationNormalized.Normalize();
		RotationDifference = CharacterRotationNormalized;
	}
	//Get the distance traveled between frames relative to the mesh rotation to find how much the foot should be offset to remain planted on the ground.
	LocationDifference = GetOwningComponent()->GetComponentRotation().UnrotateVector(
		Velocity * GetWorld()->GetDeltaSeconds());
	//Subtract the location difference from the current local location and rotate it by the rotation difference to keep the foot planted in component space.
	LocationDifference.RotateAngleAxis(RotationDifference.Yaw, FVector(0, 0, -1.f));
	LocalLocation = LocationDifference;
	//Subtract the Rotation Difference from the current Local Rotation to get the new local rotation.
	FRotator FinalRotation = LocalRotation - RotationDifference;
	FinalRotation.Normalize();
	LocalRotation = FinalRotation;
}

EMovementDirection UPlayerAnimation::CalculateMovementDirection()
{
	//Calculate the Movement Direction.
	//This value represents the direction the character is moving relative to the camera during the Looking Cirection / Aiming rotation modes,
	//and is used in the Cycle Blending Anim Layers to blend to the appropriate directional states.
	FRotator VelocityNormalized = (Velocity.Rotation() - AimingRotation);
	VelocityNormalized.Normalize();
	switch (Gait)
	{
	case EGait::Running:
		switch (RotationMode)
		{
		case ERotationMode::LookingDirection:
			return CalculateQuadrant(MovementDirection, 70.f, -70.f, 110.f, -110.f, 5.f, VelocityNormalized.Yaw);
		case ERotationMode::VelocityDirection:
			return EMovementDirection::Forward;
		case ERotationMode::Aiming:
			return CalculateQuadrant(MovementDirection, 70.f, -70.f, 110.f, -110.f, 5.f, VelocityNormalized.Yaw);
		default:
			return EMovementDirection::Forward;
			break;
		}
		break;
	case EGait::Walking:
		switch (RotationMode)
		{
		case ERotationMode::LookingDirection:
			return CalculateQuadrant(MovementDirection, 70.f, -70.f, 110.f, -110.f, 5.f, VelocityNormalized.Yaw);
		case ERotationMode::VelocityDirection:
			return EMovementDirection::Forward;
		case ERotationMode::Aiming:
			return CalculateQuadrant(MovementDirection, 70.f, -70.f, 110.f, -110.f, 5.f, VelocityNormalized.Yaw);
		default:
			return EMovementDirection::Forward;
			break;
		}
		break;
	case EGait::Sprinting:
		return EMovementDirection::Forward;
	default:
		return EMovementDirection::Forward;
		break;
	}
}

EMovementDirection UPlayerAnimation::CalculateQuadrant(EMovementDirection Current, float FRThreshold, float FLThreshold,
                                                       float BRThreshold, float BLThreshold, float Buffer, float Angle)
{
	//Take the input angle and determine its quadrant (direction).
	//Use the current Movement Direction to increase or decrease the buffers on the angle ranges for each quadrant.
	EMovementDirection NewDir = EMovementDirection::Backward;
	if (AngleInRange(Angle, FLThreshold, FRThreshold, Buffer,
	                 (Current != EMovementDirection::Forward || Current != EMovementDirection::Backward)))
		NewDir =
			EMovementDirection::Forward;
	if (AngleInRange(Angle, FRThreshold, BRThreshold, Buffer,
	                 (Current != EMovementDirection::Right || Current != EMovementDirection::Left)))
		NewDir =
			EMovementDirection::Right;
	if (AngleInRange(Angle, BLThreshold, FLThreshold, Buffer,
	                 (Current != EMovementDirection::Right || Current != EMovementDirection::Left)))
		NewDir =
			EMovementDirection::Left;
	return NewDir;
}

bool UPlayerAnimation::AngleInRange(float Angle, float MinAngle, float MaxAngle, float Buffer, bool IncreaseBuffer)
{
	if (IncreaseBuffer)
	{
		return UKismetMathLibrary::InRange_FloatFloat(Angle, MinAngle - Buffer, MaxAngle + Buffer, true, true);
	}
	else
	{
		return UKismetMathLibrary::InRange_FloatFloat(Angle, MinAngle + Buffer, MaxAngle - Buffer, true, true);
	}
}

FVelocityBlend UPlayerAnimation::InterpVelocityBlend(FVelocityBlend Current, FVelocityBlend Target, float InterpSpeed,
                                                     float DeltaTime)
{
	FVelocityBlend NewVelocityBlend;
	NewVelocityBlend.F = FMath::FInterpTo(Current.F, Target.F, DeltaTime, InterpSpeed);
	NewVelocityBlend.B = FMath::FInterpTo(Current.B, Target.B, DeltaTime, InterpSpeed);
	NewVelocityBlend.L = FMath::FInterpTo(Current.L, Target.L, DeltaTime, InterpSpeed);
	NewVelocityBlend.R = FMath::FInterpTo(Current.R, Target.R, DeltaTime, InterpSpeed);

	return NewVelocityBlend;
}

FLeanAmount UPlayerAnimation::InterpLeanAmount(FLeanAmount Current, FLeanAmount Target, float InterpSpeed,
                                               float DeltaTime)
{
	FLeanAmount NewLeanAmount;
	NewLeanAmount.LR = FMath::FInterpTo(Current.LR, Target.LR, DeltaTime, InterpSpeed);
	NewLeanAmount.FB = FMath::FInterpTo(Current.FB, Target.FB, DeltaTime, InterpSpeed);

	return NewLeanAmount;
}

EDrawDebugTrace::Type UPlayerAnimation::GetDebugTraceType(EDrawDebugTrace::Type ShowTraceType)
{
	ACharacter* Char = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	IControllerInterface* ControllerInterface = Cast<IControllerInterface>(Char);
	bool DebugView;
	bool ShowHUD;
	bool ShowTraces;
	bool ShowDebugShapes;
	bool ShowLayerColors;
	bool SlowMo;
	bool ShowCharInfo;
	if (Char && ControllerInterface)
	{
		ControllerInterface->GetDebugInfo(Char, DebugView, ShowHUD, ShowTraces, ShowDebugShapes, ShowLayerColors,
		                                  SlowMo, ShowCharInfo);
		if (ShowTraces)
		{
			return ShowTraceType;
		}
		else
		{
			return EDrawDebugTrace::Type::None;
		}
	}
	else
	{
		return EDrawDebugTrace::Type::None;
	}
}

void UPlayerAnimation::ResetIKOffsets()
{
	//Interp Foot IK offsets back to 0
	FVector ZeroV = FVector::ZeroVector;
	FRotator ZeroR = FRotator::ZeroRotator;
	FootOffset_L_Location = FMath::VInterpTo(FootOffset_L_Location, ZeroV, DeltaTimeX, 15.f);
	FootOffset_R_Location = FMath::VInterpTo(FootOffset_R_Location, ZeroV, DeltaTimeX, 15.f);
	FootOffset_L_Rotation = FMath::RInterpTo(FootOffset_L_Rotation, ZeroR, DeltaTimeX, 15.f);
	FootOffset_R_Rotation = FMath::RInterpTo(FootOffset_R_Rotation, ZeroR, DeltaTimeX, 15.f);
}


float UPlayerAnimation::GetAnimCurveClamped(FName Name, float Bias, float ClampMin, float ClampMax)
{
	float CurveValue = GetCurveValue(Name) + Bias;
	float ClampedValue = FMath::Clamp(CurveValue, ClampMin, ClampMax);
	return ClampedValue;
}
