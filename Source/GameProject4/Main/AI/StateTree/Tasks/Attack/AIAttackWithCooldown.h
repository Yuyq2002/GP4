// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIAttack.h"
#include "AIAttackWithCooldown.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT4_API UAIAttackWithCooldown : public UAIAttack
{
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition) override;
private:
	GENERATED_BODY()
};
