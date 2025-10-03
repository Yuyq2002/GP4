// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultEnemyAI.h"

#include "Net/UnrealNetwork.h"
#include "Subsystem/EnemyLookup.h"


void ADefaultEnemyAI::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
	/*DrawDebugCircle(
			GetWorld(),
			GetActorLocation(),    // Center
			100.f,                 // Radius
			32,                    // Number of segments
			FColor::Green,         // Color
			false,                 // Persistent lines
			-1.f,                  // Life time (-1 = single frame)
			0,                     // Depth priority
			2.f,                   // Line thickness
			FVector(1,0,0),        // Axis X
			FVector(0,1,0),        // Axis Y
			false                  // Draw axis (cross)
		);*/
}

// Sets default values
ADefaultEnemyAI::ADefaultEnemyAI(): CurrentTile(), PreviousTile(), PatrolPoint(nullptr), Stunned(), AggroTarget(nullptr)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TeamId = FGenericTeamId(1);
}

AActor* ADefaultEnemyAI::GetAggroTarget()
{
	if (AggroTarget.IsValid())
	{
		return AggroTarget.Get();
	}
	
	return nullptr;
}

bool ADefaultEnemyAI::CanAttack() const
{
	return AttackState == AIAttackState::ReadyToAttack;
}

void ADefaultEnemyAI::OnMove()
{
	if (HasAuthority()) //Runs on Server
		Server_OnMove();
}

void ADefaultEnemyAI::SetStunnedTime(float time)
{
	StunTime = time;
}

bool ADefaultEnemyAI::DoAttackFromArrayRef(Attacktype arrayType, int chosenAttack)
{
	

	return false;
}

void ADefaultEnemyAI::Server_OnMove_Implementation()
{
	/*CurrentTile = GetWorld()->GetSubsystem<UEnemyLookup>()->WorldToTile(GetActorLocation());
	if (PreviousTile == CurrentTile)
		return;
	auto system = GetWorld()->GetSubsystem<UEnemyLookup>();

	PreviousTile = CurrentTile;
	

	system->RemoveAIFromTile(system->TileToWorld(PreviousTile), this);
	system->AddAIToTile(GetActorLocation(), this);*/
}

FGenericTeamId ADefaultEnemyAI::GetGenericTeamId() const
{
	return TeamId;
}

// Called when the game starts or when spawned
void ADefaultEnemyAI::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld()->GetSubsystem<UEnemyLookup>()->AddAIToTile(GetActorLocation(), this);

	CurrentTile = GetWorld()->GetSubsystem<UEnemyLookup>()->WorldToTile(GetActorLocation());
}

void ADefaultEnemyAI::SetAggroTarget(AActor* target)
{
	if (!IsValid(this))
	{
		UE_LOG(LogTemp, Error, TEXT("SetAggroTarget called on invalid ADefaultEnemyAI"));
		return;
	}

	if (target == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("target is null"));
		return;
	}
	
	if (target != nullptr)
		AggroTarget = target;
}

AActor* ADefaultEnemyAI::GetClosestPlayer()
{
	AActor* ClosestPlayer = nullptr;
	float ClosestDistSq = FLT_MAX;

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC && PC->GetPawn())
		{
			float DistSq = FVector::DistSquared(GetActorLocation(), PC->GetPawn()->GetActorLocation());
			if (DistSq < ClosestDistSq)
			{
				ClosestDistSq = DistSq;
				ClosestPlayer = PC->GetPawn();
			}
		}
	}

	SetAggroTarget(ClosestPlayer);
	
	return ClosestPlayer;
}

void ADefaultEnemyAI::RemoveAggroTarget()
{
	AggroTarget = nullptr;
}

AggroRangeStatus ADefaultEnemyAI::AggroTargetIsInAttackRange()
{
	if (!AggroTarget.IsValid())
		return AggroRangeStatus::TargetIsNull;

	FVector AiLocation = GetActorLocation();
	FVector targetLocation = AggroTarget->GetActorLocation();

	float Distance = FVector::Dist(AiLocation, targetLocation);

	if (Distance >= MinDistanceToTarget && Distance <= MaxDistanceToTarget)
	{
		return AggroRangeStatus::TargetInRange;
	}

	return AggroRangeStatus::TargetNotInRange;
}

AggroRangeStatus ADefaultEnemyAI::GetAggroRange()
{
	if (!AggroTarget.IsValid())
		return AggroRangeStatus::TargetIsNull;

	FVector AiLocation = GetActorLocation();
	FVector targetLocation = AggroTarget->GetActorLocation();

	float Distance = FVector::Dist(AiLocation, targetLocation);

	if (Distance <= MinDistanceToTarget)
	{
		return AggroRangeStatus::TargetIsClose;
	}

	if (Distance >= MinDistanceToTarget && Distance <= MaxDistanceToTarget)
	{
		return AggroRangeStatus::TargetInRange;
	}

	return AggroRangeStatus::TargetNotInRange;
}

void ADefaultEnemyAI::SetAttackState_Implementation(::AIAttackState state)
{
	if (!HasAuthority())
		return;
	
	AttackState = state;
}

void ADefaultEnemyAI::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADefaultEnemyAI, IsHunting);
	DOREPLIFETIME(ADefaultEnemyAI, CurrentTile);
	DOREPLIFETIME(ADefaultEnemyAI, PreviousTile);
	DOREPLIFETIME(ADefaultEnemyAI, AIIdleBehaviour);
	DOREPLIFETIME(ADefaultEnemyAI, AggroTarget);
	DOREPLIFETIME(ADefaultEnemyAI, AttackState);
	
}

// Called to bind functionality to input
void ADefaultEnemyAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}



void ADefaultEnemyAI::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}