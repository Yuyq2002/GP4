// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Main log category used across the project */
DECLARE_LOG_CATEGORY_EXTERN(LogGameProject4, Log, All);

#define PRINT(Message) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, Message)
