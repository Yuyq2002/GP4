// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatsStruct.h"
#include "ModifiedPlayerStats.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChanged, FName, StatName, float, NewValue);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPROJECT4_API UModifiedPlayerStats : public UActorComponent
{
	GENERATED_BODY()

public:
	UModifiedPlayerStats();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Stat variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifiedPlayerStats", meta=(AllowPrivateAccess="true"))
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifiedPlayerStats", meta=(AllowPrivateAccess="true"))
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifiedPlayerStats", meta=(AllowPrivateAccess="true"))
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ModifiedPlayerStats")
	FPlayerStatsStruct BaseStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ModifiedPlayerStats")
	FPlayerStatsStruct CurrentStats;


public:
	
UPROPERTY(BlueprintAssignable, Category="ModifiedPlayerStats")
FOnStatChanged OnStatChanged;


UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats")
void ResetStatsToBase();


UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats")
FPlayerStatsStruct GetCurrentStats() const;

UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats")
void SetCurrentStats(const FPlayerStatsStruct& NewStats);
		
	// Getter and setter functions for each stat

	// UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats")
	// float GetDamage() const;
	//
	// UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats")
	// void SetDamage(float NewDamage);
	//
	// UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats")
	// float GetWalkSpeed() const;
	//
	// UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats")
	// void SetWalkSpeed(float NewWalkSpeed);
	//
	// UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats")
	// float GetProjectileSpeed() const;
	//
	// UFUNCTION(BlueprintCallable, Category="ModifiedPlayerStats")
	// void SetProjectileSpeed(float NewProjectileSpeed);
	//s
};
