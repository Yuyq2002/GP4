// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerSaveState.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT4_API APlayerSaveState : public APlayerState
{
	GENERATED_BODY()

public:
	APlayerSaveState();


protected:
	virtual void BeginPlay() override;

	
};
