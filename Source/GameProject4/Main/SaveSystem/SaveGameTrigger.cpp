// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameTrigger.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "SaveGameSubSystem.h"
#include "GameFramework/SaveGame.h"

// Sets default values
ASaveGameTrigger::ASaveGameTrigger()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
#if WITH_EDITOR
	Root->bVisualizeComponent = true;
#endif
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Text->SetupAttachment(RootComponent);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetHiddenInGame(false);
	Sphere->SetupAttachment(RootComponent);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ASaveGameTrigger::OnBeginOverlap);
}

void ASaveGameTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		if (bLoadGame)
		{
			LoadGame();
			UE_LOG(LogTemp, Warning, TEXT("Game Save Loaded !"));
			return;
		}
		
		if (bSaveGame)
		{
			SaveGame();
			UE_LOG(LogTemp, Warning, TEXT("Game Saved !"));
			return;
		}

		if (bResetData)
		{
			UE_LOG(LogTemp, Warning, TEXT("Game Save Reset !"));
			ResetData();
		}
	}
}

void ASaveGameTrigger::LoadGame()
{
	USaveGameSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	SaveSubsystem->LoadGame();
}

void ASaveGameTrigger::ResetData()
{
	USaveGameSubsystem* SaveSubsystem  = GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	SaveSubsystem->ResetData();
}

void ASaveGameTrigger::SaveGame()
{
	USaveGameSubsystem* SaveSubsystem  = GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	SaveSubsystem->SaveGame();
}


