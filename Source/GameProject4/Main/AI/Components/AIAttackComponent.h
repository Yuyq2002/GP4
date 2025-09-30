// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIAttackComponent.generated.h"

class UAbilityManagerComponent;

USTRUCT(BlueprintType)
struct FCooldown
{
	GENERATED_BODY()

	float Duration = 0.f;
	float LastUsedTime = -FLT_MAX;

	bool IsReady(float CurrentTime) const
	{
		return CurrentTime - LastUsedTime >= Duration;
	}

	void Trigger(float CurrentTime)
	{
		LastUsedTime = CurrentTime;
	}
};

UENUM(BlueprintType)
enum class EAIAttackType : uint8
{
	NormalMelee				UMETA(DisplayName = "NormalMelee"),
	NormalRanged			UMETA(DisplayName = "NormalRanged"),
	AbilityMelee			UMETA(DisplayName = "AbilityMelee"),
	AbilityRanged			UMETA(DisplayName = "AbilityRanged")
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPROJECT4_API UAIAttackComponent : public UActorComponent
{
public:
	virtual void BeginPlay() override;

private:
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAIAttackComponent();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Enemy")
	void DoAttack();

	UFUNCTION(BlueprintCallable, Category="Attack")
	bool TryActivate();
	
	UFUNCTION(BlueprintCallable, Category="Timer")
	bool IsLocalCooldownReady();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackType")
	EAIAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackType")
	float CooldownTime = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Management")
	UAbilityManagerComponent* AbilityManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Management")
	FCooldown Cooldown;
};
