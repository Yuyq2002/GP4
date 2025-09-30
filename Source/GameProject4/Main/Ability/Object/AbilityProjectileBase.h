// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Queue.h"
#include "Main/Ability/Object/AbilityEffectBase.h"
#include "Main/Core/SkillTree/ModifiedPlayerStats.h"

#include "AbilityProjectileBase.generated.h"

struct FProjectileData;
class AAbilityEffectBase;
class USphereComponent;

UCLASS(Abstract, Blueprintable, BlueprintType)
class GAMEPROJECT4_API AAbilityProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityProjectileBase();

	void SetEffects(const TArray<FEffectData>& RemainingEffects);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetupProjectile(AActor* Initiator, UModifiedPlayerStats* InModifierCollection, FProjectileData ProjectileData);
	virtual void SetupProjectile_Implementation(AActor* Initiator, UModifiedPlayerStats* InModifierCollection, FProjectileData ProjectileData);

	UFUNCTION(BlueprintNativeEvent)
	void OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OnCollision_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USphereComponent* SphereComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UModifiedPlayerStats* ModifierCollection;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FVector Direction;
protected:

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FEffectData> Effects;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	AAbilityEffectBase* ActiveEffect;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EElement ElementType;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float LifeTime;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float Damage;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float ElementStrength;

	bool bIsFinished = false;
};
