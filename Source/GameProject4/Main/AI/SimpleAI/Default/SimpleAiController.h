// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/StateTreeAIComponent.h"
#include "Perception/AIPerceptionTypes.h"

#include "SimpleAiController.generated.h"

class ASimpleDefaultAI;

UCLASS()
class GAMEPROJECT4_API ASimpleAiController : public AAIController
{
public:
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimpleAiController();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UStateTreeAIComponent* StateTreeAIComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASimpleDefaultAI* CurrentPawn;

	private:
		UFUNCTION()
		void AssignClosestPlayer() const;

		UFUNCTION(Server, Unreliable)
		void AddPlayers() const;
	
};
