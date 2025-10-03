// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"


// Sets default values
ASpawner::ASpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
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

void ASpawner::StartSpawning()
{
	if (!UnitToSpawn) return;

	// Start a timer with an initial delay
	GetWorldTimerManager().SetTimer(
		SpawnTimer,
		this,
		&ASpawner::SpawnUnits,
		SpawnInterval,           // Interval between spawns
		SpawnInterval > 0.0f,    // Should repeat?
		InitialSpawnDelay               // Initial delay before first spawn
	);
}

void ASpawner::StopSpawning()
{
	GetWorldTimerManager().ClearTimer(SpawnTimer);
}

void ASpawner::SpawnUnits()
{
	if (UnitToSpawn)
	{
		FActorSpawnParameters SpawnParams;
		auto unit = GetWorld()->SpawnActor<ASimpleDefaultAI>(UnitToSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);

		if (ShouldSpawnHunting)
		{
			if (ShouldHuntAfterTime)
			{
				GetWorldTimerManager().SetTimer(
				HuntTimer,
				[unit]()
					{
						if (unit && !unit->IsPendingKillPending())
							{
								unit->SetHunting(true);
							}
					},
					ActivateUnitTime,
					false
				);
			}
			else
			{
				unit->SetHunting(true);
			}
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
		bShowSpawnInterval = (AmountOfUnitsToSpawn > 1); // show field only if > 10
	}
}
#endif