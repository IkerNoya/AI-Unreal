// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"

#include "AI/AIControllerBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "UI/SightDetectionUI.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	DetectionWidget = CreateDefaultSubobject<UWidgetComponent>("Detection Widget");
	DetectionWidget->SetupAttachment(GetMesh());
	
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
	USightDetectionUI *Widget= Cast<USightDetectionUI>(DetectionWidget->GetUserWidgetObject());
	if(Widget)
	{
		Widget->SetOwner(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't set widget class in %s"), *GetName());
	}
	
}

void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyCharacter::OnTargetPerceptionUpdated);
}

void AEnemyCharacter::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (AIController && Actor->ActorHasTag("Player"))
	{
		bIsActorPerceived = Stimulus.WasSuccessfullySensed();
		SightRegistered.Broadcast(bIsActorPerceived, DetectionTimeSpeed);
		if (Stimulus.WasSuccessfullySensed())
		{
			AIController->UpdateHasLineOfSightKey(true);
			PerceivedActor = Actor;
			AIController->UpdateLastSeenActorPosition(Actor->GetActorLocation());
		}
		else
		{
			AIController->UpdateLastSeenActorPosition(Actor->GetActorLocation());
			AIController->UpdateHasLineOfSightKey(false);
			AIController->UpdateDetectedLineOfSight(false);
		}
	}
}

void AEnemyCharacter::TargetLost()
{
	PerceivedActor = nullptr;
	DetectionRate = 0.f;
	bIsTargetInLineOfSight = false;
	bIsTargetDetected=false;

	if (AIController)
	{
		AIController->UpdateHasLineOfSightKey(false);
		AIController->UpdateTargetActorKey(nullptr);
		AIController->UpdateDetectedLineOfSight(false);
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

void AEnemyCharacter::SeenPlayer()
{

	if(PerceivedActor)
	{
		AIController->UpdateHasLineOfSightKey(true);
		AIController->UpdateTargetActorKey(PerceivedActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Perceived actor is null"))
	}
}

void AEnemyCharacter::LostPlayer()
{
}
