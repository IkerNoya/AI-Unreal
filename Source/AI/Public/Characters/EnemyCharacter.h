// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class AI_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UAIPerceptionComponent* AIPerception;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float MovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float LineOfSightTimer = 5.f;
private:
	UPROPERTY()
	AActor* PerceivedActor;
	UPROPERTY()
	FTimerHandle TargetLostHandle;
	UPROPERTY()
	class AAIControllerBase* AIController = nullptr;
	bool bIsActorPerceived=false;
public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void TargetLost();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void UpdateMovementSpeed(float NewSpeed);
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetMovementSpeed();
	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetPerceivedActor();

};
