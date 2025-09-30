// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultAIController.h"

ETeamAttitude::Type ADefaultAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(&Other))
	{
		if (OtherTeamAgent->GetGenericTeamId() == FGenericTeamId(2)) // enemy team
		{
			return ETeamAttitude::Hostile;
		}
		else if (OtherTeamAgent->GetGenericTeamId() == FGenericTeamId(1))
		{
			return ETeamAttitude::Friendly;
		}
	}

	return ETeamAttitude::Neutral;
}

// Sets default values
ADefaultAIController::ADefaultAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADefaultAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefaultAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

