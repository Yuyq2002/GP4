// Fill out your copyright notice in the Description page of Project Settings.


#include "RushdownEnemy.h"


// Sets default values
ARushdownEnemy::ARushdownEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARushdownEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARushdownEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARushdownEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

