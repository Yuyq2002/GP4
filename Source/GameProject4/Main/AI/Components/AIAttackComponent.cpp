// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAttackComponent.h"
#include "AbilityManagerComponent.h"


void UAIAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	Cooldown.Duration = CooldownTime;
}

// Sets default values for this component's properties
UAIAttackComponent::UAIAttackComponent(): AttackType(), AbilityManager(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UAIAttackComponent::TryActivate()
{
	if (AbilityManager && AbilityManager->CanUseAbilities(this))
	{
		DoAttack();
		Cooldown.Trigger(GetWorld()->GetTimeSeconds());
		AbilityManager->OnAbilityUsed(this);
		return true;
	}
	return false;
}

bool UAIAttackComponent::IsLocalCooldownReady()
{
	return Cooldown.IsReady(GetWorld()->GetTimeSeconds());
}
