// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleDefaultAI.h"

#include "Kismet/GameplayStatics.h"
#include "Main/AI/Subsystem/EnemyLookup.h"
#include "Main/AI/Subsystem/PatrollpointsLookup.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


void ASimpleDefaultAI::BeginPlay()
{
	Super::BeginPlay();

	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ASimpleDefaultAI::HandlePerceptionUpdated);
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ASimpleDefaultAI::HandleTargetPerceptionUpdated);
	}

	GetWorld()->GetSubsystem<UEnemyLookup>()->AddAIToTile(GetActorLocation(), this);
}

UWorld* ASimpleDefaultAI::GetCurrentWorld() const
{
	return GetWorld();
}

void ASimpleDefaultAI::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASimpleDefaultAI, worldState);
	DOREPLIFETIME(ASimpleDefaultAI, AttackDistance);
}

void FCurrentPatrolPoint::AssignPatrolPoint(UWorld* world)
{
	PatrolPoint = world->GetSubsystem<UPatrollpointsLookup>()->GetPatrolPoint(PatrolPointID);
}

void FCurrentPatrolPoint::IncrementPatrolPoint()
{
	switch (PatrollingBehaviour)
	{
	case EPatrolBehaviour::RoundRobin:
		if (CurrentPatrolPointIndex >= PatrolPoint.Get()->Spline->GetNumberOfSplinePoints()-1)
		{
			CurrentPatrolPointIndex = 0;
		}
		else
		{
			CurrentPatrolPointIndex++;
		}
		break;
	case EPatrolBehaviour::BackAndForth:
		if (CurrentPatrolPointIndex >= PatrolPoint.Get()->Spline->GetNumberOfSplinePoints()-1)
		{
			Direction = -1;
		}

		if (CurrentPatrolPointIndex <= 0)
		{
			Direction = 1;
		}

		CurrentPatrolPointIndex += Direction;
		break;
	case EPatrolBehaviour::RandomIndex:
		CurrentPatrolPointIndex = FMath::RandRange(0, PatrolPoint.Get()->Spline->GetNumberOfSplinePoints()-1);
		break;
	default: ;
	}
}

FVector FCurrentPatrolPoint::GetPatrolPointInWorld() const
{
	return PatrolPoint.Get()->Spline->GetWorldLocationAtSplinePoint(CurrentPatrolPointIndex);
}

void ASimpleDefaultAI::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		if (worldState.PlayersInWorld.Num() < 2)
		{
			FTimerHandle Handle;
			GetWorldTimerManager().SetTimer(Handle, this, &ASimpleDefaultAI::AddAllPlayers, 0.5, false);
		}
		
		APawn* ClosestPlayer = nullptr;
		APawn* FurthestPlayer = nullptr;

		float ClosestDistSq = FLT_MAX;
		float FurthestDistSq = 0.f;

		for (APawn* Player : worldState.PlayersInWorld)
		{
			if (!IsValid(Player)) continue;

			const float DistSq = FVector::DistSquared(GetActorLocation(), Player->GetActorLocation());

			if (DistSq < ClosestDistSq)
			{
				ClosestDistSq = DistSq;
				ClosestPlayer = Player;
			}

			if (DistSq > FurthestDistSq)
			{
				FurthestDistSq = DistSq;
				FurthestPlayer = Player;
			}
		}

		if (ClosestPlayer && !FurthestPlayer)
		{
			FurthestPlayer = ClosestPlayer;
		}

		if (worldState.ClosestPlayer != ClosestPlayer)
		{
			worldState.ClosestPlayer = ClosestPlayer;
			worldState.ClosestPlayerDistance = FMath::Sqrt(ClosestDistSq);
		}

		worldState.furthestPlayer = FurthestPlayer;
	}

	

	
}

// Sets default values
ASimpleDefaultAI::ASimpleDefaultAI() : worldState()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AbilityManager = CreateDefaultSubobject<UAbilityManagerComponent>(TEXT("AbilityManager"));
	ExperienceGranter = CreateDefaultSubobject<UExperienceGranter>(TEXT("ExperienceGranter"));

	// Setup sight
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1200.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	TeamId = FGenericTeamId(1);
}

FGenericTeamId ASimpleDefaultAI::GetGenericTeamId() const
{
	return TeamId;
}



