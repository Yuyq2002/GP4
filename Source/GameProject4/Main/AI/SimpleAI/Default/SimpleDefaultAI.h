// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleAiController.h"
#include "GameFramework/Character.h"
#include "Main/AI/Components/AbilityManagerComponent.h"
#include "Main/AI/Components/PatrolPoint.h"
#include "Main/Core/ExperienceSystem/ExperienceGranter.h"
#include "Main/Player/CasterCharacter.h"

#include "SimpleDefaultAI.generated.h"

class UAISenseConfig_Sight;
class ASimpleDefaultAI;
class ASimpleAiController;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Patrol			UMETA(DisplayName = "Patrol"),
	Hunting			UMETA(DisplayName = "Hunting")
};

UENUM(BlueprintType)
enum class ETargetInRange : uint8
{
	Far			UMETA(DisplayName = "Far"),
	Close		UMETA(DisplayName = "Close"),
	InRange		UMETA(DisplayName = "InRange")
};

USTRUCT(BlueprintType)
struct FWorldState
{
	GENERATED_BODY()

	FWorldState()
	: ClosestPlayerDistance(MAX_FLT)
	, ClosestPlayer(nullptr)
	, PlayerInLineOfSight(false)
	, CurrentBehaviour(EAIState::Idle)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<ACasterCharacter>> PlayersInWorld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ClosestPlayerDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ACasterCharacter> ClosestPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ACasterCharacter> furthestPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool PlayerInLineOfSight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAIState CurrentBehaviour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<ASimpleDefaultAI>> NearbyAI;
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

UENUM(BlueprintType)
enum class EPatrolBehaviour : uint8
{
	RoundRobin			UMETA(DisplayName = "RoundRobin"),
	BackAndForth		UMETA(DisplayName = "BackAndForth"),
	RandomIndex			UMETA(DisplayName = "RandomIndex")
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

UCLASS()
class GAMEPROJECT4_API ASimpleDefaultAI : public ACharacter,  public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASimpleDefaultAI();
	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;
	

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
private:
	UPROPERTY()
	FGenericTeamId TeamId;

	UPROPERTY()
	TArray<AActor*> targetsInLineOfSight;
};
