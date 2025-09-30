// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAttack.h"

#include "Main/AI/Components/AIAttackComponent.h"

EStateTreeRunStatus UAIAttack::EnterState(FStateTreeExecutionContext& Context,
                                          const FStateTreeTransitionResult& Transition)
{

	
	TArray<UAIAttackComponent*> AIAttackComponents;
	AI->GetComponents<UAIAttackComponent>(AIAttackComponents);

	/*for (int i = 0; i < AIAttackComponents.Num(); i++)
	{
		if (AIAttackComponents[i]->IsAbility)
		{
			AIAttackComponents.RemoveAt(i);
			
		}
	}*/

	if (AIAttackComponents.Num() == 0)
	{
		FinishTask(false);
		return EStateTreeRunStatus::Failed;
	}
	int32 RandRange = 0;

	AI->SetAttackState_Implementation(AIAttackState::InAttack);
	switch (randomAttackState)
	{
	case RandomAttackState::PureRandom:
		RandRange = FMath::RandRange(0, AIAttackComponents.Num()-1);
		AIAttackComponents[RandRange]->DoAttack();
		break;
	case RandomAttackState::WeightedRandom:
		UE_LOG(LogTemp, Error, TEXT("NOT IMPLEMENTED YET"));
		FinishTask(false);
		return EStateTreeRunStatus::Failed;
	case RandomAttackState::ChooseFirst:
		AIAttackComponents[0]->DoAttack();
		break;
	case RandomAttackState::ChooseLast:
		AIAttackComponents[AIAttackComponents.Num()-1]->DoAttack();
		break;
	case RandomAttackState::ChooseNumberAttack:
		if (componentToChoose >=AIAttackComponents.Num() || componentToChoose < 0)
		{
			FinishTask(false);
			return EStateTreeRunStatus::Failed;
		}
		AIAttackComponents[componentToChoose]->DoAttack();
		break;
	}
	
	

	FinishTask(true);
	return EStateTreeRunStatus::Succeeded;
}
