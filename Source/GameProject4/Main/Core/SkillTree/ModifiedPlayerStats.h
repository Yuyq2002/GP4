// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatsStruct.h"

#include "GameProject4/Main/Core/SkillTree/PlayerStatsStruct.h"
#include "ModifiedPlayerStats.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChanged, FName, StatName, float, NewValue);


// USTRUCT(BlueprintType)
// struct FStatKeySelector
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	FName SelectedKey;
// };

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPROJECT4_API UModifiedPlayerStats : public UActorComponent
{
	GENERATED_BODY()

public:
	UModifiedPlayerStats();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<FName> AvailableStatKeys;
	
	UPROPERTY()
	TMap<FName, float> BaseStats;

	UPROPERTY()
	TMap<FName, float> CurrentStats;

	UPROPERTY()
	TMap<FName, TWeakObjectPtr<UObject>> BoundComponents;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	// FStatKeySelector StatToUse;
	//
	// Stat variables
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifiedPlayerStats", meta=(AllowPrivateAccess="true"))
	// float Damage;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifiedPlayerStats", meta=(AllowPrivateAccess="true"))
	// float WalkSpeed;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifiedPlayerStats", meta=(AllowPrivateAccess="true"))
	// float ProjectileSpeed;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifiedPlayerStats")
	// FPlayerStatsStruct BaseStats;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ModifiedPlayerStats")
	// FPlayerStatsStruct CurrentStats;


public:

	UPROPERTY(BlueprintAssignable, Category="ModifiedPlayerStats")
	FOnStatChanged OnStatChanged;

	UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats")
	void ResetStatsToBase();

	UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats")
	TMap<FName, float> GetCurrentStats() const;

	UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats")
	void SetCurrentStats(const TMap<FName, float>& NewStats);

	UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats")
	float GetStat(FName StatName) const;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="ModifiedPlayerStats")
	void SetStat(FName StatName, float NewValue);
	// void HandleStatChanged(FName StatName, float NewValue);
	// UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats", meta=(ExpandBoolAsExecs="ReturnValue"))
	// bool CheckStatNameMatch(FName StatA, FName StatB);

	UFUNCTION(BlueprintCallable, Category="Stats")
	TArray<FName> GetAvailableStatKeys() const;

// UPROPERTY(BlueprintAssignable, Category="ModifiedPlayerStats")
// FOnStatChanged OnStatChanged;
//
//
// UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats")
// void ResetStatsToBase();


// UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats")
// FPlayerStatsStruct GetCurrentStats() const;

// UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats")
// void SetCurrentStats(const FPlayerStatsStruct& NewStats);
// 	
};


