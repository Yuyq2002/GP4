#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPatrolBehaviour : uint8
{
	RoundRobin			UMETA(DisplayName = "RoundRobin"),
	BackAndForth		UMETA(DisplayName = "BackAndForth"),
	RandomIndex			UMETA(DisplayName = "RandomIndex")
};