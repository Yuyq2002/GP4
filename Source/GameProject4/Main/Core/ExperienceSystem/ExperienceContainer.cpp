// Fill out your copyright notice in the Description page of Project Settings.


#include "ExperienceContainer.h"

// Sets default values for this component's properties
UExperienceContainer::UExperienceContainer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//Experience stuff
	CurrentExperience = 0;
	SkillPoints = 0;
	ExpRequiredForSkillPoint = 3;
	
	// ...
}

void UExperienceContainer::AddExperience(int32 Amount)
{
	CurrentExperience += Amount;

	UE_LOG(LogTemp, Warning, TEXT("Gained %d XP. Current XP: %d / %d (Skill Points: %d)"), 
	Amount, CurrentExperience, ExpRequiredForSkillPoint, SkillPoints);
	
	int32 PreviousSkillPoints = SkillPoints;
	
	while (CurrentExperience >= ExpRequiredForSkillPoint)
	{
		CurrentExperience -= ExpRequiredForSkillPoint;

		OnSkillPointGained();
	}
	if (SkillPoints > PreviousSkillPoints)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Points Gained! Total Skill Points: %d"), SkillPoints);
		OnLevelUp.Broadcast(GetOwner(), SkillPoints);
	}

}

void UExperienceContainer::OnSkillPointGained()
{
	SkillPoints++;
	UE_LOG(LogTemp, Warning, TEXT("Skill Point Gained! Total Skill Points: %d"), SkillPoints);
	OnSkillPointsChanged.Broadcast(GetOwner(), SkillPoints);
}


// Called when the game starts
void UExperienceContainer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UExperienceContainer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


