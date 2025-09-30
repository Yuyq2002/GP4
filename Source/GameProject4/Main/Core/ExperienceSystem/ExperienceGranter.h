// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExperienceGranter.generated.h"
/*
Experience Granted Component: This component is what handles giving out the experience. Attach it to any object that should grant exp upon destruction. 
Calling the function can be done either through code: 

Attach UExperienceGranted to them.

In the health/damage system, when the enemy dies, call:

if (UExperienceGranted* GrantComp = Enemy->FindComponentByClass<UExperienceGranted>())
{
	GrantComp->GrantExperienceTo(KillerActor);
}

Or as a blueprint:

Keyboard event-> destroy actor
Event BeginPlay-> Enable Input target is actor, then assign Get Player Controller to the object.

This seemed to work best when I added the logic to the level blueprint:

Keyboard event-> GetAllActorsOfClass (with reference to what we want to destroy) -> Destroy Actor
EventBeginPlay-> EnableInput with Get Player Controller


 */ 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetLevelUp, AActor*, TargetActor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPROJECT4_API UExperienceGranter : public UActorComponent
{
	GENERATED_BODY()

public:
	// Daniel asked me to add random ranges for the experience :p 
	UExperienceGranter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
	int32 MinExperienceValue = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
	int32 MaxExperienceValue = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
	int32 RandomExperienceValue = FMath::RandRange(MinExperienceValue, MaxExperienceValue);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Experience")
	bool bGrantOnDeath = true;
	//broadcast event
	UPROPERTY(BlueprintAssignable, Category = "Experience")
	FOnTargetLevelUp OnTargetLevelUp;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleDestroyedActor(AActor* DestroyedActor);

	UFUNCTION()
	void HandleTargetLeveledUp(AActor* LeveledActor, int32 NewLevel);


public:
	//call this to grant xp to target actor
	UFUNCTION(BlueprintCallable, Category="Experience")
	void GrantExperienceTo(AActor* TargetActor);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
};
