// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Main/AI/SimpleAI/Default/SimpleDefaultAI.h"
#include "Spawner.generated.h"

UENUM(BlueprintType)
enum class ESpawnLogic : uint8
{
	Single			UMETA(DisplayName = "Single"),
	Interval		UMETA(DisplayName = "Interval"),
	Chunk			UMETA(DisplayName = "Chunk")
};

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

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "UnitSpawning")
	void StartSpawning();
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "UnitSpawning")
	void StopSpawning();
	
	UFUNCTION()
	void SpawnUnits();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<ASimpleDefaultAI> UnitToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<ASimpleAiController> IfNeededAiController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	ESpawnLogic SpawnLogic;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Data",  meta=(ClampMin="0", UIMin="0"))
	int AmountOfUnitsToSpawn;

	UPROPERTY()
	bool bShowSpawnInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data", meta=(EditCondition="bShowSpawnInterval"))
	float InitialSpawnDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data", meta=(EditCondition="bShowSpawnInterval"))
	float SpawnInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	bool ShouldSpawnHunting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	bool spawnOnBegin;

	FTimerHandle SpawnTimer;
	FTimerHandle HuntTimer;
};
