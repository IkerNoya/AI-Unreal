// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CameraInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCameraInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AI_API ICameraInterface
{
	GENERATED_BODY()
public:
	virtual void GetCameraParameters(float TP_Fov, float FP_Fov, bool RightShoulder){}
	virtual FVector GetFPCameraTarget(){return FVector::ZeroVector;}
	virtual FTransform GetTPPivotTarget(){return FTransform::Identity;}
	virtual void GetTPTraceParams(FVector& TraceOrigin, float& TraceRadius, ETraceTypeQuery& TraceChannel){}
};
