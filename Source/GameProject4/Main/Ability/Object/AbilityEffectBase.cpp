// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Ability/Object/AbilityEffectBase.h"
#include "Main/Ability/Object/AbilityProjectileBase.h"

void AAbilityEffectBase::SetData_Implementation(const FEffectData& EffectData, UModifiedPlayerStats* InModifierCollection)
{
	ModifierCollection = InModifierCollection;

	LifeTime = UDataFetcher::GetFloatDataByName("LifeTime", EffectData.Data.FloatData);
	Damage = UDataFetcher::GetFloatDataByName("Damage", EffectData.Data.FloatData);
	ElementStrength = UDataFetcher::GetFloatDataByName("ElementStrength", EffectData.Data.FloatData);
	ElementType = EffectData.ElementType;
}

UFUNCTION(BlueprintCallable)
void AAbilityEffectBase::FinishEffect()
{
	for (auto& NewProjectile : NewProjectiles)
		NewProjectile->SetEffects(Effects);

	Destroy();
}

AAbilityProjectileBase* AAbilityEffectBase::SpawnProjectile(TSubclassOf<AAbilityProjectileBase> Class, AActor* Initiator)
{
	AAbilityProjectileBase* NewProjectile = GetWorld()->SpawnActor<AAbilityProjectileBase>(Class);
	NewProjectile->SetupProjectile(Initiator, ModifierCollection, InheritingData);
	NewProjectiles.Add(NewProjectile);
	return NewProjectile;
}

void AAbilityEffectBase::SetEffects(const TArray<FEffectData>& RemainingEffects)
{
	Effects = RemainingEffects;
}

void AAbilityEffectBase::SetInheritingData(FDataContainer DataContainer)
{
	FProjectileData Data;
	Data.Data = DataContainer;
	InheritingData = Data;
}
