// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleDefaultAI.h"

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

	void AssignPatrolPoint();
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
		worldState.PlayerInLineOfSight = true;
		worldState.CurrentBehaviour = EAIState::Hunting;
		worldState.PlayersInWorld.AddUnique(Cast<ACasterCharacter>(Actor));

		targetsInLineOfSight.AddUnique(Actor);
	}
	else
	{
		targetsInLineOfSight.Remove(Actor);
	}

	if (targetsInLineOfSight.IsEmpty())
		worldState.PlayerInLineOfSight = false;

	HandleTargetPerceptionUpdated_BP(Actor,Stimulus);
}

void ASimpleDefaultAI::SetHunting(bool hunting)
{
	if (hunting)
		worldState.CurrentBehaviour = EAIState::Hunting;
	else
		worldState.CurrentBehaviour = EAIState::Idle;
}
