// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UAIPerceptionStimuliSourceComponent;
class UCameraComponent;
class USpringArmComponent;
UCLASS()
class AI_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UAIPerceptionStimuliSourceComponent* PerceptionStimuli;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "Settings")
	float LookUpRate = 45.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "Settings")
	float TurnRate = 45.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "Settings")
	float RotationRate = 540.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "Settings")
	float JumpVelocity = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "Settings")
	float AirControl = .2f;
public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void Turn(float Value);
	void LookUp(float Value);
	
};
