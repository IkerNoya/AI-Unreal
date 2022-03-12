// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"

#include "NavigationSystem.h"
#include "AI/Navigation/NavigationTypes.h"
#include "AI/AIControllerBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	AIPerception->bEditableWhenInherited = true;
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1500.f;
	SightConfig->PeripheralVisionAngleDegrees = 70.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyCharacter::OnTargetPerceptionUpdated);
}

void AEnemyCharacter::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	PerceivedActor = Actor;
	AAIControllerBase* AIController = Cast<AAIControllerBase>(GetController());
	if (AIController)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			GetWorld()->GetTimerManager().ClearTimer(TargetLostHandle);
			AIController->UpdateHasLineOfSightKey(true);
			AIController->UpdateTargetActorKey(PerceivedActor);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(TargetLostHandle, this, &AEnemyCharacter::TargetLost, 5.f, false);
			AIController->UpdateHasLineOfSightKey(false);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't cast to AAIControllerBase from %s"), *GetName())
	}
}

void AEnemyCharacter::TargetLost()
{
	PerceivedActor = nullptr;
	AAIControllerBase* AIController = Cast<AAIControllerBase>(GetController());
	if (AIController)
	{
		AIController->UpdateTargetActorKey(nullptr);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't cast to AAIControllerBase from %s"), *GetName())
	}
}

void AEnemyCharacter::RandomWander()
{
	if (!GetController()) return;

	FNavLocation LocationReached;
	UNavigationSystemV1* NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if(NavigationSystem)
	{
		NavigationSystem->GetRandomReachablePointInRadius(GetActorLocation(), 1000.f, LocationReached);
		FVector NewTargetPosition = LocationReached.Location;
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), NewTargetPosition);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't get Navigation System V1"))
	}
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

