// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "SightDetectionUI.generated.h"

UCLASS()
class AI_API USightDetectionUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float DetectionValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float DetectSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	APawn* Owner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool HasSeenSomething;

private:
	bool bPlayedAnimation = false;

public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* SightProgress;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Animation")
	void ActivateDetectionAnimation();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetOwner(APawn* Pawn);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShouldHideProgressBar(bool Value);
	
	UFUNCTION(BlueprintCallable, Category = "Detection")
	void SetDetectionBar(float Progress);
	UFUNCTION(BlueprintCallable, Category = "Detection")
	void SetAnimationCondition(bool Value);
	UFUNCTION(BlueprintCallable, Category = "Detection")
	FORCEINLINE bool GetAnimationBool() { return bPlayedAnimation; }
};
