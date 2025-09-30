// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityManagerComponent.h"


void UAbilityManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	auto owner = GetOwner();

	TArray<UAIAttackComponent*> MyComponents;
	owner->GetComponents<UAIAttackComponent>(MyComponents);

	for (auto component : MyComponents)
	{
		component->AbilityManager = this;
	}

	Abilities = MyComponents;
}

// Sets default values for this component's properties
UAbilityManagerComponent::UAbilityManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UAbilityManagerComponent::OnAbilityUsed(UAIAttackComponent* Ability)
{
	GlobalCooldown.Trigger(GetWorld()->GetTimeSeconds());
	Ability->Cooldown.Trigger(GetWorld()->GetTimeSeconds());
}

UAIAttackComponent* UAbilityManagerComponent::GetReadyAbility(EAIAttackType attackType)
{
	TArray<UAIAttackComponent*> ReadyAbilities;

	for (auto ability : Abilities)
	{
		if (ability->AttackType == attackType && ability->IsLocalCooldownReady())
		{
			ReadyAbilities.Add(ability);
		}
	}
	if (ReadyAbilities.IsEmpty())
		return nullptr;

	return ReadyAbilities[FMath::RandRange(0, ReadyAbilities.Num() - 1)];
}

bool UAbilityManagerComponent::CanUseAbilities(UAIAttackComponent* Ability)
{
	return GlobalCooldown.IsReady(GetWorld()->GetTimeSeconds()) && Ability->IsLocalCooldownReady();
}
