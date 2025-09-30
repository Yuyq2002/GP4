// Fill out your copyright notice in the Description page of Project Settings.


#include "RedEnemy.h"


// Sets default values
ARedEnemy::ARedEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARedEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARedEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

