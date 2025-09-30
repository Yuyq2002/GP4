// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrollpointsLookup.h"

void UPatrollpointsLookup::AddPatrolPoint(APatrolPoint* PatrolPoint)
{
	if (!PatrolPoints.Contains(PatrolPoint->id))
	{
		PatrolPoints.Add(PatrolPoint->id, PatrolPoint);
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Patrol point already exists in map ID: %i"), PatrolPoint->id);
		GEngine->AddOnScreenDebugMessage(
							-1,
							5.f,
							FColor::Yellow,
							FString::Printf(TEXT("Patrol point already exists in map ID: %i"), PatrolPoint->id)
						);
	}
}

APatrolPoint* UPatrollpointsLookup::GetPatrolPoint(int i)
{
	return PatrolPoints.FindRef(i);
}
