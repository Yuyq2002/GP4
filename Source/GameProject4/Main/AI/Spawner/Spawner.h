// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Main/AI/SimpleAI/Default/SimpleDefaultAI.h"
#include "Spawner.generated.h"

UCLASS()
class GAMEPROJECT4_API ASpawner : public AActor
{
private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawner();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION(BlueprintCallable, Category = "UnitSpawning")
	void StartSpawning();
	
	UFUNCTION(BlueprintCallable, Category = "UnitSpawning")
	void StopSpawning();
	
	UFUNCTION()
	void SpawnUnits();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<ASimpleDefaultAI> UnitToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data",  meta=(ClampMin="0", UIMin="0"))
	int AmountOfUnitsToSpawn = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	float InitialSpawnDelay = 0.0f;

	UPROPERTY()
	bool bShowSpawnInterval = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data", meta=(EditCondition="bShowSpawnInterval"))
	float SpawnInterval = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	bool ShouldSpawnHunting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data", meta=(EditCondition="ShouldSpawnHunting"))
	bool ShouldHuntAfterTime = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data", meta=(EditCondition="ShouldHuntAfterTime"))
	float ActivateUnitTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	bool spawnOnBegin = false;

	FTimerHandle SpawnTimer;
	FTimerHandle HuntTimer;
};
