// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "OnlineSessionSettings.h"

#include "MultiplayerSessionSubsystem.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerSubsystemOnInviteAccepted, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMultiplayerSubsystemOnCreateSession, FName, SessionName, bool, bWasSuccessful);

UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UMultiplayerSessionSubsystem();

	UFUNCTION(BlueprintCallable)
	void OpenFriendMenu();

	UFUNCTION(BlueprintCallable)
	void GetFriendList();

	UFUNCTION(BlueprintCallable)
	void CreateSession(int32 numOfConnections);

	UFUNCTION(BlueprintCallable)
	void FindSession(int32 maxSearchResult);

	UFUNCTION(BlueprintCallable)
	void DestroySession();

	UFUNCTION(BlueprintCallable)
	void StartSession();

	UFUNCTION(BlueprintCallable)
	void ServerTravelToLevel(FString path, bool seamlessTravel = false);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void OnReadComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnAcceptInvite(const bool bWasSuccessful, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult);

private:
	void JoinSession(const FOnlineSessionSearchResult& sessionResult);

private:
	bool RecreateSession = false;
	bool RejoinSession = false;

	FOnlineSessionSearchResult sessionToJoin;

	IOnlineSessionPtr sessionInterface;
	IOnlineFriendsPtr friendInterface;
	
	TSharedPtr<FOnlineSessionSettings> lastSessionSetting;
	TSharedPtr<FOnlineSessionSearch> sessionSearch;

	FOnCreateSessionCompleteDelegate createSessionCompleteDelegate;
	FDelegateHandle createSessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate findSessionCompleteDelegate;
	FDelegateHandle findSessionCompleteDelegateHandle;

	FOnJoinSessionCompleteDelegate joinSessionCompleteDelegate;
	FDelegateHandle joinSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate destroySessionCompleteDelegate;
	FDelegateHandle destroySessionCompleteDelegateHandle;

	FOnStartSessionCompleteDelegate startSessionCompleteDelegate;
	FDelegateHandle startSessionCompleteDelegateHandle;

	FOnSessionUserInviteAcceptedDelegate acceptInviteCompleteDelegate;
	FDelegateHandle acceptInviteCompleteDelegateHandle;

	FOnReadFriendsListComplete readFriendListCompleteDelegate;
	
public:
	FMultiplayerSubsystemOnInviteAccepted MultiplayerSubsystemOnInviteAccepted;

	UPROPERTY(BlueprintAssignable)
	FMultiplayerSubsystemOnCreateSession MultiplayerSubsystemOnCreateSession;
};
