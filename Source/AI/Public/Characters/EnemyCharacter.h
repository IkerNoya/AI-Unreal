// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* DetectionWidget;
	
	UPROPERTY(VisibleAnywhere, Category = "Settings")
	UAISenseConfig_Sight* SightConfig;
	UPROPERTY(VisibleAnywhere, Category = "Settings")
	UAISenseConfig_Sight* SightConfig;
	UPROPERTY(VisibleAnywhere, Category = "Settings")
	UAISenseConfig_Sight* SightConfig;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float MovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float LineOfSightTimer = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float TimeToDetectPlayer = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float DetectionTimeSpeed = 1.f;
	
private:
	UPROPERTY()
	AActor* PerceivedActor;
	UPROPERTY()
	FTimerHandle TargetLostHandle;
	UPROPERTY()
	class AAIControllerBase* AIController = nullptr;
	UPROPERTY()
	class USightDetectionUI* SightWidget;
	bool bIsActorPerceived=false;
	bool bIsTargetDetected = false;
	float DetectionRate = 0.f;
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
	UFUNCTION(BlueprintCallable, Category  = "AI")
	void SeenPlayer();
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	FORCEINLINE float GetDetectionRate() const {return DetectionRate;}
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	FORCEINLINE UCurveFloat * GetSightCurve() const{return SightCurve;}
private:
	void EvaluateDetection();
};
