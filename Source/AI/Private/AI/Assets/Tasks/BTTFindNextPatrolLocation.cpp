// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Assets/Tasks/BTTFindNextPatrolLocation.h"
#include "NavigationSystem.h"
#include "AI/Navigation/NavigationTypes.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EnemyCharacter.h"

EBTNodeResult::Type UBTTFindNextPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if(EnemyCharacter && Blackboard)
	{
		FNavLocation LocationReached;
		UNavigationSystemV1* NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
		if(NavigationSystem)
		{
			if(NavigationSystem->GetRandomReachablePointInRadius(EnemyCharacter->GetActorLocation(), PatrolRadius, LocationReached))
			{
				FVector NewTargetPosition = LocationReached.Location;
				Blackboard->SetValueAsVector(PatrolLocation.SelectedKeyName, NewTargetPosition);
			}
			else
			{
				Blackboard->SetValueAsVector(PatrolLocation.SelectedKeyName, EnemyCharacter->GetActorLocation());
			}
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::Type::Succeeded);
		UE_LOG(LogTemp, Warning, TEXT("Completed Task"));
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Type::Failed);
		UE_LOG(LogTemp, Error, TEXT("Couldn't get owner in task"));
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
