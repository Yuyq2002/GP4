// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTestCharacter.h"

#include "Components/CapsuleComponent.h"


// Sets default values
AMyTestCharacter::AMyTestCharacter()
{
	
	maxHealth = 10;
	currentHealth = maxHealth;
	startPos = FVector(0.0f, 0.0f, 312.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyTestCharacter::OnBeginOverlap);
	
}

// Called when the game starts or when spawned
void AMyTestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
}


void AMyTestCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (!OtherActor || OtherActor == this) return;

	if (OtherActor->ActorHasTag("Damage"))
	{
		DOTakeDamage();
	}
	if (OtherActor->ActorHasTag("Save"))
	{
		currentPos = GetActorLocation();
	}
	
}

// Called to bind functionality to input
void AMyTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyTestCharacter::OnLoadGame_Implementation()
{
	//Load Saved stuff
	currentHealth = FMath::Clamp(currentHealth, 0, maxHealth);
	TeleportTo(currentPos, GetActorRotation(), false, true);
}

void AMyTestCharacter::OnResetData_Implementation()
{
	ResetSave();
}

void AMyTestCharacter::ResetSave()
{
	//Reset shiii
	currentHealth = maxHealth;
	TeleportTo(startPos, GetActorRotation(), false, true);
}

void AMyTestCharacter::DOTakeDamage()
{
	currentHealth -= 1;
}
