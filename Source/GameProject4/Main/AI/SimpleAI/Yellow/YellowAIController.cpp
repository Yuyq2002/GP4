// Fill out your copyright notice in the Description page of Project Settings.


#include "YellowAIController.h"


// Sets default values
AYellowAIController::AYellowAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AYellowAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AYellowAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