void ASimpleDefaultAI::HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{	
	OnPerceptionUpdated_BP(UpdatedActors);
	
}

void ASimpleDefaultAI::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		worldState.CurrentBehaviour = EAIState::Hunting;

		auto enemiesInRadius = GetWorld()->GetSubsystem<UEnemyLookup>()->FindAllContentInRadius(GetActorLocation(), 1);

		for(auto enemy : enemiesInRadius)
		{
			if (!IsValid(enemy))
				continue;

			enemy->SetHunting(true);
			
			FStateTreeEvent AttackEvent;
			AttackEvent.Tag = FGameplayTag::RequestGameplayTag(FName("ActiveStatus.true"));

			if (IsValid(enemy->CurrentController))
				enemy->CurrentController->StateTreeAIComponent->SendStateTreeEvent(AttackEvent);
		}
		
		int32 NumPlayers = UGameplayStatics::GetNumPlayerControllers(GetWorld());

		if (NumPlayers > worldState.PlayersInWorld.Num())
		{
			for (int i = 0; i <= (NumPlayers - 1); i++)
			{
				if (UGameplayStatics::GetPlayerController(GetWorld(), i)->GetPawn() != nullptr)	
					worldState.PlayersInWorld.AddUnique(UGameplayStatics::GetPlayerController(GetWorld(), i)->GetPawn());
			}
		}
	}
	AIPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), false);

	HandleTargetPerceptionUpdated_BP(Actor,Stimulus);
}

void ASimpleDefaultAI::SetHunting(bool hunting)
{
	if (hunting)
	{
		worldState.CurrentBehaviour = EAIState::Hunting;

		if (worldState.ClosestPlayer == nullptr)
		{
			if (!GetWorld())
			{
				return;
			}
			UWorld* World = GetWorld();
			if (!World)
				return;
			
			APlayerController* C = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (!C)
				return;
			
			worldState.ClosestPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
		}
	}
	else
		worldState.CurrentBehaviour = EAIState::Idle;
}

void ASimpleDefaultAI::StartAllActions()
{
	if (CurrentController)
		if (CurrentController->StateTreeAIComponent)
			CurrentController->StateTreeAIComponent->StartLogic();
}

void ASimpleDefaultAI::StopAllActions()
{
	if (HasAuthority() && IsValid(CurrentController))
		CurrentController->StateTreeAIComponent->StopLogic("Paused_actions");
}

void ASimpleDefaultAI::AddAllPlayers()
{
	if (worldState.PlayersInWorld.Num() >= 2)
		return;
	
	int32 NumPlayers = UGameplayStatics::GetNumPlayerControllers(GetWorld());

	if (NumPlayers > worldState.PlayersInWorld.Num())
	{
		for (int i = 0; i <= (NumPlayers - 1); i++)
		{
			if (IsValid(UGameplayStatics::GetPlayerController(GetWorld(), i)->GetPawn()))
			{
				worldState.PlayersInWorld.AddUnique(UGameplayStatics::GetPlayerController(GetWorld(), i)->GetPawn());
			}
			
		}
	}
}

TArray<APawn*> ASimpleDefaultAI::GetAllPlayersInWorld()
{
	TArray<APawn*> PlayerPawns;
	
	int32 NumPlayers = UGameplayStatics::GetNumPlayerControllers(GetWorld());

	for (int i = 0; i <= (NumPlayers - 1); i++)
	{
		if (UGameplayStatics::GetPlayerController(GetWorld(), i)->GetPawn() != nullptr)	
			PlayerPawns.AddUnique(UGameplayStatics::GetPlayerController(GetWorld(), i)->GetPawn());
	}

	return PlayerPawns;
}

void ASimpleDefaultAI::OnMove()
{
	if (HasAuthority()) //Runs on Server
		Server_OnMove();
}

void ASimpleDefaultAI::Server_OnMove_Implementation()
{
	worldState.CurrentTile = GetWorld()->GetSubsystem<UEnemyLookup>()->WorldToTile(GetActorLocation());
	if (worldState.PreviousTile == worldState.CurrentTile)
		return;
	auto system = GetWorld()->GetSubsystem<UEnemyLookup>();

	system->RemoveAIFromTile(system->TileToWorld(worldState.PreviousTile), this);

	worldState.PreviousTile = worldState.CurrentTile;
	system->AddAIToTile(GetActorLocation(), this);	


}
