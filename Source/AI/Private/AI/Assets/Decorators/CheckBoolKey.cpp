// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Assets/Decorators/CheckBoolKey.h"

#include "BehaviorTree/BlackboardComponent.h"

bool UCheckBoolKey::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if(!bSuccess) return false;

	bool Result = OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if(Result)
	{
		return true;
	}
	
	return false;
}
