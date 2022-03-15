// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterDataEnums.generated.h"

UENUM()
enum class AI_API EGait : uint8
{
	Walking,
	Running,
	Sprinting
};
UENUM()
enum class AI_API EMovementAction : uint8
{
	None,
	LowMantle,
	HighMantle,
	Rolling,
	GettingUp
};
UENUM()
enum class AI_API EMovementState : uint8
{
	None, Grounded, InAir, Mantling, Ragdoll
};
UENUM()
enum class AI_API EMovementType : uint8
{
	Default, Masculine, Feminine, Injured, HandsTied, Rifle, Pistol1H, Pistol2H,
	Bow, Torch, Binoculars, Box, Barrel
};
UENUM()
enum class AI_API ERotationMode : uint8
{
	VelocityDirection, LookingDirection, Aiming
};
UENUM()
enum class AI_API EStance : uint8
{
	Standing, Crouching
};
UENUM()
enum class AI_API EViewMode : uint8
{
	ThirdPerson, FirstPerson
};
UENUM()
enum class AI_API EAnimFeatureExample : uint8
{
	StrideBlending, AdditiveBlending, SprintImpulse
};
UENUM()
enum class AI_API EFootstepType : uint8
{
	Step, WalkRun, Jump, Land
};
UENUM()
enum class AI_API EGroundedEntryState : uint8
{
	None, Roll
};
UENUM()
enum class AI_API EHipsDirection : uint8
{
	F, B, RF, RB, LF, LB
};
UENUM()
enum class AI_API EMantleType : uint8
{
	HighMantle, LowMantle, FallingCatch
};
UENUM()
enum class AI_API EMovementDirection : uint8
{
	Forward, Right, Left, Backward
};