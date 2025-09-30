// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Main/SaveSystem/SaveGameInterface.h"
#include "MyTestCharacter.generated.h"

UCLASS()
class GAMEPROJECT4_API AMyTestCharacter : public ACharacter, public ISaveGameInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyTestCharacter();

	// Interface implementations
	virtual void OnLoadGame_Implementation() override;
	virtual void OnResetData_Implementation() override;

	int maxHealth;

	FVector startPos;
	
	UPROPERTY(EditAnywhere, SaveGame, Category="SaveGames")
	int currentHealth;

	UPROPERTY(VisibleAnywhere, SaveGame, Category="SaveGames")
	FVector currentPos;

	UFUNCTION()
	void ResetSave();

	UFUNCTION()
	void DOTakeDamage();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
