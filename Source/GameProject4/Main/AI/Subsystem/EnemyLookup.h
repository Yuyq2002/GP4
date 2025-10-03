// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/AI/SimpleAI/Default/SimpleDefaultAI.h"
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
	TMap<FIntVector, TArray<ASimpleDefaultAI*>> TileMap;

	UPROPERTY(BlueprintReadOnly)
	int TileSize = 1000;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Tile")
	void AddAIToTile(FVector Position, ASimpleDefaultAI* ai);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Tile")
	void RemoveAIFromTile(FVector Position, ASimpleDefaultAI* ai);

	UFUNCTION(BlueprintCallable)
	void RemoveAllReference(ASimpleDefaultAI* ai);

	UFUNCTION(BlueprintCallable, Category = "Tile")
	FIntVector WorldToTile(FVector WorldLocation);
	
	UFUNCTION(BlueprintCallable, Category = "Tile")
	FVector TileToWorld(FIntVector TileLocation);
	
	UFUNCTION(BlueprintCallable, Category = "Tile")
	TArray<ASimpleDefaultAI*> FindContentOfTile(FIntVector Tile);

	UFUNCTION(BlueprintCallable, Category = "Tile")
	TArray<ASimpleDefaultAI*> FindAllContentInRadius(FVector center, int tilesDistanceAway);
};
