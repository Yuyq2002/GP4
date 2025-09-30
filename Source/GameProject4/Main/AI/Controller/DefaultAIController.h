// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DefaultAIController.generated.h"

UCLASS()
class GAMEPROJECT4_API ADefaultAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	// Sets default values for this actor's properties
	ADefaultAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
