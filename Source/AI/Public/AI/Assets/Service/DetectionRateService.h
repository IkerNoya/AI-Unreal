// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTService.h"
#include "Characters/EnemyCharacter.h"
#include "DetectionRateService.generated.h"

/**
 * 
 */
UCLASS()
class AI_API UDetectionRateService : public UBTService
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "Blackboard")
	float MaxDetectionRate = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "Blackboard")
	float DetectionRateSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "Blackboard")
	FBlackboardKeySelector LineOfSightKey;
	
private:
	UPROPERTY()
	UBlackboardComponent* Blackboard;
	UPROPERTY()
	AAIControllerBase* AIController;
	float DetectionRate;
	UPROPERTY()
	AEnemyCharacter* Enemy;
	bool bHasDetectedTarget=false;

public:
	UDetectionRateService();
	
protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	FORCEINLINE bool GetSelectedKey(UBlackboardComponent* CurrentBlackboard) const
	{
		return CurrentBlackboard->GetValueAsBool(LineOfSightKey.SelectedKeyName);
	}
	
	
};
