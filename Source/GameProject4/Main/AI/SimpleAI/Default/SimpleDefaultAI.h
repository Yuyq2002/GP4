// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleAiController.h"
#include "GameFramework/Character.h"
#include "Main/Ability/Enums/AiState.h"
#include "Main/Ability/Enums/PatrollBehaviour.h"
#include "Main/Ability/Enums/TargetInRange.h"
#include "Main/AI/Components/AbilityManagerComponent.h"
#include "Main/AI/Components/PatrolPoint.h"
#include "Main/Core/ExperienceSystem/ExperienceGranter.h"
#include "Main/Player/CasterCharacter.h"

#include "SimpleDefaultAI.generated.h"

class UAISenseConfig_Sight;
class ASimpleDefaultAI;
class ASimpleAiController;

USTRUCT(BlueprintType)
struct FWorldState
{
	GENERATED_BODY()

	FWorldState()
		: ClosestPlayerDistance(MAX_FLT)
		  , ClosestPlayer(nullptr)
		  , CurrentBehaviour(EAIState::Idle)
		  ,	CurrentTile()
		  , PreviousTile()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<APawn>> PlayersInWorld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ClosestPlayerDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APawn> ClosestPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APawn> furthestPlayer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAIState CurrentBehaviour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<ASimpleDefaultAI>> NearbyAI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Tile")
	FIntVector CurrentTile;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Tile")
	FIntVector PreviousTile;
};

USTRUCT(BlueprintType)
struct FAttackDistance
{
	GENERATED_BODY()

	FAttackDistance()
	: MinAttackDistance(200.0f)
	, MaxAttackDistance(400.0f)
	, TargetInRange(ETargetInRange::Far)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, NotReplicated)
	float MinAttackDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NotReplicated)
	float MaxAttackDistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETargetInRange TargetInRange;
};



USTRUCT(BlueprintType)
struct FCurrentPatrolPoint
{
	GENERATED_BODY()

	FCurrentPatrolPoint()
		: PatrolPointID(-1)
		, PatrollingBehaviour(EPatrolBehaviour::RoundRobin)
		, CurrentPatrolPointIndex(0)
		, PatrolPoint(nullptr)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, NotReplicated)
	int PatrolPointID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, NotReplicated)
	EPatrolBehaviour PatrollingBehaviour;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentPatrolPointIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APatrolPoint> PatrolPoint;
	
	void AssignPatrolPoint(UWorld* world);
	
	void IncrementPatrolPoint();
	
	FVector GetPatrolPointInWorld() const;

	private:
		UPROPERTY()
		int Direction = 1;
};

UCLASS(BlueprintType)
class GAMEPROJECT4_API ASimpleDefaultAI : public ACharacter,  public IGenericTeamAgentInterface
{
public:
	virtual void Tick(float DeltaSeconds) override;

private:
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASimpleDefaultAI();
	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="World")
	UWorld* GetCurrentWorld() const;

	// In AMyAIController.h
	// In AMyAIController.h
	UFUNCTION(BlueprintImplementableEvent, Category="AI")
	void OnPerceptionUpdated_BP(const TArray<AActor*>& UpdatedActors);

	// Internal C++ handler
	UFUNCTION()
	void HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UFUNCTION(BlueprintImplementableEvent, Category="AI")
	void HandleTargetPerceptionUpdated_BP(AActor* Actor, FAIStimulus Stimulus);
	
	// In your AI class header
	UFUNCTION()
	void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION(BlueprintCallable, Category = "AI|Behaviour")
	void SetHunting(bool hunting);

	UFUNCTION(BlueprintCallable, Category = "AI|Behaviour")
	void StartAllActions();
	
	UFUNCTION(BlueprintCallable, Category = "AI|Behaviour")
	void StopAllActions();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddAllPlayers();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	TArray<APawn*> GetAllPlayersInWorld();
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Structs")
	FWorldState worldState;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Structs")
	FAttackDistance AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UAbilityManagerComponent* AbilityManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UExperienceGranter* ExperienceGranter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Controller")
	ASimpleAiController* CurrentController;

	UFUNCTION(BlueprintCallable, Category = "AI|Tile")
	void OnMove();

	UFUNCTION(Server, Unreliable, Category = "AI|Tile")
	void Server_OnMove();

private:
	UPROPERTY()
	FGenericTeamId TeamId;
};
