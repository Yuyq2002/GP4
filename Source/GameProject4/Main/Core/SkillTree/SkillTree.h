// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "Blueprint/UserWidget.h"
#include "SkillTree.generated.h"

class UToolTip;
class UExperienceContainer;
/**
 * 
 */
class UBorder;
class UCanvasPanel;
class UImage;
class UTextBlock;
class USkill;
class UScaleBox;
class UModifiedPlayerStats;
UCLASS()
class GAMEPROJECT4_API USkillTree : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional), Category = "Tooltip")
	// UToolTip* SkillTooltip;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* SkillPointsText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UCanvasPanel* CanvasPanel;

//for making the skill tree more easily customizable
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TArray<USkill*> SkillNodes;

	UPROPERTY(EditAnywhere, Category = "Skills")
	UModifiedPlayerStats* PlayerStats;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skills")
	TSubclassOf<class USkill> SkillWidgetClass;
	bool bIsSkillTreeToggled = true;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
public:
	UFUNCTION(BlueprintCallable, Category="Skills")
	void SetPlayerStats(UModifiedPlayerStats* InStats) { PlayerStats = InStats; }
	UFUNCTION(BlueprintCallable, Category="UI")
	void CloseSkillTree();
	// UFUNCTION(BlueprintCallable)
	// void ShowSkillTooltip(const FText& Name, const FText& Description, const FVector2D& ScreenPosition);
	//
	// UFUNCTION(BlueprintCallable)
	// void HideSkillTooltip();

private:
	UPROPERTY()
	UExperienceContainer* ExperienceComponent;

	UFUNCTION()
	void HandleLevelUp(AActor* OwnerActor, int32 NewSkillPoints);

	void RefreshSkillPoints();

};
