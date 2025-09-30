// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveGameInterface.generated.h"

UINTERFACE()
class USaveGameInterface : public UInterface
{
	GENERATED_BODY()
};

class GAMEPROJECT4_API ISaveGameInterface
{
	GENERATED_BODY()

public:
	// Load game data from save file
	UFUNCTION(BlueprintNativeEvent)
	void OnLoadGame();

	// Rest and save data to original state
	UFUNCTION(BlueprintNativeEvent)
	void OnResetData();

	// Save game to save file
	UFUNCTION(BlueprintNativeEvent)
	void OnSaveGame();

};
