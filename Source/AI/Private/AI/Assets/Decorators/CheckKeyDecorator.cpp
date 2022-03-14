// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Assets/Decorators/CheckKeyDecorator.h"

#include "BehaviorTree/BlackboardComponent.h"

bool UCheckKeyDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if(!bSuccess) return false;

	UObject* Result = OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey());
	if(Result != nullptr)
	{
		return true;
	}
	
	return false;
}
