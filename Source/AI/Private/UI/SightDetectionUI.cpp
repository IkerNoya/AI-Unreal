// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SightDetectionUI.h"

void USightDetectionUI::NativeConstruct()
{
	Super::NativeConstruct();
	Enemy = Cast<AEnemyCharacter>(Owner);
	if(!Enemy)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't get enemy character from %s"), *GetName());
	}
	Enemy->SightRegistered.AddDynamic(this, &USightDetectionUI::SightRegistered);
	if(!SightProgress)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't Bind Sight progress"));
	}
}

void USightDetectionUI::EvaluateSightDetection()
{
	if(DetectionValue<= 0 && !HasSeenSomething)
	{
		GiveUp();
		if(Enemy)
		{
			Enemy->LostPlayer();
		}
	}
	else
	{
		if(DetectionValue >= 1.f && Enemy)
		{
			Enemy->SeenPlayer();
		}
	}
}

void USightDetectionUI::SightRegistered(bool WasSeen, float DetectionSpeed)
{
	UE_LOG(LogTemp, Warning, TEXT("Entro en delegate"));
	DetectSpeed = DetectionSpeed;
	HasSeenSomething = WasSeen;
	if(SightProgress)
	{
		SightProgress->SetVisibility(ESlateVisibility::Visible);
	}
	
}

void USightDetectionUI::GiveUp()
{
	if(SightProgress)
	{
		SightProgress->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USightDetectionUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(!HasSeenSomething && DetectionValue <= 0) return;
	float TargetValue = HasSeenSomething? 1.0f : 0.0f;
	DetectionValue = FMath::Lerp(DetectionValue, TargetValue, InDeltaTime * DetectSpeed);
	if(SightProgress)
	{
		SightProgress ->SetPercent(DetectionValue);
	}
	EvaluateSightDetection();
}

void USightDetectionUI::SetOwner(APawn* Pawn)
{
	Owner = Pawn;
}
