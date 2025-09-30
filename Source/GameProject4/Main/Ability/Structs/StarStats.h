// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/Ability/Enums/EffectElementType.h"
#include "Main/Ability/Object/AbilityData.h"

#include "StarStats.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FStarStats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	EElement ElementType;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadOnly)
	float MaxMana;

	UPROPERTY(BlueprintReadWrite)
	float Mana;  

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAbilityData* PrimaryAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAbilityData* SecondaryAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAbilityData* DashAbility;
};

USTRUCT(BlueprintType)
struct FDataTableStarStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Mana;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EElement ElementType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAbilityData* PrimaryAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAbilityData* SecondaryAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAbilityData* DashAbility;
};

USTRUCT(BlueprintType)
struct FActiveUltimate
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float Gauge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAbilityData* Ultimate;
};

USTRUCT(BlueprintType)
struct FElementWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = EElement))
	int32 Element;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = EElement))
	UAbilityData* Ultimate;
};

UCLASS(BlueprintType)
class UUltimateData : public UDataAsset
{
	GENERATED_BODY()
	 
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxGauge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FElementWrapper> UltimateMap;
};