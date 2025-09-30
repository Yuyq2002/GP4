// Fill out your copyright notice in the Description page of Project Settings.


#include "RushdownEnemyController.h"

// Sets default values
ARushdownEnemyController::ARushdownEnemyController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARushdownEnemyController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARushdownEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

