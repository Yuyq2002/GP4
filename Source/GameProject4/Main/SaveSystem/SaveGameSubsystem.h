// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubsystem.generated.h"

class UMySaveGame;

UCLASS()
class GAMEPROJECT4_API USaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString SaveSlotName = TEXT("SaveGame01");

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UMySaveGame> CurrentSaveGame;

	UPROPERTY(Transient)
	bool bIsLaoding = false;

	
	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();

	UFUNCTION(BlueprintCallable)
	void ResetData();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

};
