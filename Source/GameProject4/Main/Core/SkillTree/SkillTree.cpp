// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTree.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "GameProject4/Main/Core/ExperienceSystem/ExperienceContainer.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

USkill* PendingConnectionStart = nullptr;

void USkillTree::NativeConstruct()
{
	Super::NativeConstruct();
	ACharacter* PC = Cast<ACharacter>(GetOwningPlayerPawn());
	if (!PC)
	{
		if (APlayerController* PCtrl = GetOwningPlayer())
		{
			PC = Cast<ACharacter>(PCtrl ? PCtrl->GetPawn() : nullptr);
		}
	}
	if (PC)
	{
		ExperienceComponent = PC->FindComponentByClass<UExperienceContainer>();
		if (ExperienceComponent)
		{
			ExperienceComponent->OnLevelUp.AddDynamic(this, &USkillTree::HandleLevelUp);
			ExperienceComponent->OnSkillPointsChanged.AddDynamic(this, &USkillTree::HandleLevelUp);
			RefreshSkillPoints();
		}
	}
}

FReply USkillTree::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void USkillTree::HandleLevelUp(AActor* OwnerActor, int32 NewSkillPoints)
{
	RefreshSkillPoints();
}

void USkillTree::RefreshSkillPoints()
{
	if (SkillPointsText && ExperienceComponent)
	{
		const int32 SkillPoint = ExperienceComponent->GetSkillPoints();

		FText DisplayText = FText::Format(
			NSLOCTEXT("SkillTree", "SkillPointsLabel", "Skill Points:\n{0}"),
			FText::AsNumber(SkillPoint)
		);

		SkillPointsText->SetText(DisplayText);
	}
}
