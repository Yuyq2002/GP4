// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/MeshComponent.h"
#include "InteractionComponent.generated.h"

class USphereComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPROJECT4_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractionComponent();

	UFUNCTION(BlueprintCallable, Category="Interaction")
	void TryInteract();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnRegister() override;   // attach + register sphere safely

	UPROPERTY(EditAnywhere, Category="Interaction|Sphere")
	float SphereRadius = 220.f;

	UPROPERTY(EditAnywhere, Category="Interaction|Sphere")
	TEnumAsByte<ECollisionChannel> OverlapChannelA;

	UPROPERTY(EditAnywhere, Category="Interaction|Sphere")
	TEnumAsByte<ECollisionChannel> OverlapChannelB;

	// ===== Components =====
	UPROPERTY(EditAnywhere, Category="Interaction|Sphere")
	USphereComponent* Sphere = nullptr;

	// ===== State =====
	UPROPERTY()
	AActor* FocusedActor = nullptr;

	UPROPERTY(EditAnywhere, Category="Interaction|Sphere")
	UMaterialInterface* HighlightMaterial = nullptr;

	TMap<TWeakObjectPtr<UStaticMeshComponent>, UMaterialInterface*> SavedOriginalMaterials;

	// Keep a set of candidates currently inside the sphere
	TSet<TWeakObjectPtr<AActor>> Candidates;

	// ===== Helpers =====
	void RefreshFocusedActor();

	void SetHighlightMaterial(AActor* Actor, bool bEnabled);

	UFUNCTION()
	void OnSphereBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
					   const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void ServerExecuteInteract(AActor* InFocusedActor);
};
