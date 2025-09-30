// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillData.h"
#include "Skill.h"
//Autogenerates an ID for each skill in the data asset
#if WITH_EDITOR
void USkillData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);


	if (PropertyChangedEvent.Property &&
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(USkillData, Skills))
	{
		int32 ID = 0;

		for (auto& Elem : Skills)
		{
			FSkillDefinition& SkillDef = Elem.Value;
			
			if (SkillDef.SkillID.IsValid())
			{
				SkillDef.SkillID = FName(*FString::Printf(TEXT("Skill_%d"), ID++));
			}
		}
	}
}
#endif
bool USkillData::ArePrerequisitesUnlockedByTags(const USkillData* Data, const FGameplayTag& SkillTag, const FGameplayTagContainer& UnlockedTags)
{
	// if (!ForSkill || !ForSkill->SkillData)
	// {
	// 	return false;
	// }
	//
	// const TArray<USkillData*>& PrereqDatas = ForSkill->SkillData->PrerequisiteSkills;
	//
	// if (PrereqDatas.Num() == 0)
	// {
	// 	return true;
	// }
	//
	// for (const USkillData* PrereqData : PrereqDatas)
	// {
	// 	bool isPrereqMet = false;
	//
	// 	for (const USkill* ConnectedSkill : ForSkill->ConnectedSkills)
	// 	{
	// 		if (IsValid(ConnectedSkill) &&
	// 			ConnectedSkill->SkillData == PrereqData &&
	// 			ConnectedSkill->IsUnlocked())
	// 		{
	// 			isPrereqMet = true;
	// 			break;
	// 		}
	// 	}
	//
	// 	if (!isPrereqMet)
	// 	{
	// 		return false; 
	// 	}
	// }
	// return true;
	if (!Data)
	{
		return false;
	}
	
	if (const FSkillDefinition* Def = Data->Skills.Find(SkillTag))
	{
		const FGameplayTagContainer& PrerequisiteTags = Def->PrerequisiteSkillsContainer;
		
		for (const FGameplayTag& PrerequisiteTag : PrerequisiteTags)
		{
			if (!UnlockedTags.HasTag(PrerequisiteTag))
			{
				return false; 
			}
		}

		return true;
	}

	return false;
}
