// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyLookup.h"

#include "Net/UnrealNetwork.h"

void UEnemyLookup::AddAIToTile_Implementation(FVector Position, ADefaultEnemyAI* ai)
{
	FIntVector Tile = WorldToTile(Position);
	if (TileMap.Contains(Tile))
	{
		TileMap.FindRef(Tile).Add(ai);
	}
	else
	{
		TileMap.Add(Tile, {ai});
	}
}

void UEnemyLookup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEnemyLookup, TileMap);
}


void UEnemyLookup::RemoveAIFromTile_Implementation(FVector Position, ADefaultEnemyAI* ai)
{
	if (TileMap.Contains(WorldToTile(Position)))
	{
		TileMap.FindRef(WorldToTile(Position)).Remove(ai);
	}
}

FIntVector UEnemyLookup::WorldToTile(FVector WorldLocation)
{
	int tileX = FMath::Floor(WorldLocation.X / TileSize);
	int tileY = FMath::Floor(WorldLocation.Y / TileSize);
	int tileZ = FMath::Floor(WorldLocation.Z / TileSize);
	
	return FIntVector(tileX, tileY, tileZ);
}

FVector UEnemyLookup::TileToWorld(FIntVector TileLocation)
{
	float tileX = TileLocation.X * TileSize;
	float tileY = TileLocation.Y * TileSize;
	float tileZ = TileLocation.Z * TileSize;

	return FVector(tileX, tileY, tileZ);
}

TArray<ADefaultEnemyAI*> UEnemyLookup::FindContentOfTile(FIntVector Tile)
{
	return TileMap.FindRef(Tile);
}

TArray<ADefaultEnemyAI*> UEnemyLookup::FindAllContentInRadius(FVector center, int tilesDistanceAway)
{
	TArray<ADefaultEnemyAI*> AIInRadius;

	FIntVector CenterTile = WorldToTile(center);
	FIntVector tileToCheck = CenterTile;

	for (int x = -tilesDistanceAway; x <= tilesDistanceAway; x++)
	{
		for (int y = -tilesDistanceAway; y <= tilesDistanceAway; y++)
		{
			for (int z = -tilesDistanceAway; z <= tilesDistanceAway; z++)
			{
				tileToCheck = CenterTile + FIntVector(x, y, z);

				if (TileMap.Contains(tileToCheck))
				{
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(
							-1,
							5.f,
							FColor::Yellow,
							FString::Printf(TEXT("Tile: X=%d, Y=%d, Z=%d"), tileToCheck.X, tileToCheck.Y, tileToCheck.Z)
						);

						GEngine->AddOnScreenDebugMessage(
							-1,
							5.f,
							FColor::Yellow,
							FString::Printf(TEXT("Tile: numItems=%d"), FindContentOfTile(tileToCheck).Num())
						);
					}

					UE_LOG(LogTemp, Log, TEXT("Tile: X=%d, Y=%d, Z=%d"), tileToCheck.X, tileToCheck.Y, tileToCheck.Z);

					
					for (auto a : FindContentOfTile(tileToCheck))
					{
						AIInRadius.Add(a);
					}
				}
			}
		}
	}	
	
	return AIInRadius;
}
