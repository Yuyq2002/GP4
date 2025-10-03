// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointSubsystem.h"

#include "SaveGameTrigger.h"

void UCheckpointSubsystem::RegisterCheckpoint(ASaveGameTrigger* Trigger)
{
	Checkpoints.Add(Trigger);
}

ASaveGameTrigger* UCheckpointSubsystem::GetSaveTrigger(int TriggerID)
{
	for (ASaveGameTrigger* Checkpoint : Checkpoints)
	{
		if (Checkpoint == nullptr)
			continue;
		if (Checkpoint->thisCheckpointNumber == TriggerID)
		{
			return Checkpoint;
		}
	}
	
	return nullptr;
}

void UCheckpointSubsystem::CheckpointTriggered(int savepoint)
{
	CurrentCheckpoint = savepoint;
}
