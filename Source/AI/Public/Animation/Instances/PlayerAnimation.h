// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimation.generated.h"

/**
 * 
 */
UCLASS()
class AI_API UPlayerAnimation : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	ACharacter* Character = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	float DeltaTimeX=0.f;
	
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
	bool bIsMoving=false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	bool bHasMovementInput = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	float Speed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	float MovementInputAmount=0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	float AimYawRate = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	float ZoomAmount = 0.f;

};
