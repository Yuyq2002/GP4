#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class StunnedType : uint8
{
	None		UMETA(DisplayName = "None"),
	Frozen		UMETA(DisplayName = "Frozen"),
	Stunned		UMETA(DisplayName = "Stunned"),
	Shocked		UMETA(DisplayName = "Shocked")
	
};