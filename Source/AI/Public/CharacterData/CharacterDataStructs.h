// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveVector.h"
#include "CharacterDataStructs.generated.h"

USTRUCT()
struct AI_API FComponentAndTransform
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPrimitiveComponent* Component;
};

USTRUCT()
struct AI_API FCameraSettings
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetArmLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SocketOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LagSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationLagSpeed;
};

USTRUCT()
struct AI_API FCameraSettings_Gait
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCameraSettings Walking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCameraSettings Running;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCameraSettings Sprinting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCameraSettings Crouching;
};

USTRUCT()
struct AI_API FCameraSettings_State
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCameraSettings_Gait VelocityDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCameraSettings_Gait LookingDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCameraSettings_Gait Aiming;
};

USTRUCT()
struct AI_API FDynamicMontageParams
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequenceBase* Animation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendInTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendOutTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartTime;
};

USTRUCT()
struct AI_API FLeanAmount
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FB;
};

USTRUCT()
struct AI_API FMantleAsset
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveVector* PositionCorrectionCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartingOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LowHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LowPlayRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LowStartPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HighHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HighPlayRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HighStartPosition;
};

USTRUCT()
struct AI_API FMantleParams
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveVector* PositionCorrectionCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartingPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartingOffset;
};

USTRUCT()
struct AI_API FMantleTraceSettings
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxLedgeHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinLedgeHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReachDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ForwardTraceRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DownwardTraceRadius;
};

USTRUCT()
struct AI_API FMovementSettings
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveVector* MovementCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* RotationRateCurve;
};

USTRUCT()
struct AI_API FMovementSettings_Stance
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMovementSettings Standing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMovementSettings Crouching;
};

USTRUCT()
struct AI_API FMovementSettings_State
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMovementSettings_Stance VelocityDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMovementSettings_Stance LookingDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMovementSettings_Stance Aiming;
};

USTRUCT()
struct AI_API FRotateInPlaceAsset
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SlotName = NAME_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlowTurnRate = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FastTurnRate = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlowPlayRate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FastPlayRate = 1.f;
};

USTRUCT()
struct AI_API FTurnInPlaceAsset
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequenceBase* Animation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SlotName = NAME_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimatedAngle = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayRate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bScaleTurnAngle = true;
};

USTRUCT()
struct AI_API FVelocityBlend
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float F;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float B;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float L;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float R;
};
