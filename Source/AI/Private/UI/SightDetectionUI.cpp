// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SightDetectionUI.h"

#include "Characters/EnemyCharacter.h"

void USightDetectionUI::NativeConstruct()
{
	Super::NativeConstruct();
	if (!SightProgress)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't Bind Sight progress"));
	}
}

void USightDetectionUI::SetOwner(APawn* Pawn)
{
	Owner = Pawn;
}

void USightDetectionUI::ShouldHideProgressBar(bool Value)
{
	if (SightProgress)
	{
		if (Value)
		{
			SightProgress->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			SightProgress->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void USightDetectionUI::SetDetectionBar(float Progress)
{
	if (SightProgress)
	{
		SightProgress->SetPercent(Progress);
	}
}

void USightDetectionUI::SetAnimationCondition(bool Value)
{
	bPlayedAnimation = Value;
}
