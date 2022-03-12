// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"

#include "AI/AIControllerBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	AIController = Cast<AAIControllerBase>(GetController());
}

void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyCharacter::OnTargetPerceptionUpdated);
}

void AEnemyCharacter::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	PerceivedActor = Actor;
	if (AIController)
	{
		bIsActorPerceived = Stimulus.WasSuccessfullySensed();
		if (Stimulus.WasSuccessfullySensed() && Actor->ActorHasTag("Player"))
		{
			GetWorld()->GetTimerManager().ClearTimer(TargetLostHandle);
			AIController->UpdateHasLineOfSightKey(true);
			AIController->UpdateTargetActorKey(PerceivedActor);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(TargetLostHandle, this, &AEnemyCharacter::TargetLost,
			                                       LineOfSightTimer, false);
			AIController->UpdateLastSeenActorPosition(Stimulus.StimulusLocation);
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
	if (AIController)
	{
		AIController->UpdateHasLineOfSightKey(false);
		AIController->UpdateTargetActorKey(nullptr);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't cast to AAIControllerBase from %s"), *GetName())
	}
}


// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacter::UpdateMovementSpeed(float NewSpeed)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
}

void AEnemyCharacter::ResetMovementSpeed()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	}
}

AActor* AEnemyCharacter::GetPerceivedActor()
{
	return PerceivedActor;
}
