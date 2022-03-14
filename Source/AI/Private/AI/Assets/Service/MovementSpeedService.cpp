// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Assets/Service/MovementSpeedService.h"

#include "AIController.h"
#include "Characters/EnemyCharacter.h"


UMovementSpeedService::UMovementSpeedService()
{
	bNotifyBecomeRelevant= true;
}

void UMovementSpeedService::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if(Enemy)
	{
		Enemy->UpdateMovementSpeed(MovementSpeed);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't get enemy character in service"));
	}
}
