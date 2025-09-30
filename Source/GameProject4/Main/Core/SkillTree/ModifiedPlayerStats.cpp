// Fill out your copyright notice in the Description page of Project Settings.


#include "ModifiedPlayerStats.h"


UModifiedPlayerStats::UModifiedPlayerStats()
{
	PrimaryComponentTick.bCanEverTick = false;

	BaseStats.Damage = 10.0f;
	BaseStats.WalkSpeed = 600.0f;
	BaseStats.ProjectileSpeed = 1000.0f;

	CurrentStats = BaseStats;

	// ...
}


void UModifiedPlayerStats::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UModifiedPlayerStats::ResetStatsToBase()
{
	CurrentStats = BaseStats;
	OnStatChanged.Broadcast("Damage", CurrentStats.Damage);
	OnStatChanged.Broadcast("WalkSpeed", CurrentStats.WalkSpeed);
	OnStatChanged.Broadcast("ProjectileSpeed", CurrentStats.ProjectileSpeed);
}

FPlayerStatsStruct UModifiedPlayerStats::GetCurrentStats() const
{
	return CurrentStats;
}

void UModifiedPlayerStats::SetCurrentStats(const FPlayerStatsStruct& NewStats)
{
	if (CurrentStats.Damage != NewStats.Damage)
		OnStatChanged.Broadcast("Damage", NewStats.Damage);
	if (CurrentStats.WalkSpeed != NewStats.WalkSpeed)
		OnStatChanged.Broadcast("WalkSpeed", NewStats.WalkSpeed);
	if (CurrentStats.ProjectileSpeed != NewStats.ProjectileSpeed)
		OnStatChanged.Broadcast("ProjectileSpeed", NewStats.ProjectileSpeed);

	CurrentStats = NewStats;
}

// float UModifiedPlayerStats::GetDamage() const
// {
// 	return Damage;
// }
//
// void UModifiedPlayerStats::SetDamage(float NewDamage)
// {
// 	if (Damage != NewDamage)
// 	{
// 		Damage = NewDamage;
// 		OnStatChanged.Broadcast("Damage", Damage);
// 	}
// }
//
// float UModifiedPlayerStats::GetWalkSpeed() const
// {
// 	return WalkSpeed;
// }
//
// void UModifiedPlayerStats::SetWalkSpeed(float NewWalkSpeed)
// {
// 	if (WalkSpeed != NewWalkSpeed)
// 	{
// 		WalkSpeed = NewWalkSpeed;
// 		OnStatChanged.Broadcast("WalkSpeed", WalkSpeed);
// 	}
// }
//
// float UModifiedPlayerStats::GetProjectileSpeed() const
// {
// 	return ProjectileSpeed;
// }
//
// void UModifiedPlayerStats::SetProjectileSpeed(float NewProjectileSpeed)
// {
// 	if (ProjectileSpeed != NewProjectileSpeed)
// 	{
// 		ProjectileSpeed = NewProjectileSpeed;
// 		OnStatChanged.Broadcast("ProjectileSpeed", ProjectileSpeed);
// 	}
// }


