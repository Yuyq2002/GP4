// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "Debug.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASpawner::ASpawner(): SpawnLogic(ESpawnLogic::Single), AmountOfUnitsToSpawn(0), InitialSpawnDelay(0), bShowSpawnInterval(false),
                      SpawnInterval(0),
                      ShouldSpawnHunting(false),
                      spawnOnBegin(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	if (spawnOnBegin)
	{
		StartSpawning();
	}
	
}

void ASpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASpawner, AmountOfUnitsToSpawn);
}

void ASpawner::StopSpawning_Implementation()
{
	if (!HasAuthority())
		return;
	GetWorldTimerManager().ClearTimer(SpawnTimer);
}

void ASpawner::SpawnUnits()
{
	if (!HasAuthority())
		return;
	if (UnitToSpawn)
	{
		FActorSpawnParameters SpawnParams;
		auto unit = GetWorld()->SpawnActor<ASimpleDefaultAI>(UnitToSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);
				
		if (ShouldSpawnHunting)
		{
			if (IsValid(unit))
				unit->SetHunting(true);
		}

		AmountOfUnitsToSpawn--;

		if (AmountOfUnitsToSpawn <= 0)
			StopSpawning();
	}
}

#if WITH_EDITOR
void ASpawner::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property &&
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ASpawner, AmountOfUnitsToSpawn))
	{
		bShowSpawnInterval = (SpawnLogic == ESpawnLogic::Interval); // show field only if > 10
	}
}
#endif


void ASpawner::StartSpawning_Implementation()
{
	if (!HasAuthority())
	{
		Debug::Log("no authoroty");
		return;
	}
	if (!UnitToSpawn)
	{
		return;
	}
	FActorSpawnParameters SpawnParams;

	ASimpleDefaultAI* Unit = nullptr;

	switch (SpawnLogic)
	{
	case ESpawnLogic::Single:
		
		/*Unit = GetWorld()->SpawnActor<ASimpleDefaultAI>(UnitToSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);

		if (Unit->CurrentController == nullptr)
		{
			auto* controller = GetWorld()->SpawnActor<ASimpleAiController>(IfNeededAiController, GetActorLocation(), GetActorRotation(), SpawnParams);
			controller->Possess(Unit);

			Unit->CurrentController = controller;
		}*/

		SpawnUnits();

		break;
	case ESpawnLogic::Interval:
		GetWorldTimerManager().SetTimer(
			SpawnTimer,
			this,
			&ASpawner::SpawnUnits,
			SpawnInterval,
			true,
			InitialSpawnDelay
		);
		break;
	case ESpawnLogic::Chunk:
		for (int i = 0; i < AmountOfUnitsToSpawn; i++)
		{
			auto unit = GetWorld()->SpawnActor<ASimpleDefaultAI>(UnitToSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);

			if (ShouldSpawnHunting)
			{
				unit->SetHunting(true);
			}
		}

		AmountOfUnitsToSpawn = 0;
		break;
	default: ;
	}

	
}
