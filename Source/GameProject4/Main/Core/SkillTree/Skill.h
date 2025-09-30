// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Skill.generated.h"
class USkillData;
/**
 * 
 */
class UButton;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillClicked, USkill*, ClickedSkill);
UENUM()
enum class ESkillType : uint8
{
	Defense UMETA(DisplayName = "Defense"),
	Offense UMETA(DisplayName = "Offense"),
	Cunning UMETA(DisplayName = "Cunning")
};

UCLASS()
class GAMEPROJECT4_API USkill : public UUserWidget
{
	GENERATED_BODY()

	//Functions for deducting skill points for purchasing skills
public:
	// UFUNCTION(BlueprintCallable, Category="Skill")
	// static bool ArePrerequisitesUnlocked(const USkill* ForSkill);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	USkillData* SkillData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill")
	FGameplayTag SkillTag;
	UFUNCTION(BlueprintCallable, Category = "Skills")
	bool IsPurchasable();
	UFUNCTION(BlueprintCallable, Category = "Skills")
	void PurchaseSkill();
	UFUNCTION(BlueprintCallable, Category = "Skills")
	bool IsUnlocked() const { return bIsSkillUnlocked; }
	//Functions for deducting skill points for purchasing skills

	//for a more customizable skill tree
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSkillClicked OnSkillClicked;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	// TArray<USkill*> ConnectedSkills;
	
	bool bIsSkillUnlocked = false;


	
protected:
	//Functions for deducting skill points for purchasing skills
	UFUNCTION()
	void OnButtonClicked();
	//Functions for deducting skill points for purchasing skills
	
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UButton* Button;

	// UPROPERTY(EditAnywhere, Category = "Skills | Type")
	// ESkillType SkillType;
	//
	// UPROPERTY(EditAnywhere, Category = "Skills | Cost")
	// int32 Cost;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	// FVector2D NodePosition;


};
