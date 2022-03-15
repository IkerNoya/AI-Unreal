// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Instances/PlayerAnimation.h"

#include "GameFramework/Character.h"

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
		bRotateL= false;
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
	ElapsedDelayTime=0.f;
	bRotateL = false;
	bRotateR = false;
}
