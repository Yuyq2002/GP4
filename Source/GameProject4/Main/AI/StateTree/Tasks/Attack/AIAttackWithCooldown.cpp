// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAttackWithCooldown.h"

#include "AIAttack.h"
#include "Main/AI/DefaultEnemyAI.h"
#include "Main/AI/Components/AIAttackComponent.h"

EStateTreeRunStatus UAIAttackWithCooldown::EnterState(FStateTreeExecutionContext& Context,
                                                      const FStateTreeTransitionResult& Transition)
{
	
	FinishTask(true);
	return EStateTreeRunStatus::Succeeded;
}
