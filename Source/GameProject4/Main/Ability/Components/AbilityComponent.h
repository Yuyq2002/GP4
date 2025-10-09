// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/Ability/Enums/ActionType.h"
#include "Main/Ability/Enums/EffectElementType.h"
#include "Main/Ability/Structs/StarStats.h"
#include "Components/ActorComponent.h"
#include "Main/Core/SkillTree/ModifiedPlayerStats.h"

#include "AbilityComponent.generated.h"


class UAbilityData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPROJECT4_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStarChangedSignature, const TArray<FStarStats>&, NewList);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStarRotatedSignature, EElement, PrimaryElement, EElement, SecondaryElement);

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	FStarStats** EvaluateActionToSet(EActionType ActionType);
	FStarStats* AddStar(EElement InElementType);

	UFUNCTION(BlueprintCallable)
	EActionType GetEmptyAction();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetStar(EActionType ActionType, EElement InElementType, EActionType SwapFrom = EActionType::None);

	UFUNCTION(BlueprintCallable)
	EActionType FindActionTypeFromElement(EElement InElementType);

	UFUNCTION(BlueprintCallable)
	EElement FindElementFromActionType(EActionType InActionType);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void RotateElement();

	UFUNCTION(Client, Reliable)
	void SyncRotation();

	UFUNCTION(Client, Reliable)
	void SyncStarStats(const TArray<FStarStats>& NewList);

	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void PrimaryAction();

	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void SecondaryAction();

	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void DashAction();

	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void DualElementAction();

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnStarChangedSignature OnStarChangedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnStarRotatedSignature OnStarRotatedDelegate;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	UDataTable* StarDataTable;

	UPROPERTY()
	UModifiedPlayerStats* ModifierCollection;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FStarStats> Stars;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float RechargeRate;

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<EElement> UnlockedElements;

private:
	int MaxStars = 3;
	FStarStats *PrimaryStar = nullptr, *SecondaryStar = nullptr, *DashStar = nullptr;
	float PrimaryCooldown = 0, SecondaryCooldown = 0, DashCooldown = 0;
};
