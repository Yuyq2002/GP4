// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "GameProject4/Main/Core/SkillTree/ModifiedPlayerStats.h"
#include "CasterCharacter.generated.h"


class UInputAction;
struct FInputActionValue;

UCLASS()
class GAMEPROJECT4_API ACasterCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ACasterCharacter();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModifiedPlayerStats")
	UModifiedPlayerStats* ModifiedPlayerStats;
protected:
	// Called when the game starts or when spawned

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input


	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	
	UPROPERTY(VisibleAnywhere)
	FGenericTeamId TeamId;
};
