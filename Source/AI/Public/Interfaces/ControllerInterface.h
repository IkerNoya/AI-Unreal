// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AI_API IControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void GetDebugInfo(ACharacter* DebugFocusCharacter, bool& DebugView, bool& ShowHUD, bool& ShowTraces,
		bool& ShowDebugShapes, bool &ShowLayerColors, bool& SlowMo, bool& ShowCharacterInfo){}
};
