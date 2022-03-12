// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIControllerBase.h"

#include "BehaviorTree/BlackboardComponent.h"

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunBehaviorTree(BehaviorTree);
}

void AAIControllerBase::UpdateTargetActorKey(AActor* TargetActor)
{
	Blackboard->SetValueAsObject(TargetActorName, TargetActor);
}

void AAIControllerBase::UpdateHasLineOfSightKey(bool HasLineOfSight)
{
	Blackboard->SetValueAsBool(HasLineOfSightName, HasLineOfSight);
}
