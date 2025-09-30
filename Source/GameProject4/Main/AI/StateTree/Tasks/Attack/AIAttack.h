// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Main/AI/DefaultEnemyAI.h"
#include "AIAttack.generated.h"

UENUM(BlueprintType)
enum class RandomAttackState : uint8
{
	PureRandom			UMETA(DisplayName = "PureRandom"),
	WeightedRandom		UMETA(DisplayName = "WeightedRandom"),
	ChooseFirst			UMETA(DisplayName = "ChooseFirst"),
	ChooseLast			UMETA(DisplayName = "ChooseLast"),
	ChooseNumberAttack	UMETA(DisplayName = "ChooseNumberAttack")
};

/**
 * 
 */
UCLASS()
class GAMEPROJECT4_API UAIAttack : public UStateTreeTaskBlueprintBase
{
public:
	UPROPERTY(EditAnywhere, Category="Context")
	ADefaultEnemyAI* AI;

	UPROPERTY(EditAnywhere)
	RandomAttackState randomAttackState;

	UPROPERTY(EditAnywhere)
	int componentToChoose = 0;
	
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition) override;

private:
	GENERATED_BODY()
};
