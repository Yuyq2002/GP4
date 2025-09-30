// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/OnlineExternalUIInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"
#include "GameFramework/PlayerController.h"

UMultiplayerSessionSubsystem::UMultiplayerSessionSubsystem() :
	createSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	findSessionCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionComplete)),
	joinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	destroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	startSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete)),
	readFriendListCompleteDelegate(FOnReadFriendsListComplete::CreateUObject(this, &ThisClass::OnReadComplete)),
	acceptInviteCompleteDelegate(FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &ThisClass::OnAcceptInvite))
{
	IOnlineSubsystem* subsystem = IOnlineSubsystem::Get();
	if (subsystem)
	{
		sessionInterface = subsystem->GetSessionInterface();
		friendInterface = subsystem->GetFriendsInterface();
	}
}

void UMultiplayerSessionSubsystem::CreateSession(int32 numOfConnections)
{
	if (!sessionInterface.IsValid())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString("Session interface not valid"));
		}
		return;
	}

	auto existingSession = sessionInterface->GetNamedSession(NAME_GameSession);

	if (existingSession != nullptr)
	{
		RecreateSession = true;
		sessionInterface->DestroySession(NAME_GameSession);
		return;
	}

	lastSessionSetting = MakeShareable(new FOnlineSessionSettings());
	lastSessionSetting->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	lastSessionSetting->NumPublicConnections = numOfConnections;
	lastSessionSetting->bAllowJoinInProgress = true;
	lastSessionSetting->bAllowJoinViaPresence = true;
	lastSessionSetting->bShouldAdvertise = true;
	lastSessionSetting->bUsesPresence = true;
	lastSessionSetting->bUseLobbiesIfAvailable = true;

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *lastSessionSetting))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString("Fail to create session!"));
		}
	}
}

void UMultiplayerSessionSubsystem::FindSession(int32 maxSearchResult)
{
	if (!sessionInterface.IsValid()) return;

	sessionSearch = MakeShareable(new FOnlineSessionSearch());
	sessionSearch->MaxSearchResults = maxSearchResult;
	sessionSearch->bIsLanQuery = false;
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString("Looking for session"));
	}

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	sessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), sessionSearch.ToSharedRef());
}

void UMultiplayerSessionSubsystem::JoinSession(const FOnlineSessionSearchResult& sessionResult)
{
	FString id = sessionResult.GetSessionIdStr();
	FString user = sessionResult.Session.OwningUserName;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("id: %s, User: %s"), *id, *user));
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString("Joining for session"));
	}

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	sessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, sessionResult);
}

void UMultiplayerSessionSubsystem::DestroySession()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString("Start destroying"));
	}

	auto existingSession = sessionInterface->GetNamedSession(NAME_GameSession);

	if (existingSession != nullptr)
	{
		sessionInterface->DestroySession(NAME_GameSession);
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString("No session to destroy"));
		}
	}
}

void UMultiplayerSessionSubsystem::StartSession()
{
}

void UMultiplayerSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	createSessionCompleteDelegateHandle = sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(createSessionCompleteDelegate);
	findSessionCompleteDelegateHandle = sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(findSessionCompleteDelegate);
	joinSessionCompleteDelegateHandle = sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(joinSessionCompleteDelegate);
	acceptInviteCompleteDelegateHandle = sessionInterface->AddOnSessionUserInviteAcceptedDelegate_Handle(acceptInviteCompleteDelegate);
	destroySessionCompleteDelegateHandle = sessionInterface->AddOnDestroySessionCompleteDelegate_Handle(destroySessionCompleteDelegate);
}

void UMultiplayerSessionSubsystem::Deinitialize()
{
	Super::Deinitialize();

	sessionInterface->ClearOnSessionUserInviteAcceptedDelegate_Handle(acceptInviteCompleteDelegateHandle);
	sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(createSessionCompleteDelegateHandle);
	sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(findSessionCompleteDelegateHandle);
	sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(joinSessionCompleteDelegateHandle);
	sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(destroySessionCompleteDelegateHandle);
}

void UMultiplayerSessionSubsystem::OnReadComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
{
	if (!bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString("Read failed!"));
		}

		return; 
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Green, FString("Read Successful!"));

		TArray<TSharedRef<FOnlineFriend>> friendList;
		friendInterface->GetFriendsList(LocalUserNum, ListName, friendList);
	}
}

void UMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Green, FString::Printf(TEXT("Created session: %s"), *SessionName.ToString()));
		}

		MultiplayerSubsystemOnCreateSession.Broadcast(SessionName, bWasSuccessful);
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString::Printf(TEXT("Failed to create session!")));
		}
	}
}

void UMultiplayerSessionSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	if (!sessionInterface.IsValid()) return;

	if (!bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString("Failed to find session"));
		}

		return;
	}

	for (auto result : sessionSearch->SearchResults)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Green, FString("Found session"));
		}

		JoinSession(result);
	}
}

void UMultiplayerSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result)
{
	if (!sessionInterface.IsValid()) return;

	FString address;
	if (sessionInterface->GetResolvedConnectString(NAME_GameSession, address))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("Connect string: %s"), *address));
		}

		APlayerController* playerController = GetGameInstance()->GetFirstLocalPlayerController();
		if (playerController)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("Travelling to: %s"), *address));
			}

			playerController->ClientTravel(address, TRAVEL_Absolute);
		}
	}
}

void UMultiplayerSessionSubsystem::OnAcceptInvite(const bool bWasSuccessful, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult)
{
	if (!sessionInterface.IsValid()) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString("Accepted invite"));
	}

	auto existingSession = sessionInterface->GetNamedSession(NAME_GameSession);

	if (existingSession != nullptr)
	{
		RejoinSession = true;
		sessionToJoin = InviteResult;
		sessionInterface->DestroySession(NAME_GameSession);
		return;
	}

	MultiplayerSubsystemOnInviteAccepted.Broadcast(true);

	JoinSession(InviteResult);
}

void UMultiplayerSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!sessionInterface.IsValid()) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString("Session Destroyed"));

		if (RecreateSession)
		{
			RecreateSession = false;
			CreateSession(2);
			return;
		}

		if (RejoinSession)
		{
			RejoinSession = false;
			JoinSession(sessionToJoin);
			return;
		}

		AGameModeBase* gameMode = GetWorld()->GetAuthGameMode();
		if (gameMode)
		{
			gameMode->ReturnToMainMenuHost();
			GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString("Return to main menu HOST"));
		}
		else
		{
			APlayerController* playerController = GetGameInstance()->GetFirstLocalPlayerController();

			if (playerController)
			{
				playerController->ClientReturnToMainMenuWithTextReason(FText());
				GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString("Return to main menu CLIENT"));
			}
		}
	}
}

void UMultiplayerSessionSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UMultiplayerSessionSubsystem::OpenFriendMenu()
{
	if (!friendInterface.IsValid()) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString("Open friend UI"));
	}

	auto uiRef = IOnlineSubsystem::Get()->GetExternalUIInterface();

	if (uiRef.IsValid())
	{
		uiRef->ShowInviteUI(0);
	}
}

void UMultiplayerSessionSubsystem::GetFriendList()
{
	if (!friendInterface.IsValid()) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString("Get friend list"));
	}
}

void UMultiplayerSessionSubsystem::ServerTravelToLevel(FString path, bool seamlessTravel)
{
	UWorld* world = GetWorld();
	AGameModeBase* gameMode = world->GetAuthGameMode();
	
	if (world && gameMode)
	{
		gameMode->bUseSeamlessTravel = seamlessTravel;
		world->ServerTravel(FString::Printf(TEXT("%s?listen"), *path));
	}
}
