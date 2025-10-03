// Fill out your copyright notice in the Description page of Project Settings.


#include "ModifiedPlayerStats.h"


UModifiedPlayerStats::UModifiedPlayerStats()
{
	PrimaryComponentTick.bCanEverTick = false;

	// BaseStats.Add("Damage") = 10.0f;
	// BaseStats.Add("WalkSpeed") = 600.0f;
	// BaseStats.Add("ProjectileSpeed") = 1000.0f;

	CurrentStats = BaseStats;

	// ...
}
//
//
void UModifiedPlayerStats::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

TArray<FName> UModifiedPlayerStats::GetAvailableStatKeys() const
{
    return AvailableStatKeys;
}

void UModifiedPlayerStats::ResetStatsToBase()
{
	CurrentStats = BaseStats;
	for (const TPair<FName, float>& Pair : CurrentStats)
	{
		OnStatChanged.Broadcast(Pair.Key, Pair.Value);
	}
}

TMap<FName, float> UModifiedPlayerStats::GetCurrentStats() const
{
	return CurrentStats;
}

void UModifiedPlayerStats::SetCurrentStats(const TMap<FName, float>& NewStats)
{
	for (const TPair<FName, float>& Pair : NewStats)
	{
		const float* ExistingValue = CurrentStats.Find(Pair.Key);
		if (!ExistingValue || !FMath::IsNearlyEqual(*ExistingValue, Pair.Value))
		{
			OnStatChanged.Broadcast(Pair.Key, Pair.Value);
		}
	}
	CurrentStats = NewStats;
}

float UModifiedPlayerStats::GetStat(FName StatName) const
{
	const float* FoundValue = CurrentStats.Find(StatName);
	return FoundValue ? *FoundValue : 0.0f;
}

void UModifiedPlayerStats::SetStat(FName StatName, float NewValue)
{
	float* ExistingValue = CurrentStats.Find(StatName);
	if (!ExistingValue || !FMath::IsNearlyEqual(*ExistingValue, NewValue))
	{
		CurrentStats.Add(StatName, NewValue);
		OnStatChanged.Broadcast(StatName, NewValue);
	}
}




