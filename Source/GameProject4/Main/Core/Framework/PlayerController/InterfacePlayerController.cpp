// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Core/Framework/PlayerController/InterfacePlayerController.h"

#include "GameProject4.h"
#include "Main/Core/Data/Interface/ControllableInterface.h"
#include "Main/SaveSystem/SaveGameSubSystem.h"
#include "Net/UnrealNetwork.h" 

void AInterfacePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (SaveId.IsEmpty())
	{
		const bool IsServerCharacterOnServer = HasAuthority() && IsLocalController();
		const bool IsClientCharacterOnServer = HasAuthority() && !IsLocalController();
		
		const bool IsClientCharacterOnClient = !HasAuthority() && IsLocalController();
		const bool IsServerCharacterOnClient = !HasAuthority() && !IsLocalController();
		
		SaveId = (IsServerCharacterOnServer || IsServerCharacterOnClient ? "Server" : "Client");
	}

	
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerMappingContext, 0);
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, "Added mapping");
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Primary
		EnhancedInputComponent->BindAction(PrimaryInputAction, ETriggerEvent::Triggered, this, &AInterfacePlayerController::OnPrimary);

		// Secondary
		EnhancedInputComponent->BindAction(SecondaryInputAction, ETriggerEvent::Triggered, this, &AInterfacePlayerController::OnSecondary);

		// Dash
		EnhancedInputComponent->BindAction(DashInputAction, ETriggerEvent::Triggered, this, &AInterfacePlayerController::OnDash);

		// Rotate Element
		EnhancedInputComponent->BindAction(RotationInputAction, ETriggerEvent::Triggered, this, &AInterfacePlayerController::OnRotation);
		EnhancedInputComponent->BindAction(RotationInputAction, ETriggerEvent::Started, this, &AInterfacePlayerController::OnRotationStart);
		EnhancedInputComponent->BindAction(RotationInputAction, ETriggerEvent::Completed, this, &AInterfacePlayerController::OnRotationEnd);

		// Movement
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AInterfacePlayerController::OnMove);

		EnhancedInputComponent->BindAction(TabInputAction, ETriggerEvent::Triggered, this, &AInterfacePlayerController::OnTab);

		EnhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Triggered, this, &AInterfacePlayerController::OnInteract);

		EnhancedInputComponent->BindAction(UsePotionInputAction, ETriggerEvent::Triggered, this, &AInterfacePlayerController::OnUsePotion);

		EnhancedInputComponent->BindAction(UseAuraInputAction, ETriggerEvent::Triggered, this, &AInterfacePlayerController::OnUseAura);

		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, "Binded actions");
	}
}

void AInterfacePlayerController::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);

	bImplementedInterface = inPawn->GetClass()->ImplementsInterface(UControllableInterface::StaticClass());

	GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, bImplementedInterface ? "Have interface" : "No interface");
}

void AInterfacePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInterfacePlayerController, bImplementedInterface);
}

void AInterfacePlayerController::OnPrimary()
{
	if (!bImplementedInterface)
		return;

	IControllableInterface::Execute_PrimaryAction(GetPawn());
}

void AInterfacePlayerController::OnSecondary()
{
	if (!bImplementedInterface)
		return;

	IControllableInterface::Execute_SecondaryAction(GetPawn());
}

void AInterfacePlayerController::OnDash()
{
	if (!bImplementedInterface)
		return;

	IControllableInterface::Execute_DashAction(GetPawn());
}

void AInterfacePlayerController::OnRotation()
{
	if (!bImplementedInterface)
		return;

	IControllableInterface::Execute_RotationAction(GetPawn());
}

void AInterfacePlayerController::OnRotationStart()
{
	if (!bImplementedInterface)
		return;

	IControllableInterface::Execute_RotationStartAction(GetPawn());
}

void AInterfacePlayerController::OnRotationEnd()
{
	if (!bImplementedInterface)
		return;

	IControllableInterface::Execute_RotationEndAction(GetPawn());
}

void AInterfacePlayerController::OnTab()
{
	if (!bImplementedInterface)
		return;

	IControllableInterface::Execute_TabAction(GetPawn());
}

void AInterfacePlayerController::OnInteract()
{
	if (!bImplementedInterface)
		return;

	IControllableInterface::Execute_InteractAction(GetPawn());
}

void AInterfacePlayerController::OnMove(const FInputActionValue& Value)
{
	if (!bImplementedInterface)
		return;

	IControllableInterface::Execute_MoveAction(GetPawn(), Value.Get<FVector>());
}

void AInterfacePlayerController::OnUsePotion()
{
	if (!bImplementedInterface)
		return;

	IControllableInterface::Execute_UsePotionAction(GetPawn());
}

void AInterfacePlayerController::OnUseAura()
{
	if (!bImplementedInterface)
		return;

	IControllableInterface::Execute_UseAuraAction(GetPawn());
}

void AInterfacePlayerController::ServerRequestSaveLoad_Implementation(bool bSave, bool bLoad, bool bReset)
{
	// Server executes save/load
	//ExecuteSaveLoad(bSave, bLoad, bReset);
}

void AInterfacePlayerController::ExecuteSaveLoad(int savepoint)
{

	checkPoint = savepoint;

	USaveGameSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	//SaveSubsystem->SaveGame(savepoint);
	
	/*
	if (bSave)
	{
		USaveGameSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
		SaveSubsystem->SaveGame();
		UE_LOG(LogTemp, Warning, TEXT("Server executed SaveGame"));
	}

	if (bLoad)
	{
		USaveGameSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
		SaveSubsystem->LoadGame();   
		UE_LOG(LogTemp, Warning, TEXT("Server executed LoadGame + multicast"));
	}

	if (bReset)
	{
		
		USaveGameSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
		SaveSubsystem->ResetData();
		UE_LOG(LogTemp, Warning, TEXT("Server executed ResetData"));
	}
	*/
}
