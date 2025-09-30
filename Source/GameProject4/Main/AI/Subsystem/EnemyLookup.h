// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/AI/DefaultEnemyAI.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyLookup.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT4_API UEnemyLookup : public UWorldSubsystem
{
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	GENERATED_BODY()

public:
	TMap<FIntVector, TArray<ADefaultEnemyAI*>> TileMap;

	UPROPERTY(BlueprintReadOnly)
	int TileSize = 100;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Tile")
	void AddAIToTile(FVector Position, ADefaultEnemyAI* ai);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Tile")
	void RemoveAIFromTile(FVector Position, ADefaultEnemyAI* ai);

	UFUNCTION(BlueprintCallable, Category = "Tile")
	FIntVector WorldToTile(FVector WorldLocation);
	
	UFUNCTION(BlueprintCallable, Category = "Tile")
	FVector TileToWorld(FIntVector TileLocation);
	
	UFUNCTION(BlueprintCallable, Category = "Tile")
	TArray<ADefaultEnemyAI*> FindContentOfTile(FIntVector Tile);

	UFUNCTION(BlueprintCallable, Category = "Tile")
	TArray<ADefaultEnemyAI*> FindAllContentInRadius(FVector center, int tilesDistanceAway);
	
};
