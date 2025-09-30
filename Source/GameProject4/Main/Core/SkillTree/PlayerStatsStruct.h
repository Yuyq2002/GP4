// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStatsStruct.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStatsStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Stats", meta = (Categories = "Player Stats"))
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Stats", meta = (Categories = "Player Stats"))
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Stats", meta = (Categories = "Player Stats"))
	float ProjectileSpeed = 1000.0f;
};
