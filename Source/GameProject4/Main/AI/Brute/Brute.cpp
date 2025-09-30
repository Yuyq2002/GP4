// Fill out your copyright notice in the Description page of Project Settings.


#include "Brute.h"


// Sets default values
ABrute::ABrute()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABrute::UpdatePlan()
{
	if (!IsHunting)
	{
		currentPlan = 0; //Idle
		return;
	}
	if (shouldUseAbility)
	{
		switch (auto rangeToTarget = GetAggroRange())
		{
		case AggroRangeStatus::TargetIsClose:
		case AggroRangeStatus::TargetInRange:
				currentPlan = 20; //Melee ability plan
			break;
		case AggroRangeStatus::TargetNotInRange:
				currentPlan = 10; //far ability plan
			break;
		case AggroRangeStatus::TargetIsNull:
			currentPlan = 0;
			break;
		}
	}
	else
	{
		switch (auto rangeToTarget = GetAggroRange())
		{
		case AggroRangeStatus::TargetIsClose:
		case AggroRangeStatus::TargetInRange:
			currentPlan = 2; //Attack plan
			break;
		case AggroRangeStatus::TargetNotInRange:
			currentPlan = 1; //Move to player plan
			break;

		case AggroRangeStatus::TargetIsNull:
			currentPlan = 0; //Idle
			break;
		}
	}
}

void ABrute::StartAbilityCooldown()
{
	GetWorld()->GetTimerManager().SetTimer(
	AbilityCooldownTimer,                   // handle to manage the timer
	this,                          // object that has the function
	&ABrute::AbilityOffCooldown,   // function to call
	timeBetweenAbility,            // time between calls
	false,                         // loop?
	0.0f                          // initial delay
	);

	shouldUseAbility = false;
}

void ABrute::AbilityOffCooldown()
{
	shouldUseAbility = true;
	OnUpdatePlan.Broadcast();
}

void ABrute::SetTimeBetweenAbilities(float time)
{
	timeBetweenAbility = time;
}
