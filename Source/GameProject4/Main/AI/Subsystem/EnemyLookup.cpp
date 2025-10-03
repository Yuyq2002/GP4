// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyLookup.h"

#include "Net/UnrealNetwork.h"
#include "Debug.h"
void UEnemyLookup::AddAIToTile_Implementation(FVector Position, ASimpleDefaultAI* ai)
{
	FIntVector Tile = WorldToTile(Position);
	if (TileMap.Contains(Tile))
	{
		if (ai != nullptr)
			TileMap[Tile].AddUnique(ai);
	}
	else
	{
		if (ai!=nullptr)
			TileMap.Add(Tile, {ai});
	}
}

void UEnemyLookup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEnemyLookup, TileMap);
}


void UEnemyLookup::RemoveAIFromTile_Implementation(FVector Position, ASimpleDefaultAI* ai)
{
	if (TileMap.Contains(WorldToTile(Position)))
	{
		if (TileMap[WorldToTile(Position)].Remove(ai) < 0)
			Debug::Log("Not removed: " + ai->GetName());
	}
}

void UEnemyLookup::RemoveAllReference(ASimpleDefaultAI* ai)
{
	TArray<FIntVector> Keys;
	TileMap.GenerateKeyArray(Keys);

	for (auto& Key : Keys)
	{
		TileMap[Key].Remove(ai);
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

TArray<ASimpleDefaultAI*> UEnemyLookup::FindContentOfTile(FIntVector Tile)
{
	return TileMap.FindRef(Tile);
}

TArray<ASimpleDefaultAI*> UEnemyLookup::FindAllContentInRadius(FVector center, int tilesDistanceAway)
{
	TArray<ASimpleDefaultAI*> AIInRadius;

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
					for (auto a : FindContentOfTile(tileToCheck))
					{
						if(IsValid(a))
							AIInRadius.AddUnique(a);
					}
				}
			}
		}
	}	
	
	return AIInRadius;
}
