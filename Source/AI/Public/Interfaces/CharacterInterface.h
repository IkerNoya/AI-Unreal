// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterData/CharacterDataEnums.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AI_API ICharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void GetCurrentStates(EMovementMode& PawnMovementMode, EMovementState& MovementState,
	                              EMovementState& PrevMovementState, EMovementAction& MovementAction,
	                              ERotationMode& RotationMode, EGait& ActualGait, EStance& ActualStance,
	                              EViewMode& ViewMode, EMovementType& OverlayState);
	virtual void GetEssentialValues(FVector& Velocity, FVector& Acceleration, FVector& MovementInput, bool& IsMoving, bool& HasMovementInput, float& Speed, float& MovementInputAmount, FRotator& AimingRotation, float& AimYawRate);
};
