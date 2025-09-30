// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Ability/Object/AbilityData.h"

int UDataFetcher::GetIntDataByName(FString Name, const TArray<FIntData>& IntArray)
{
	{
		for (auto& Data : IntArray)
		{
			if (Name.Equals(Data.Name))
				return Data.Value;
		}

		return 0;
	}
}

float UDataFetcher::GetFloatDataByName(FString Name, const TArray<FFloatData>& FloatArray)
{
	{
		for (auto& Data : FloatArray)
		{
			if (Name.Equals(Data.Name))
				return Data.Value;
		}

		return 0;
	}
}

FVector UDataFetcher::GetVectorDataByName(FString Name, const TArray<FVectorData>& VectorArray)
{
	{
		for (auto& Data : VectorArray)
		{
			if (Name.Equals(Data.Name))
				return Data.Value;
		}

		return FVector::ZeroVector;
	}
}
