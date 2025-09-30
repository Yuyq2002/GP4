// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/AI/Components/PatrolPoint.h"
#include "Subsystems/WorldSubsystem.h"
#include "PatrollpointsLookup.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT4_API UPatrollpointsLookup : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TMap<int, APatrolPoint*> PatrolPoints;

	UFUNCTION(BlueprintCallable)
	void AddPatrolPoint(APatrolPoint* PatrolPoint);

	UFUNCTION(BlueprintCallable)
	APatrolPoint* GetPatrolPoint(int i);
};
