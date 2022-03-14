// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Assets/Tasks/LooseTargetTask.h"

#include "AIController.h"
#include "Characters/EnemyCharacter.h"

EBTNodeResult::Type ULooseTargetTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if(EnemyCharacter)
	{
		EnemyCharacter->TargetLost();
		FinishLatentTask(OwnerComp, EBTNodeResult::Type::Succeeded);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't Cast to Enemy Character"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Type::Failed);
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
