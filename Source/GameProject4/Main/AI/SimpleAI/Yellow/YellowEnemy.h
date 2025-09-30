// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/AI/SimpleAI/Green/GreenEnemy.h"
#include "YellowEnemy.generated.h"

UCLASS()
class GAMEPROJECT4_API AYellowEnemy : public AGreenEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AYellowEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
