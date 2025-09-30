// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "MultiplayerSessionSubsystem.h"

void UMenu::MenuSetup()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString("Menu setup"));
	}

	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* world = GetWorld();
	if (world)
	{
		APlayerController* playerController = world->GetFirstPlayerController();
		if (playerController)
		{
			FInputModeUIOnly inputModeData;
			inputModeData.SetWidgetToFocus(TakeWidget());
			inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			playerController->SetInputMode(inputModeData);
			playerController->SetShowMouseCursor(true);
		}
	}
}

bool UMenu::Initialize()
{
	if (!Super::Initialize()) return false;

	UGameInstance* gameInstance = GetGameInstance();
	if (gameInstance)
	{
		multiplayerSessionSubsystem = gameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
	}

	if (multiplayerSessionSubsystem)
	{
		multiplayerSessionSubsystem->MultiplayerSubsystemOnInviteAccepted.AddDynamic(this, &ThisClass::OnInviteAccepted);
	}

	return true;
}

void UMenu::HostSession()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Yellow, FString(TEXT("Host")));
	}

	if (multiplayerSessionSubsystem)
	{
		multiplayerSessionSubsystem->CreateSession(3);
	}
}

void UMenu::JoinSession()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Yellow, FString(TEXT("Join")));
	}

	if (multiplayerSessionSubsystem)
	{
		multiplayerSessionSubsystem->FindSession(10000);
	}
}

void UMenu::OpenFriendInviteUI()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Yellow, FString(TEXT("Fetch friend")));
	}

	if(multiplayerSessionSubsystem)
	{
		multiplayerSessionSubsystem->OpenFriendMenu();
	}
}

void UMenu::TravelToLevel(FString path, bool seamlessTravel)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Yellow, FString(TEXT("Travelling to level")));
	}

	if (multiplayerSessionSubsystem)
	{
		multiplayerSessionSubsystem->ServerTravelToLevel(path, seamlessTravel);
	}
}

void UMenu::DestroySession()
{
	if (multiplayerSessionSubsystem)
	{
		multiplayerSessionSubsystem->DestroySession();
	}
}

void UMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* world = GetWorld();
	if (world)
	{
		APlayerController* playerController = world->GetFirstPlayerController();
		if (playerController)
		{
			FInputModeGameOnly inputModeData;
			playerController->SetInputMode(inputModeData);
			playerController->SetShowMouseCursor(false);
		}
	}
}
