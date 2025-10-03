// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameSubsystem.h"

#include "CheckpointSubsystem.h"
#include "MySaveGame.h"
#include "Engine/World.h"
#include "SaveGameInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Main/Core/Framework/PlayerController/InterfacePlayerController.h"
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
	/*
	 * if(!HasAuthority())
	 *	return;
	 */
	
	if (bIsLaoding) return;
	
	TArray<AActor*> SaveGameActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveGameInterface::StaticClass(), SaveGameActors);
	
	CurrentSaveGame->SavedActors.Empty();

	for (AActor* Actor : SaveGameActors)
	{
		
		FActorSaveData ActorData;

		if (APawn* Pawn = Cast<APawn>(Actor))
		{
			if (AInterfacePlayerController* PC = Cast<AInterfacePlayerController>(Pawn->GetController()))
			{
				//MyId = PC->SaveId;

				const bool IsServerCharacterOnServer = PC->HasAuthority() && PC->IsLocalController();
				const bool IsClientCharacterOnServer = PC->HasAuthority() && !PC->IsLocalController();
		
				const bool IsClientCharacterOnClient = !PC->HasAuthority() && PC->IsLocalController();
				const bool IsServerCharacterOnClient = !PC->HasAuthority() && !PC->IsLocalController();

				ActorData.StableId = (IsServerCharacterOnServer || IsServerCharacterOnClient ? "Server" : "Client");

				
				
				ActorData.ActorName = Actor->GetFName();
			}
			else
			{
				ActorData.StableId = ("Client");
			}

		}
		else
		{
			ActorData.StableId = Actor->GetFName().ToString();
			ActorData.ActorName = Actor->GetFName();
		}
		//UCheckpointSubsystem* Subsystem = (GetWorld()->GetSubsystem<UCheckpointSubsystem>());
		//ActorData.CurrentCheckpoint = Subsystem->CurrentCheckpoint;

		FMemoryWriter MyMemoryWriter(ActorData.ByteData);
	
		FObjectAndNameAsStringProxyArchive Ar(MyMemoryWriter, true);

		
		ISaveGameInterface::Execute_OnSaveGame(Actor);
		
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);
		
	
		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
}

void USaveGameSubsystem::LoadGame()
{
	bIsLaoding = true;
	
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0)) return;

	CurrentSaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	TArray<AActor*> SaveGameActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveGameInterface::StaticClass(), SaveGameActors);

	//if (SaveGameActors.Num() >= 6)
		//SaveGameActors.Swap(4, 5);

	for (AActor* Actor : SaveGameActors)
	{
		FString MyId;
		
		if (APawn* Pawn = Cast<APawn>(Actor))
		{
			if (AInterfacePlayerController* PC = Cast<AInterfacePlayerController>(Pawn->GetController()))
			{
				//MyId = PC->SaveId;

				const bool IsServerCharacterOnServer = PC->HasAuthority() && PC->IsLocalController();
				const bool IsClientCharacterOnServer = PC->HasAuthority() && !PC->IsLocalController();
		
				const bool IsClientCharacterOnClient = !PC->HasAuthority() && PC->IsLocalController();
				const bool IsServerCharacterOnClient = !PC->HasAuthority() && !PC->IsLocalController();

				MyId = (IsServerCharacterOnServer || IsServerCharacterOnClient ? "Server" : "Client");
			}
			else
			{
				MyId = Actor->GetFName().ToString();
			}
		}
		else
		{
			MyId = Actor->GetFName().ToString();
		}
	
		
		FActorSaveData* ActorData = CurrentSaveGame->SavedActors.FindByPredicate(
			[&](const FActorSaveData& Data) { return Data.StableId == MyId; });

		if (!ActorData) continue;

		FMemoryReader Reader(ActorData->ByteData);
		FObjectAndNameAsStringProxyArchive Ar(Reader, true);
		Ar.ArIsSaveGame = true;

		Actor->Serialize(Ar);
		
		ISaveGameInterface::Execute_OnLoadGame(Actor);
		FVector Location = Actor->GetActorLocation();
	}

	bIsLaoding = false;
}

void USaveGameSubsystem::ResetData()
{
	
	TArray<AActor*> SaveGameActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveGameInterface::StaticClass(), SaveGameActors);

	for (AActor* Actor : SaveGameActors)
	{
		ISaveGameInterface::Execute_OnResetData(Actor);
	}

	CurrentSaveGame->SavedActors.Empty();
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
}

