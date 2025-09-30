// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug.h"

EStateTreeRunStatus UDebug::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	if (PrintTextToScreen)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,              // Key (use -1 for a new line every time)
				timeToDisplayTextSeconds,            // Time to display (seconds)
				textColor,   // Text color
				textToPrint// Message
			);
		}
	}

	if (PrintTextToConsole)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *textToPrint);
	}

	switch (TaskSuccessStatus)
	{
		case TaskSuccessStatus::Success:
		FinishTask(true);
		return EStateTreeRunStatus::Succeeded;
		case TaskSuccessStatus::Failure:
		FinishTask(false);
		return EStateTreeRunStatus::Failed;
	case TaskSuccessStatus::Nothing:
	default: ;
	}
	
	return Super::EnterState(Context, Transition);
}
