// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterDataEnums.generated.h"

UENUM()
enum class EGait : uint8
{
	Walking,
	Running,
	Sprinting
};
UENUM()
enum class EMovementAction : uint8
{
	None,
	LowMantle,
	HighMantle,
	Rolling,
	GettingUp
};
UENUM()
enum class EMovementState : uint8
{
	None, Grounded, InAir, Mantling, Ragdoll
};
UENUM()
enum class EMovementType : uint8
{
	Default, Masculine, Feminine, Injured, HandsTied, Rifle, Pistol1H, Pistol2H,
	Bow, Torch, Binoculars, Box, Barrel
};
UENUM()
enum class ERotationMode : uint8
{
	VelocityDirection, LookingDirection, Aiming
};
UENUM()
enum class EStance : uint8
{
	Standing, Crouching
};
UENUM()
enum class EViewMode : uint8
{
	ThirdPerson, FirstPerson
};
UENUM()
enum class EAnimFeatureExample : uint8
{
	StrideBlending, AdditiveBlending, SprintImpulse
};
UENUM()
enum class EFootstepType : uint8
{
	Step, WalkRun, Jump, Land
};
UENUM()
enum class EGroundedEntryState : uint8
{
	None, Roll
};
UENUM()
enum class EHipsDirection : uint8
{
	F, B, RF, RB, LF, LB
};
UENUM()
enum class EMantleType : uint8
{
	HighMantle, LowMantle, FallingCatch
};
UENUM()
enum class EMovementDirection : uint8
{
	Forward, Right, Left, Backward
};