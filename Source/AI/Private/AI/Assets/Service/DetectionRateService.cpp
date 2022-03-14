// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Assets/Service/DetectionRateService.h"

UDetectionRateService::UDetectionRateService()
{
	bNotifyBecomeRelevant = true;
}

void UDetectionRateService::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	Blackboard = OwnerComp.GetBlackboardComponent();
	AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Blackboard)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't Get Blackboard component in %s"), *GetName());
	}
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't Get AIController in %s"), *GetName());
	}
	if (!Enemy)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't Get Enemy Character in %s"), *GetName());
	}
}


void UDetectionRateService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
}
