// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Assets/Tasks/BTTFindNextPatrolLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EnemyCharacter.h"

EBTNodeResult::Type UBTTFindNextPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if(EnemyCharacter)
	{
		EnemyCharacter->SetNextPatrolLocation();
		FinishLatentTask(OwnerComp, EBTNodeResult::Type::Succeeded);
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Type::Failed);
		UE_LOG(LogTemp, Error, TEXT("Couldn't get owner in task"));
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
