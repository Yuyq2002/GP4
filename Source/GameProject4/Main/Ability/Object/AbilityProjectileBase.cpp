// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Ability/Object/AbilityProjectileBase.h"
#include "Main/Ability/Object/AbilityData.h"
#include "Components/SphereComponent.h"

// Sets default values
AAbilityProjectileBase::AAbilityProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You 
	// can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComponent;

	Tags.Add("Projectile");

	bReplicates = true;
	SetReplicateMovement(true);
}

void AAbilityProjectileBase::SetEffects(const TArray<FEffectData>& RemainingEffects)
{
	Effects = RemainingEffects;
	if (Effects.Num() > 0)
	{
		if (!Effects[0].Effect) return;

		ActiveEffect = GetWorld()->SpawnActor<AAbilityEffectBase>(Effects[0].Effect);
		ActiveEffect->SetData(Effects[0], ModifierCollection);
		ActiveEffect->SetInheritingData(Effects[0].Data);
		Effects.RemoveAt(0);
	}
}

void AAbilityProjectileBase::SetupProjectile_Implementation(AActor* Initiator, UModifiedPlayerStats* InModifierCollection, FProjectileData ProjectileData)
{
	LifeTime = UDataFetcher::GetFloatDataByName("LifeTime", ProjectileData.Data.FloatData);
	Damage = UDataFetcher::GetFloatDataByName("Damage", ProjectileData.Data.FloatData);
	ElementStrength = UDataFetcher::GetFloatDataByName("ElementStrength", ProjectileData.Data.FloatData);
	ElementType = ProjectileData.ElementType;
	Direction = Initiator->GetActorForwardVector();
	ModifierCollection = InModifierCollection;
}

void AAbilityProjectileBase::OnCollision_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("Projectile")) return;

	bIsFinished = true;
}

// Called when the game starts or when spawned
void AAbilityProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAbilityProjectileBase::OnCollision);
}

// Called every frame
void AAbilityProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LifeTime -= DeltaTime;
	if (LifeTime <= 0 || bIsFinished)
	{
		if (ActiveEffect)
		{
			ActiveEffect->SetEffects(Effects);
			ActiveEffect->ApplyEffect(this);
		}
		Destroy();
	}
}

