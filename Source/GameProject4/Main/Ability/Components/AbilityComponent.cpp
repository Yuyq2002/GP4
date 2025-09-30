// Fill out your copyright notice in the Description page of Project Settings.

#include "Main/Ability/Components/AbilityComponent.h"
#include "Main/Ability/Structs/StarStats.h"
#include "Main/Ability/Object/AbilityData.h"
#include "Main/Ability/Object/AbilityProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ModifierCollection = GetOwner()->GetComponentByClass<UModifiedPlayerStats>();
}

FStarStats** UAbilityComponent::EvaluateActionToSet(EActionType ActionType)
{
	switch (ActionType)
	{
	case EActionType::Primary:
		return &PrimaryStar;
	case EActionType::Secondary:
		return &SecondaryStar;
	case EActionType::Dash:
		return &DashStar;
	default:
		return nullptr;
	}
}

FStarStats* UAbilityComponent::AddStar(EElement InElementType)
{
	if (Stars.Num() >= MaxStars) return nullptr;

	FName EnumName = FName(UEnum::GetDisplayValueAsText(InElementType).ToString());
	FDataTableStarStats* Entry = StarDataTable->FindRow<FDataTableStarStats>(EnumName, "");

	FStarStats NewStar;
	NewStar.ElementType = InElementType;
	NewStar.Icon = Entry->Icon;
	NewStar.MaxMana = Entry->Mana;
	NewStar.Mana = Entry->Mana;
	NewStar.PrimaryAbility = Entry->PrimaryAbility;
	NewStar.SecondaryAbility = Entry->SecondaryAbility;
	NewStar.DashAbility = Entry->DashAbility;
	Stars.Add(NewStar);

	return &Stars.Last();
}

void UAbilityComponent::UpdateUltimate()
{
	if (!PrimaryStar || !SecondaryStar)
		return;

	int32 Element = int32(PrimaryStar->ElementType | SecondaryStar->ElementType);
	UAbilityData* UltimateToSet = nullptr;

	for (auto& Ultimate : UltimateData->UltimateMap)
	{
		if (Ultimate.Element == Element)
		{
			UltimateToSet = Ultimate.Ultimate;

			break;
		}
	}

	ActiveUltimate.Ultimate = UltimateToSet;
}

EActionType UAbilityComponent::GetEmptyAction()
{
	if (!PrimaryStar)
		return EActionType::Primary;
	else if (!SecondaryStar)
		return EActionType::Secondary;
	else if (!DashStar)
		return EActionType::Dash;

	return EActionType::None;
}

void UAbilityComponent::SetStar_Implementation(EActionType ActionType, EElement InElementType, EActionType SwapFrom)
{
	// Check which action to bind star
	FStarStats** ActionToSet = EvaluateActionToSet(ActionType);
	if (ActionToSet == nullptr)
		return;

	// Check if an star is bind to the action
	if (*ActionToSet == nullptr && SwapFrom == EActionType::None)
	{
		// Try bind to a new star
		FStarStats* NewStar = AddStar(InElementType);
		if (!NewStar) return;
		(*ActionToSet) = NewStar;
	}
	else
	{
		FStarStats** SwapFromAction = EvaluateActionToSet(SwapFrom);
		if (!SwapFromAction)
		{
			FName EnumName = FName(UEnum::GetDisplayValueAsText(InElementType).ToString());
			FDataTableStarStats* Entry = StarDataTable->FindRow<FDataTableStarStats>(EnumName, "");

			FStarStats NewStar;
			NewStar.ElementType = InElementType;
			NewStar.Icon = Entry->Icon;
			NewStar.MaxMana = Entry->Mana;
			NewStar.Mana = Entry->Mana;
			NewStar.PrimaryAbility = Entry->PrimaryAbility;
			NewStar.SecondaryAbility = Entry->SecondaryAbility;
			NewStar.DashAbility = Entry->DashAbility;

			(**ActionToSet) = NewStar;
		}
		else
		{
			FStarStats* Temp = (*ActionToSet);
			(*ActionToSet) = (*SwapFromAction);
			(*SwapFromAction) = Temp;
			EElement SecondElement = (Temp) ? (*Temp).ElementType : EElement::None;
		}
	}

	//UpdateUltimate();

	TArray<FStarStats> NewList = { PrimaryStar ? *PrimaryStar : FStarStats(), SecondaryStar ? *SecondaryStar : FStarStats(), DashStar ? *DashStar : FStarStats() };
	SyncStarStats(NewList);
}

