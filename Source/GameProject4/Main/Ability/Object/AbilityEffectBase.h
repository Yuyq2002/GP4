// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Main/Ability/Object/AbilityData.h"
#include "AbilityEffectBase.generated.h"

class AAbilityProjectileBase;
UCLASS(Blueprintable, BlueprintType)
class GAMEPROJECT4_API AAbilityEffectBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AAbilityProjectileBase*> NewProjectiles;

	UPROPERTY()
	TArray<FEffectData> Effects;

	UPROPERTY()
	FProjectileData InheritingData;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UModifiedPlayerStats* ModifierCollection;

public:	
	UFUNCTION(BlueprintNativeEvent)
	void SetData(FEffectData EffectData, UModifiedPlayerStats* InModifierCollection);
	void SetData_Implementation(const FEffectData& EffectData, UModifiedPlayerStats* InModifierCollection);

	UFUNCTION(BlueprintNativeEvent)
	void ApplyEffect(AAbilityProjectileBase* Initiator);
	virtual void ApplyEffect_Implementation(AAbilityProjectileBase* Initiator) {};

	UFUNCTION(BlueprintCallable)
	void FinishEffect();

	UFUNCTION(BlueprintCallable)
	AAbilityProjectileBase* SpawnProjectile(TSubclassOf<AAbilityProjectileBase> Class, AActor* Initiator);

	void SetEffects(const TArray<FEffectData>& RemainingEffects);
	void SetInheritingData(FDataContainer DataContainer);

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EElement ElementType;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float LifeTime;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float Damage;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float ElementStrength;
};
