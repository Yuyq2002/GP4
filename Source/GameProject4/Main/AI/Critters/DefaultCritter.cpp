// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultCritter.h"


// Sets default values
ADefaultCritter::ADefaultCritter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

bool ADefaultCritter::IsCloseToPlayer()
{
	if (UWorld* World = GetWorld())
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
			if (APlayerController* PC = Iterator->Get(); PC && PC->GetPawn())
				if (APawn* Pawn = PC->GetPawn(); GetDistanceTo(Pawn) < MaxDistanceToPlayerBeforeToClose)
					return true;
	return false;
}

