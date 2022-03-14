// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/EnemyCharacter.h"
#include "Components/ProgressBar.h"
#include "SightDetectionUI.generated.h"

UCLASS()
class AI_API USightDetectionUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "UI")
	float DetectionValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "UI")
	float DetectSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "UI")
	APawn* Owner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "UI")
	bool HasSeenSomething;

private:
	UPROPERTY()
	AEnemyCharacter* Enemy;

	bool bPlayedAnimation = false;
	
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* SightProgress;
	
protected:	
	virtual void NativeConstruct() override;
	void EvaluateSightDetection();
	UFUNCTION()
	void SightRegistered(bool WasSeen, float DetectionSpeed);
	void GiveUp();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UFUNCTION(BlueprintImplementableEvent, Category = "Animation")
	void ActivateDetectionAnimation();

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetOwner(APawn* Pawn);

	
};
