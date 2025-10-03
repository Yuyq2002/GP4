// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameTrigger.h"

#include "CheckpointSubsystem.h"
#include "MySaveGame.h"
#include "SaveGameSubsystem.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Main/Core/Framework/PlayerController/InterfacePlayerController.h"

// Sets default values
ASaveGameTrigger::ASaveGameTrigger()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
#if WITH_EDITOR
	Root->bVisualizeComponent = true;
#endif
	RootComponent = Root;


	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetHiddenInGame(false);
	Sphere->SetupAttachment(RootComponent);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ASaveGameTrigger::OnBeginOverlap);
}

void ASaveGameTrigger::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<UCheckpointSubsystem>()->RegisterCheckpoint(this);
}


void ASaveGameTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	APawn* OverlappingPawn = Cast<APawn>(OtherActor);
	if (!OverlappingPawn) return;

	AInterfacePlayerController* PC = Cast<AInterfacePlayerController>(OverlappingPawn->GetController());
	if (!PC) return;

	USaveGameSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	UCheckpointSubsystem* Subsystem = (GetWorld()->GetSubsystem<UCheckpointSubsystem>());

	Subsystem->CheckpointTriggered(thisCheckpointNumber);
	SaveSubsystem->SaveGame();
}







