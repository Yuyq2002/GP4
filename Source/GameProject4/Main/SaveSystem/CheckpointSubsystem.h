// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CheckpointSubsystem.generated.h"

class ASaveGameTrigger;
/**
 * 
 */
UCLASS()
class GAMEPROJECT4_API UCheckpointSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void RegisterCheckpoint(ASaveGameTrigger* Trigger);

	UFUNCTION( BlueprintPure)
	ASaveGameTrigger* GetSaveTrigger(int TriggerID);

	//Current checkpoint here instead of on player struct
	UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
	int32 CurrentCheckpoint = 0;

	// Server applies save/load/reset
	void CheckpointTriggered(int savepoint);
	

private:
	TArray<ASaveGameTrigger*> Checkpoints;	
};
