// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Debug.generated.h"

UENUM(BlueprintType)
enum class TaskSuccessStatus : uint8
{
	Success			UMETA(DisplayName = "Success"),
	Failure			UMETA(DisplayName = "failure"),
	Nothing         UMETA(DisplayName = "Nothing")
};

/**
 * 
 */
UCLASS(Category = "Debug")
class GAMEPROJECT4_API UDebug : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	EStateTreeRunStatus EnterState ( FStateTreeExecutionContext& Context,
const FStateTreeTransitionResult& Transition
) override;

	UPROPERTY(EditAnywhere)
	bool PrintTextToScreen = false;

	UPROPERTY(EditAnywhere)
	FString textToPrint;

	UPROPERTY(EditAnywhere)
	FColor textColor = FColor::Green;

	UPROPERTY(EditAnywhere)
	float timeToDisplayTextSeconds = 5.0f;

	UPROPERTY(EditAnywhere)
	bool PrintTextToConsole = false;

	

	UPROPERTY(EditAnywhere)
	TaskSuccessStatus TaskSuccessStatus = TaskSuccessStatus::Nothing;
};
