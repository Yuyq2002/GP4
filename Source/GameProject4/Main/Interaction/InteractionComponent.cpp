// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "InteractionInterface.h"
#include "Components/SphereComponent.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Create the sphere as a subobject of this component
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	Sphere->InitSphereRadius(SphereRadius);
	//Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	//Sphere->SetCollisionResponseToChannel(OverlapChannelA, ECR_Overlap);
	//Sphere->SetCollisionResponseToChannel(OverlapChannelB, ECR_Overlap);
	Sphere->SetGenerateOverlapEvents(true);

	// Make it invisible in game/editor
	Sphere->SetHiddenInGame(true);
	Sphere->SetVisibility(true);
	Sphere->bHiddenInGame = true;

	// Don’t auto register until we can attach to owner’s root in OnRegister
	Sphere->CreationMethod = EComponentCreationMethod::Native;
}

void UInteractionComponent::OnRegister()
{
	Super::OnRegister();
	
	if (AActor* Owner = GetOwner())
	{
		if (USceneComponent* Root = Owner->GetRootComponent())
		{
			// Attach and register the sphere
			Sphere->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
			if (!Sphere->IsRegistered())
			{
				Sphere->RegisterComponent();
			}

			Sphere->SetSphereRadius(SphereRadius);

			Sphere->OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::OnSphereBegin);
			Sphere->OnComponentEndOverlap  .AddDynamic(this, &UInteractionComponent::OnSphereEnd);
		}
	}
}

// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	RefreshFocusedActor();
}



void UInteractionComponent::OnSphereBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner()) return;

	// Only consider actors that implement the interface (fast filter)
	if (OtherActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		Candidates.Add(OtherActor);
		RefreshFocusedActor();
	}
}

void UInteractionComponent::OnSphereEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor) return;
 
	Candidates.Remove(OtherActor);
	if (FocusedActor == OtherActor)
	{
		SetHighlightMaterial(FocusedActor, false);
		FocusedActor = nullptr;
		Candidates.Remove(OtherActor);
	}
	RefreshFocusedActor();
}

void UInteractionComponent::ServerExecuteInteract_Implementation(AActor* InFocusedActor)
{
	IInteractionInterface::Execute_InteractWithMe(InFocusedActor, GetOwner());
}

void UInteractionComponent::RefreshFocusedActor()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		if (FocusedActor) SetHighlightMaterial(FocusedActor, false);
		FocusedActor = nullptr;
		return;
	}

	// Choose the closest valid candidate
	float ClosestDistSqr = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;

	const FVector Origin = Owner->GetActorLocation();

	for (const TWeakObjectPtr<AActor>& Weak : Candidates)
	{
		AActor* A = Weak.Get();
		if (!IsValid(A)) continue;
		if (!A->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass())) continue;

		const float DistSqr = FVector::DistSquared(Origin, A->GetActorLocation());
		if (DistSqr < ClosestDistSqr)
		{
			ClosestDistSqr = DistSqr;
			ClosestActor = A;
		}
	}

	if (ClosestActor)
	{
		if (FocusedActor)
		{
			SetHighlightMaterial(FocusedActor, false);
		}
		
		FocusedActor = ClosestActor;
		
		if (FocusedActor)
		{
			SetHighlightMaterial(FocusedActor, true);
		}
	}
}

void UInteractionComponent::SetHighlightMaterial(AActor* Actor, bool bEnabled)
{
	if (!Actor) return;

	TArray<UActorComponent*> Meshes;
	Actor->GetComponents(UStaticMeshComponent::StaticClass(), Meshes);
	
	for (UActorComponent* MC : Meshes)
	{
		UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(MC);
		if (!Mesh) continue;

		if (bEnabled)
		{
			if (!SavedOriginalMaterials.Contains(Mesh))
			{
				UMaterialInterface* Original = Mesh->GetMaterial(0);
				SavedOriginalMaterials.Add(Mesh, Original);
			}

			if (HighlightMaterial)
			{
				Mesh->SetOverlayMaterial(HighlightMaterial);
			}
		}
		else
		{
			if (UMaterialInterface** Found = SavedOriginalMaterials.Find(Mesh))
			{
				Mesh->SetOverlayMaterial(*Found);
				SavedOriginalMaterials.Remove(Mesh);
			}
		}
	}
	
}

void UInteractionComponent::TryInteract()
{
	// Use the freshest target
	RefreshFocusedActor();
	if (!FocusedActor) return;

	// Client asks server to do the authoritative call
	if (GetOwner())
	{
		ServerExecuteInteract(FocusedActor);
		RefreshFocusedActor();
	}
}











