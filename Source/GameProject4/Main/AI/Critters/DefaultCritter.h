// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DefaultCritter.generated.h"

UCLASS()
class GAMEPROJECT4_API ADefaultCritter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADefaultCritter();
	
	UFUNCTION(BlueprintCallable, Category = "Critter|Behaviour")
	bool IsCloseToPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Critter|Behaviour")
	float MaxDistanceToPlayerBeforeToClose = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Critter|Behaviour")
	float IdleTime = 5.0f;
};
