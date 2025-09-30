// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPoint.h"

#include "Main/AI/Subsystem/PatrollpointsLookup.h"


// Sets default values
APatrolPoint::APatrolPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SetRootComponent(Spline);
	
}

// Called when the game starts or when spawned
void APatrolPoint::BeginPlay()
{
	//Spline->GetNumberOfSplinePoints()-1 - will return the num of all splinepoints programattically
	GetWorld()->GetSubsystem<UPatrollpointsLookup>()->AddPatrolPoint(this);
}
