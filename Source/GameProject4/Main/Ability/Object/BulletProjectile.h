// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/Ability/Object/AbilityProjectileBase.h"
#include "BulletProjectile.generated.h"


/**
 * 
 */
UCLASS()
class GAMEPROJECT4_API ABulletProjectile : public AAbilityProjectileBase
{
	GENERATED_BODY()
	
public:
	ABulletProjectile();

	virtual void SetupProjectile_Implementation(AActor* Initiator, UModifiedPlayerStats* InModifierCollectio, FProjectileData ProjectileData) override;

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMeshComponent;

private:
	float Speed;
};
