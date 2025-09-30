// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/AI/DefaultEnemyAI.h"
#include "Brute.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdatePlan);

UCLASS()
class GAMEPROJECT4_API ABrute : public ADefaultEnemyAI
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABrute();

	UFUNCTION(BlueprintCallable, Category="AI|Planning")
	void UpdatePlan();

	UFUNCTION(BlueprintCallable, Category="AI|Planning")
	void StartAbilityCooldown();

	UFUNCTION(BlueprintCallable)
	void AbilityOffCooldown();

	UFUNCTION(BlueprintCallable, Category="AI|Planning")
	void SetTimeBetweenAbilities(float time);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Brute|Planning")
	int currentPlan = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Brute|Planning")
	bool shouldUseAbility = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Brute|Planning")
	float timeBetweenAbility = 5.0f;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnUpdatePlan OnUpdatePlan;

private:

	UPROPERTY()
	FTimerHandle AbilityCooldownTimer;
};
