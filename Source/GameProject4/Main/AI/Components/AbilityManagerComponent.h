// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIAttackComponent.h"
#include "AbilityManagerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPROJECT4_API UAbilityManagerComponent : public UActorComponent
{
public:
	virtual void BeginPlay() override;

private:
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilityManagerComponent();

	UFUNCTION(BlueprintCallable)
	bool CanUseAbilities(UAIAttackComponent* Ability);

	UFUNCTION(BlueprintCallable)
	void OnAbilityUsed(UAIAttackComponent* Ability);

	UFUNCTION(BlueprintCallable)
	UAIAttackComponent* GetReadyAbility(EAIAttackType attackType);
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAIAttackComponent*> Abilities;
	
private:
	FCooldown GlobalCooldown;
};
