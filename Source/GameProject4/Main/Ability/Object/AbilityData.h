// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Main/Ability/Enums/EffectElementType.h"
#include "AbilityData.generated.h"

class AAbilityProjectileBase;
class AAbilityEffectBase;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FIntData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int Value;
};

USTRUCT(BlueprintType)
struct FFloatData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Value;

	FFloatData() = default;
	FFloatData(FString InName, float InValue)
	{
		Name = InName;
		Value = InValue;
	}
};

USTRUCT(BlueprintType)
struct FVectorData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector Value;
};

USTRUCT(BlueprintType)
struct FDataContainer
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FIntData> IntData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FFloatData> FloatData = {FFloatData("LifeTime", 0), FFloatData("Damage", 0), FFloatData("ElementStrength", 0)};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FVectorData> VectorData;
};

USTRUCT(BlueprintType)
struct FProjectileData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EElement ElementType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FDataContainer Data;
};

USTRUCT(BlueprintType)
struct FEffectData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AAbilityEffectBase> Effect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EElement ElementType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FDataContainer Data;
};


UCLASS()
class GAMEPROJECT4_API UAbilityData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AAbilityProjectileBase> Projectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FProjectileData ProjectileData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ManaCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Cooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FEffectData> Effects;
};

/* Actual Interface declaration. */
UCLASS()
class UDataFetcher : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static int GetIntDataByName(FString Name, const TArray<FIntData>& IntArray);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float GetFloatDataByName(FString Name, const TArray<FFloatData>& FloatArray);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FVector GetVectorDataByName(FString Name, const TArray<FVectorData>& VectorArray);

};