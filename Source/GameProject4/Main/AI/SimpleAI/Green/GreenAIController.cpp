// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenAIController.h"


// Sets default values
AGreenAIController::AGreenAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGreenAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGreenAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

