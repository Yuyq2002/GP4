// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameSubsystem.h"
#include "MySaveGame.h"
#include "Engine/World.h"
#include "SaveGameInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void USaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SaveSlotName = TEXT("SaveGame01");
	
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		CurrentSaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	else
	{
		CurrentSaveGame = CastChecked<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	}
}

void USaveGameSubsystem::SaveGame()
{
	
	TArray<AActor*> SaveGameActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveGameInterface::StaticClass(), SaveGameActors);
	
	CurrentSaveGame->SavedActors.Empty();

	for (AActor* Actor : SaveGameActors)
	{
		
		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetFName();
		
		ISaveGameInterface::Execute_OnSaveGame(Actor);

		FMemoryWriter MyMemoryWriter(ActorData.ByteData);
	
		FObjectAndNameAsStringProxyArchive Ar(MyMemoryWriter, true);

		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);
	
		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
}

void USaveGameSubsystem::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		CurrentSaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

		TArray<AActor*> SaveGameActors;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveGameInterface::StaticClass(), SaveGameActors);

		for (AActor* Actor : SaveGameActors)
		{
			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetFName())
				{
					FMemoryReader MyMemoryReader(ActorData.ByteData);
	
					FObjectAndNameAsStringProxyArchive Ar(MyMemoryReader, true);
					Ar.ArIsSaveGame = true;
					
					Actor->Serialize(Ar);
	
					ISaveGameInterface::Execute_OnLoadGame(Actor);
	
					break;
				}
			}
		}
	}
	
}

void USaveGameSubsystem::ResetData()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
	
	TArray<AActor*> SaveGameActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveGameInterface::StaticClass(), SaveGameActors);

	for (AActor* Actor : SaveGameActors)
	{
		ISaveGameInterface::Execute_OnResetData(Actor);
	}

	CurrentSaveGame->SavedActors.Empty();
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
}

