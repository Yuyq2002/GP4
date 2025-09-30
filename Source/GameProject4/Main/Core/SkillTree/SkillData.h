// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SkillData.generated.h"

class USkill;
class UActor;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FSkillDefinition
{
	GENERATED_BODY()
	static int32 SkillCounter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill")
	FName SkillID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill")
	FText SkillName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill")
	int32 Cost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill", meta = (Categories = "Skill"))
	//TArray<FGameplayTag> PrerequisiteSkills;
	FGameplayTagContainer PrerequisiteSkillsContainer;
	
};

// UCLASS(BlueprintType)
// class GAMEPROJECT4_API USkillData : public UDataAsset
// {
// 	GENERATED_BODY()
// public:
//
//
// 	
// };

UCLASS(BlueprintType)
class GAMEPROJECT4_API USkillData : public UDataAsset
{
	GENERATED_BODY()

public:
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
	UFUNCTION(BlueprintCallable, Category="Skill")
	static bool ArePrerequisitesUnlockedByTags(const USkillData* Data, const FGameplayTag& SkillTag, const FGameplayTagContainer& UnlockedTags);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ForceInlineRow, Categories = "Skill"))
	TMap<FGameplayTag, FSkillDefinition> Skills;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill")
	// FName SkillID;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill")
	// FText SkillName;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill")
	// int32 Cost;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill")
	// TArray<USkillData*> PrerequisiteSkills;
	
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skill")
	//ESkillType SkillType;
};
