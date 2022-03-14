// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "MovementSpeedService.generated.h"

/**
 * 
 */
UCLASS()
class AI_API UMovementSpeedService : public UBTService
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MovementSpeed = 125.f;
	
protected:
	UMovementSpeedService();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