EActionType UAbilityComponent::FindActionTypeFromElement(EElement InElementType)
{
	if (PrimaryStar)
		if (PrimaryStar->ElementType == InElementType)
			return EActionType::Primary;

	if (SecondaryStar)
		if (SecondaryStar->ElementType == InElementType)
			return EActionType::Secondary;

	if (DashStar)
		if (DashStar->ElementType == InElementType)
			return EActionType::Dash;

	return EActionType::None;
}

EElement UAbilityComponent::FindElementFromActionType(EActionType InActionType)
{
	switch (InActionType)
	{
	case EActionType::Primary:
		return PrimaryStar ? PrimaryStar->ElementType : EElement::None;
		break;
	case EActionType::Secondary:
		return SecondaryStar ? SecondaryStar->ElementType : EElement::None;
		break;
	case EActionType::Dash:
		return DashStar ? DashStar->ElementType : EElement::None;
		break;
	default:
		return EElement::None;
	}
}

void UAbilityComponent::RotateElement_Implementation()
{
	RotateKeyDown = false;
	if (Timer <= 0 || Stars.Num() <= 0) return;

	FStarStats* Temp = DashStar;

	DashStar = SecondaryStar;
	SecondaryStar = PrimaryStar;
	PrimaryStar = Temp;

	//UpdateUltimate();

	SyncRotation();
}

void UAbilityComponent::StartRechargeTimer_Implementation()
{
	Timer = RotateThreshold;
	RotateKeyDown = true;
}

void UAbilityComponent::SyncRotation_Implementation()
{
	if (!GetOwner()->HasAuthority())
	{
		FStarStats* Temp = DashStar; 

		DashStar = SecondaryStar;
		SecondaryStar = PrimaryStar;
		PrimaryStar = Temp;
	}

	OnStarRotatedDelegate.Broadcast(PrimaryStar ? PrimaryStar->ElementType : EElement::None);
}

void UAbilityComponent::SyncStarStats_Implementation(const TArray<FStarStats>& NewList)
{
	if (!GetOwner()->HasAuthority())
	{
		Stars = NewList;

		PrimaryStar = Stars[0].ElementType != EElement::None ? &Stars[0] : nullptr;
		SecondaryStar = Stars[1].ElementType != EElement::None ? &Stars[1] : nullptr;
		DashStar = Stars[2].ElementType != EElement::None ? &Stars[2] : nullptr;
	}

	OnStarChangedDelegate.Broadcast(NewList);
}

void UAbilityComponent::SyncUltimate_Implementation(EElement Primary, EElement Secondary)
{
	OnUltimateChangedDelegate.Broadcast(Primary, Secondary);
}

void UAbilityComponent::PrimaryAction_Implementation()
{

	if (!PrimaryStar || !(PrimaryStar->PrimaryAbility))
	{
		return;
	}

	if (PrimaryCooldown <= 0 && PrimaryStar->Mana >= PrimaryStar->PrimaryAbility->ManaCost)
	{
		AActor* Parent = GetOwner();
		AAbilityProjectileBase* NewProjectile = GetWorld()->SpawnActor<AAbilityProjectileBase>(PrimaryStar->PrimaryAbility->Projectile, Parent->GetActorLocation(), Parent->GetActorForwardVector().Rotation());
		NewProjectile->SetEffects(PrimaryStar->PrimaryAbility->Effects);
		NewProjectile->SetupProjectile(Parent, ModifierCollection, PrimaryStar->PrimaryAbility->ProjectileData);
		PrimaryStar->Mana -= PrimaryStar->PrimaryAbility->ManaCost;
		PrimaryStar->Mana = FMath::Max(0, PrimaryStar->Mana);
		PrimaryCooldown = PrimaryStar->PrimaryAbility->Cooldown;

		TArray<FStarStats> NewList = { PrimaryStar ? *PrimaryStar : FStarStats(), SecondaryStar ? *SecondaryStar : FStarStats(), DashStar ? *DashStar : FStarStats() };
		SyncStarStats(NewList);
	}
}

