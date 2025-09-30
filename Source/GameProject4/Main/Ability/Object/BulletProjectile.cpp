// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Ability/Object/BulletProjectile.h"
#include "Main/Ability/Object/AbilityData.h"
#include "Main/Ability/Object/AbilityEffectBase.h"
#include "Components/SphereComponent.h"

ABulletProjectile::ABulletProjectile()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComponent->SetupAttachment(SphereComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StaticMeshComponent->SetIsReplicated(true);
}

void ABulletProjectile::SetupProjectile_Implementation(AActor* Initiator, UModifiedPlayerStats* InModifierCollectio, FProjectileData ProjectileData)
{
	Speed = UDataFetcher::GetFloatDataByName("Speed", ProjectileData.Data.FloatData);
}

void ABulletProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Delta = Direction * (Speed * DeltaTime);
	SetActorLocation(GetActorLocation() + Delta);
}
