#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETargetInRange : uint8
{
	Far			UMETA(DisplayName = "Far"),
	Close		UMETA(DisplayName = "Close"),
	InRange		UMETA(DisplayName = "InRange")
};