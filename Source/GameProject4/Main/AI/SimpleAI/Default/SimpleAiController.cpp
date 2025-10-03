// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleAiController.h"

#include "Kismet/GameplayStatics.h"

#include "SimpleDefaultAI.h"
#include "Perception/AIPerceptionTypes.h"


ETeamAttitude::Type ASimpleAiController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(&Other))
	{
		if (OtherTeamAgent->GetGenericTeamId() == FGenericTeamId(2)) // enemy team
		{
			return ETeamAttitude::Hostile;
		}
		else if (OtherTeamAgent->GetGenericTeamId() == FGenericTeamId(1))
		{
			return ETeamAttitude::Friendly;
		}
	}

	return ETeamAttitude::Neutral;
}

void ASimpleAiController::BeginPlay()
{
	Super::BeginPlay();
}

void ASimpleAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CurrentPawn = Cast<ASimpleDefaultAI>(InPawn);
	
	if (!CurrentPawn)
		return;

	
	CurrentPawn->CurrentController = this;
	AddPlayers();

	
}

// Sets default values
ASimpleAiController::ASimpleAiController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StateTreeAIComponent = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAIComponent"));
}

void ASimpleAiController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentPawn)
		return;

	AssignClosestPlayer();
}

void ASimpleAiController::AssignClosestPlayer() const
{
	if (!CurrentPawn)
		return;
	
	if (CurrentPawn->worldState.PlayersInWorld.IsEmpty())
	{
		return;
	}

	auto players = CurrentPawn->worldState.PlayersInWorld;


	for (auto CasterCharacter : players)
	{
		float Distance = FVector::Dist(CasterCharacter->GetActorLocation(), CurrentPawn->GetActorLocation());
		if (CasterCharacter != CurrentPawn->worldState.ClosestPlayer)
		{
			if (Distance < CurrentPawn->worldState.ClosestPlayerDistance)
			{
				CurrentPawn->worldState.ClosestPlayer = CasterCharacter;

				FStateTreeEvent Event;
				Event.Tag = FGameplayTag::RequestGameplayTag(FName("AggroTarget.Changed"));
				StateTreeAIComponent->SendStateTreeEvent(Event);
			}
		}
		if (CasterCharacter != CurrentPawn->worldState.furthestPlayer)
			if (Distance > CurrentPawn->worldState.ClosestPlayerDistance)
			{
				CurrentPawn->worldState.furthestPlayer = CasterCharacter;
			}
	}

	CurrentPawn->worldState.ClosestPlayerDistance = FVector::Dist(CurrentPawn->worldState.ClosestPlayer->GetActorLocation(), CurrentPawn->GetActorLocation());

	if (CurrentPawn->AttackDistance.MinAttackDistance > CurrentPawn->worldState.ClosestPlayerDistance)
	{
		CurrentPawn->AttackDistance.TargetInRange = ETargetInRange::Close;
	}
	else if (CurrentPawn->AttackDistance.MaxAttackDistance < CurrentPawn->worldState.ClosestPlayerDistance)
	{
		CurrentPawn->AttackDistance.TargetInRange = ETargetInRange::Far;
	}
	else
	{
		CurrentPawn->AttackDistance.TargetInRange = ETargetInRange::InRange;
	}
}

void ASimpleAiController::AddPlayers_Implementation() const
{
	TArray<AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACasterCharacter::StaticClass(), FoundPlayers);

	CurrentPawn->worldState.PlayersInWorld.Empty();

	for (AActor* Actor : FoundPlayers)
	{
		if (ACasterCharacter* Player = Cast<ACasterCharacter>(Actor))
		{
			CurrentPawn->worldState.PlayersInWorld.AddUnique(Player);
		}
	}
}
