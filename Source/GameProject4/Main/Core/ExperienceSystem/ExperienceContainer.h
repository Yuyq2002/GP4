// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameProject4/Main/Core/SkillTree/SkillData.h"
#include "ExperienceContainer.generated.h"
/*
Experience System Component: this is the container that keeps track of the total experience and skill points earned per actor.
This is a component so it can be attached to any object that would need to track its experience. 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelUp, AActor*, OwnerActor, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillPointsChanged, AActor*, OwnerActor, int32, NewSkillPoints);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPROJECT4_API UExperienceContainer : public UActorComponent
{
	GENERATED_BODY()

	//Experience Orb stuff
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Experience", meta = (AllowPrivateAccess = "true"))
	int32 CurrentExperience;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Experience", meta = (AllowPrivateAccess = "true"))
	int32 SkillPoints;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Experience", meta = (AllowPrivateAccess = "true"))
	int32 ExpRequiredForSkillPoint;
	
	// Unlocked skills container
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Skills", meta = (AllowPrivateAccess = "true"))
	TArray<FGameplayTag> UnlockedSkillTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Skills", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer UnlockedTagsContainer;


public:	
	// Sets default values for this component's properties
	UExperienceContainer();
	//exp stuff
	UFUNCTION(BlueprintCallable, Category="Experience", meta = (AllowPrivateAccess = "true"))
	void AddExperience(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Experience", meta = (AllowPrivateAccess = "true"))
	void OnSkillPointGained();
	//For skill tree point distribution
	UFUNCTION(BlueprintCallable, Category = "Experience")
	int32 GetSkillPoints() const { return SkillPoints; }
    
	UFUNCTION(BlueprintCallable, Category = "Experience")
	void SetSkillPoints(int32 NewValue)
	{
		const int32 Clamped = FMath::Max(0, NewValue);
		if (SkillPoints != Clamped)
		{
			SkillPoints = Clamped;
			OnSkillPointsChanged.Broadcast(GetOwner(), SkillPoints);
		}
	}

	//for broadcast event
	UPROPERTY(BlueprintCallable, Category = "Experience")
	FOnLevelUp OnLevelUp;
	UPROPERTY(BlueprintCallable, Category="Experience")
	FOnSkillPointsChanged OnSkillPointsChanged;
	// Accessors for unlocked tags
	UFUNCTION(BlueprintCallable, Category="Skills")
	const FGameplayTagContainer& GetUnlockedTags() const { return UnlockedTagsContainer; }

	UFUNCTION(BlueprintCallable, Category="Skills")
	bool IsTagUnlocked(FGameplayTag Tag) const { return UnlockedTagsContainer.HasTag(Tag); }

	UFUNCTION(BlueprintCallable, Category="Skills")
	void AddUnlockedTag(FGameplayTag Tag)
	{
		if (!UnlockedTagsContainer.HasTag(Tag))
		{
			UnlockedTagsContainer.AddTag(Tag);
			UnlockedSkillTags.Add(Tag);
		}
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
		
};
