// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill.h"
#include "SkillData.h"
#include "GameProject4/Main/Core/ExperienceSystem/ExperienceContainer.h"
#include "Components/Button.h"
#include "GameFrameWork/Character.h"

class UExperienceContainer;
void USkill::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button)
	{
		Button->OnClicked.AddDynamic(this, &USkill::OnButtonClicked);
	}
}

bool USkill::IsPurchasable()
{
	if(!SkillData)
	{
		return false;
	}
	
	if (bIsSkillUnlocked)
	{
		return false;
	}
	//Prevents nodes from being picked out of sequence
	// if (!USkillData::ArePrerequisitesUnlocked(this, ConnectedSkills))
	// {
	// 	return false;
	// }
	// if (PrerequisiteSkills.Num() > 0)
	// {
	// 	for (USkill* Prereq : PrerequisiteSkills)
	// 	{
	// 		if (IsValid(Prereq))
	// 		{
	// 			if (!Prereq->isSkillUnlocked)
	// 			{
	// 				return false;
	// 			}
	// 		}
	// 	}
	// }
	
	ACharacter* Character = Cast<ACharacter>(GetOwningPlayerPawn());
	if (!Character){return false;}


	UExperienceContainer* ExpComp = Character->FindComponentByClass<UExperienceContainer>();
	if (!ExpComp)
	{
		return false;
	}
	
	// reads from SkillData
	// const int32 CostFromData = SkillData->Cost;
	// return ExpComp->GetSkillPoints() >= CostFromData;
	// if (const FSkillDefinition* Def = SkillData->Skills.Find(SkillTag))
	// {
	// 	return ExpComp->GetSkillPoints() >= Def->Cost;
	// }
	if (const FSkillDefinition* Def = SkillData->Skills.Find(SkillTag))
	{
		if (Def->PrerequisiteSkillsContainer.Num() > 0)
		{
			if (!USkillData::ArePrerequisitesUnlockedByTags(SkillData, SkillTag, ExpComp->GetUnlockedTags()))
			{
				return false;
			}
		}
		if (ExpComp->GetSkillPoints() < Def->Cost)
		{
			return false;
		}
		return true;
	}
	return false;
}

void USkill::PurchaseSkill()
{
	if (!IsPurchasable())
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("You sure you have enough skill points?"));
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwningPlayerPawn());
	if (!Character)
		return;

	UExperienceContainer* ExpComp = Character->FindComponentByClass<UExperienceContainer>();
	if (!ExpComp)
		return;

	if (const FSkillDefinition* Def = SkillData->Skills.Find(SkillTag))
	{
		const int32 CostFromData = Def->Cost;
	
		ExpComp->SetSkillPoints(ExpComp->GetSkillPoints() - CostFromData);
		bIsSkillUnlocked = true;
		OnSkillClicked.Broadcast(this);
		UE_LOG(LogTemp, Warning, TEXT("Got mad skills! Cost: %d, Remaining Skill Points: %d"), 
			CostFromData, ExpComp->GetSkillPoints());
		ExpComp->AddUnlockedTag(SkillTag);
		if (GEngine)
		{
			FString Message = FString::Printf(TEXT("Mad skill acquired! -%d points (%d remaining)"), 
				CostFromData, ExpComp->GetSkillPoints());
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, Message);
		}

		if (Button)
		{
			Button->SetIsEnabled(false);
		}
		OnSkillClicked.Broadcast(this);
	}


}

void USkill::OnButtonClicked()
{
	PurchaseSkill();
	OnSkillClicked.Broadcast(this); //broadcast for skill customization
}