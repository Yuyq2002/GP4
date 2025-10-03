// Fill out your copyright notice in the Description page of Project Settings.


#include "YellowEnemy.h"

#include "Kismet/GameplayStatics.h"


void AYellowEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!ShouldAimAtPlayer) return;

	if (auto TargetPlayer = worldState.ClosestPlayer)
	{
		FVector PlayerLoc = TargetPlayer->GetActorLocation();
		FVector PlayerVel = TargetPlayer->GetVelocity();

		// Predict where the player will be after some time
		float PredictionTime = 1.0f; // seconds into the future
		FVector PredictedLoc = PlayerLoc + PlayerVel * PredictionTime;

		// Direction from enemy to predicted location
		FVector ToTarget = (PredictedLoc - GetActorLocation()).GetSafeNormal2D();

		if (!ToTarget.IsNearlyZero())
		{
			FRotator TargetRot = ToTarget.Rotation();
			FRotator CurrentRot = GetActorRotation();

			// Smooth interpolation
			FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, RotationSpeed);

			SetActorRotation(NewRot);
		}
	}
}

// Sets default values
AYellowEnemy::AYellowEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AYellowEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

