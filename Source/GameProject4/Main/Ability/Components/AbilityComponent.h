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
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStarRotatedSignature, EElement, PrimaryElement);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUltimateChangedSignature, EElement, PrimaryElement, EElement, SecondaryElement);

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	FStarStats** EvaluateActionToSet(EActionType ActionType);
	FStarStats* AddStar(EElement InElementType);
	void UpdateUltimate();

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

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void StartRechargeTimer();

	UFUNCTION(Client, Reliable)
	void SyncRotation();

	UFUNCTION(Client, Reliable)
	void SyncStarStats(const TArray<FStarStats>& NewList);

	UFUNCTION(Client, Reliable)
	void SyncUltimate(EElement Primary, EElement Secondary);

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

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnUltimateChangedSignature OnUltimateChangedDelegate;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	UDataTable* StarDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UUltimateData* UltimateData;

	UPROPERTY()
	UModifiedPlayerStats* ModifierCollection;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FStarStats> Stars;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float RechargeRate;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float RotateThreshold;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<EElement> UnlockedElements;

	FActiveUltimate ActiveUltimate;

private:
	int MaxStars = 3;
	FStarStats *PrimaryStar = nullptr, *SecondaryStar = nullptr, *DashStar = nullptr;
	float PrimaryCooldown = 0, SecondaryCooldown = 0, DashCooldown = 0;
	float Timer;
	bool RotateKeyDown = false;
};
