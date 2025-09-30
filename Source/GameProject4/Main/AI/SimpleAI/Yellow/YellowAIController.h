// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/AI/SimpleAI/Green/GreenAIController.h"
#include "YellowAIController.generated.h"

UCLASS()
class GAMEPROJECT4_API AYellowAIController : public AGreenAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AYellowAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
