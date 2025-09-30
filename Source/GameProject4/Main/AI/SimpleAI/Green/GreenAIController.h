// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/AI/SimpleAI/Default/SimpleAiController.h"
#include "GreenAIController.generated.h"

UCLASS()
class GAMEPROJECT4_API AGreenAIController : public ASimpleAiController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGreenAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
