// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Components/AIAttackComponent.h"
#include "Components/PatrolPoint.h"
#include "GameFramework/Character.h"
#include "Main/Ability/Enums/StunnedType.h"
#include "DefaultEnemyAI.generated.h"

UENUM(BlueprintType)
enum class AggroRangeStatus : uint8
{
	TargetInRange			UMETA(DisplayName = "TargetInRange"),
	TargetNotInRange		UMETA(DisplayName = "TargetNotInRange"),
	TargetIsClose			UMETA(DisplayName = "TargetIsClose"),
	TargetIsNull			UMETA(DisplayName = "TargetIsNull")
};

UENUM(BlueprintType)
enum class AIAttackState : uint8
{
	ReadyToAttack		UMETA(DisplayName = "ReadyToAttack"),
	InAttack			UMETA(DisplayName = "InAttack"),
	EndOfAttack			UMETA(DisplayName = "EndOfAttack")
};

UENUM(BlueprintType)
enum class AiIdleBehaviour : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Patrol			UMETA(DisplayName = "Patrol")
};

UENUM(BlueprintType)
enum class Attacktype : uint8
{
	Far		UMETA(DisplayName = "Far"),
	Close		UMETA(DisplayName = "Close"),
	Normal		UMETA(DisplayName = "Normal")
};

UCLASS()
class GAMEPROJECT4_API ADefaultEnemyAI : public ACharacter, public IGenericTeamAgentInterface
{
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay() override;
public:
	// Sets default values for this character's properties
	ADefaultEnemyAI();

	UFUNCTION(BlueprintCallable, Category = "AI|Attack")
	AActor* GetAggroTarget();
	
	UFUNCTION(BlueprintCallable, Category = "AI|Attack")
	void SetAggroTarget(AActor* target);

	UFUNCTION(BlueprintCallable, Category = "AI|Attack")
	AActor* GetClosestPlayer();

	UFUNCTION(BlueprintCallable, Category = "AI|Attack")
	void RemoveAggroTarget();
	
	UFUNCTION(BlueprintCallable, Category = "AI|Attack")
	AggroRangeStatus AggroTargetIsInAttackRange();

	UFUNCTION(BlueprintCallable, Category = "AI|Attack")
	AggroRangeStatus GetAggroRange();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "AI|Attack")
	void SetAttackState(AIAttackState state);

	UFUNCTION(BlueprintCallable, Category = "AI|Attack")
	bool CanAttack() const;

	UFUNCTION(BlueprintCallable, Category = "AI|Tile")
	void OnMove();

	UFUNCTION(Server, Reliable, Category = "AI|Tile")
	void Server_OnMove();

	UFUNCTION(BlueprintCallable, Category = "AI|Attack")
	void SetStunnedTime(float time);

	UFUNCTION(BlueprintCallable, Category = "AI|Attack")
	bool DoAttackFromArrayRef(Attacktype arrayType, int chosenAttack);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Attack")
	float MinDistanceToTarget = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Attack")
	float MaxDistanceToTarget = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Attack")
	float HitByAttackStunnedDurationSeconds = 0.3f;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "AI|Attack")
	bool IsHunting = false;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "AI|Attack")
	AIAttackState AttackState = AIAttackState::ReadyToAttack;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "AI|Tile")
	FIntVector CurrentTile;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "AI|Tile")
	FIntVector PreviousTile;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "AI|Behaviour")
	AiIdleBehaviour AIIdleBehaviour = AiIdleBehaviour::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behaviour")
	APatrolPoint* PatrolPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Attack")
	float StunTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behaviour")
	StunnedType Stunned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Attack")
	TArray<UAIAttackComponent*> FarAttacks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Attack")
	TArray<UAIAttackComponent*> CloseAttacks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Attack")
	TArray<UAIAttackComponent*> NormalAttacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Attack")
	bool AbilityOnCooldown = false;
	
private:
	GENERATED_BODY()
	
	/**
	 * The current target of this enemy.\n
	 * Is used mostly for pathfinding.
	 */
	UPROPERTY(Replicated, VisibleAnywhere)
	TWeakObjectPtr<AActor> AggroTarget;
	
	UPROPERTY()
	FGenericTeamId TeamId;


};
