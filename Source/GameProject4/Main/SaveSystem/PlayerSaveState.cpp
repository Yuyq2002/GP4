// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSaveState.h"
#include "Net/UnrealNetwork.h"

APlayerSaveState::APlayerSaveState()
{
	bReplicates = true;
}

void APlayerSaveState::BeginPlay()
{
	Super::BeginPlay();


}

