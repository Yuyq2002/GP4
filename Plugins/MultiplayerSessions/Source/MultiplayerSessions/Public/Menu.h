// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Menu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup();

	UFUNCTION(BlueprintCallable)
	void MenuTearDown();

	UFUNCTION(BlueprintCallable)
	void HostSession();

	UFUNCTION(BlueprintCallable)
	void JoinSession();

	UFUNCTION(BlueprintCallable)
	void OpenFriendInviteUI();

	UFUNCTION(BlueprintCallable)
	void TravelToLevel(FString path, bool seamlessTravel = false);

	UFUNCTION(BlueprintCallable)
	void DestroySession();

protected:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnInviteAccepted(bool bWasSuccessful);

private:
	class UMultiplayerSessionSubsystem* multiplayerSessionSubsystem;

};
