// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "PatrolPoint.generated.h"

UCLASS()
class GAMEPROJECT4_API APatrolPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APatrolPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:
	UPROPERTY(VisibleAnywhere, Category="Spline")
	USplineComponent* Spline;

	//for the lookup
	UPROPERTY(EditAnywhere)
	int id = 0;
};