void UAbilityComponent::SecondaryAction_Implementation()
{
	if (!SecondaryStar || !(SecondaryStar->SecondaryAbility))
	{
		return;
	}

	if (SecondaryCooldown <= 0 && SecondaryStar->Mana >= SecondaryStar->SecondaryAbility->ManaCost)
	{
		AActor* Parent = GetOwner();
		AAbilityProjectileBase* NewProjectile = GetWorld()->SpawnActor<AAbilityProjectileBase>(SecondaryStar->SecondaryAbility->Projectile, Parent->GetActorLocation(), Parent->GetActorForwardVector().Rotation());
		NewProjectile->SetEffects(SecondaryStar->SecondaryAbility->Effects);
		NewProjectile->SetupProjectile(Parent, ModifierCollection, SecondaryStar->SecondaryAbility->ProjectileData);
		SecondaryStar->Mana -= SecondaryStar->SecondaryAbility->ManaCost;
		SecondaryStar->Mana = FMath::Max(0, SecondaryStar->Mana);
		SecondaryCooldown = SecondaryStar->SecondaryAbility->Cooldown;

		TArray<FStarStats> NewList = { PrimaryStar ? *PrimaryStar : FStarStats(), SecondaryStar ? *SecondaryStar : FStarStats(), DashStar ? *DashStar : FStarStats() };
		SyncStarStats(NewList);
	}
}

void UAbilityComponent::DashAction_Implementation()
{
	if (!DashStar || !(DashStar->DashAbility))
	{
		return;
	}

	if (DashCooldown <= 0 && DashStar->Mana >= DashStar->DashAbility->ManaCost)
	{
		AActor* Parent = GetOwner();
		AAbilityProjectileBase* NewProjectile = GetWorld()->SpawnActor<AAbilityProjectileBase>(DashStar->DashAbility->Projectile, Parent->GetActorLocation(), Parent->GetActorForwardVector().Rotation());
		NewProjectile->SetEffects(DashStar->DashAbility->Effects);
		NewProjectile->SetupProjectile(Parent, ModifierCollection, DashStar->DashAbility->ProjectileData);
		DashStar->Mana -= DashStar->DashAbility->ManaCost;
		DashStar->Mana = FMath::Max(0, DashStar->Mana);
		DashCooldown = DashStar->DashAbility->Cooldown;

		TArray<FStarStats> NewList = { PrimaryStar ? *PrimaryStar : FStarStats(), SecondaryStar ? *SecondaryStar : FStarStats(), DashStar ? *DashStar : FStarStats() };
		SyncStarStats(NewList);
	}
}

void UAbilityComponent::DualElementAction_Implementation()
{
	if (ActiveUltimate.Ultimate == nullptr || ActiveUltimate.Gauge <= UltimateData->MaxGauge)
		return;

	ActiveUltimate.Gauge = 0;
	AActor* Parent = GetOwner();
	AAbilityProjectileBase* NewProjectile = GetWorld()->SpawnActor<AAbilityProjectileBase>(ActiveUltimate.Ultimate->Projectile, Parent->GetActorLocation(), Parent->GetActorForwardVector().Rotation());
	NewProjectile->SetEffects(ActiveUltimate.Ultimate->Effects);
	NewProjectile->SetupProjectile(Parent, ModifierCollection, ActiveUltimate.Ultimate->ProjectileData);
}


// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PrimaryCooldown -= DeltaTime;
	SecondaryCooldown -= DeltaTime;
	DashCooldown -= DeltaTime;

	if (RotateKeyDown)
	{
		if (Timer > 0)
			Timer -= DeltaTime;
		else
		{
			for(auto& Star : Stars)
				Star.Mana = FMath::Min(Star.Mana + (DeltaTime * RechargeRate), Star.MaxMana);

			TArray<FStarStats> NewList = { PrimaryStar ? *PrimaryStar : FStarStats(), SecondaryStar ? *SecondaryStar : FStarStats(), DashStar ? *DashStar : FStarStats() };
			SyncStarStats(NewList);
		}
	}

}