// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType, Meta = (Bitflags))
enum class EElement : uint8
{
	None,
	Physical,
	Fire,
	Ice,
	Wind,
	Lightning,
	Water,
	Light,
	Oil,

};
ENUM_CLASS_FLAGS(EElement);