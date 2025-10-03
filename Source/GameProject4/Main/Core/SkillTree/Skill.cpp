// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill.h"
#include "SkillData.h"
#include "GameProject4/Main/Core/ExperienceSystem/ExperienceContainer.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFrameWork/Character.h"

class UExperienceContainer;
void USkill::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button)
	{
		Button->OnClicked.AddDynamic(this, &USkill::OnButtonClicked);
	}
	if (SkillCostText)
	{
		int32 Cost = GetSkillCost();
		SkillCostText->SetText(FText::AsNumber(Cost));
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
	
	ACharacter* Character = Cast<ACharacter>(GetOwningPlayerPawn());
	if (!Character){return false;}


	UExperienceContainer* ExpComp = Character->FindComponentByClass<UExperienceContainer>();
	if (!ExpComp)
	{
		return false;
	}
	
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

int32 USkill::GetSkillCost() const
{
	if (!SkillData) return 0;

	if (const FSkillDefinition* Def = SkillData->Skills.Find(SkillTag))
	{
		return Def->Cost;
	}

	return 0;
}

void USkill::OnButtonClicked()
{
	PurchaseSkill();
	OnSkillClicked.Broadcast(this); //broadcast for skill customization
}
