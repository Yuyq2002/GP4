// Fill out your copyright notice in the Description page of Project Settings.


#include "BigBoss.h"


// Sets default values
ABigBoss::ABigBoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABigBoss::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABigBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABigBoss::ChangePhase(const int i) const
{
	BossPhaseChanged.Broadcast(i);
}
