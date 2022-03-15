// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class AI_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName TargetActorName = "TargetActor";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName HasLineOfSightName = "HasLineOfSight";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName TargetLocationName = "TargetLocation";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName DetectedLoSName = "DetectedLineOfSight";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName LastSeenPositionName = "LastSeenPosition";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName PatrolLocationName = "PatrolLocation";

	virtual void OnPossess(APawn* InPawn) override;
public:
	UFUNCTION(BlueprintCallable, Category = "AI")
	void UpdateTargetActorKey(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, Category = "AI")
	void UpdateHasLineOfSightKey(bool HasLineOfSight);
	UFUNCTION(BlueprintCallable, Category = "AI")
	void UpdateLastSeenActorPosition(FVector Position);
	UFUNCTION(BlueprintCallable, Category = "AI")
	void UpdateDetectedLineOfSight(bool Value);
	UFUNCTION(BlueprintCallable, Category = "AI")
	void UpdateTargetLocation(FVector TargetLocation);
	UFUNCTION(BlueprintCallable, Category = "AI")
	void UpdatePatrolLocation(FVector PatrolLocation);
};
