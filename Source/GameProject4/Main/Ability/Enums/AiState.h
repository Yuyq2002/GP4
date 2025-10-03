#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Patrol			UMETA(DisplayName = "Patrol"),
	Hunting			UMETA(DisplayName = "Hunting")
};
