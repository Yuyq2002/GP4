// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTree.h"

#include "ToggleSkillTreeUI.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "GameProject4/Main/Core/ExperienceSystem/ExperienceContainer.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "GameFramework/PawnMovementComponent.h"

UToggleSkillTreeUI* SkillTreeWidget;
USkill* PendingConnectionStart = nullptr;

void USkillTree::NativeConstruct()
{
	Super::NativeConstruct();
	// if (TooltipBox)
	// {
	// 	TooltipBox->SetVisibility(ESlateVisibility::Collapsed);
	// }
	
	ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter)
	{
		if (APlayerController* PlayerController = GetOwningPlayer())
		{
			PlayerCharacter = Cast<ACharacter>(PlayerController ? PlayerController->GetPawn() : nullptr);
		}
	}
	if (PlayerCharacter)
	{
		ExperienceComponent = PlayerCharacter->FindComponentByClass<UExperienceContainer>();
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

void USkillTree::CloseSkillTree()
{
	APlayerController* LocalPlayerController = GetOwningPlayer();
	if (!LocalPlayerController)
	{
		return;
	}

	APawn* Pawn = LocalPlayerController->GetPawn();
	if (Pawn && Pawn->GetMovementComponent())
	{
		Pawn->GetMovementComponent()->Activate();
	}

	SetVisibility(ESlateVisibility::Collapsed);
	LocalPlayerController->SetInputMode(FInputModeGameOnly());
	LocalPlayerController->SetShowMouseCursor(true);

	bIsSkillTreeToggled = true;
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

// void USkillTree::ShowSkillTooltip(const FText& SkillName, const FText& Description, const FVector2D& Position)
// {
// 	if (TooltipBox && TooltipTextDiscription)
// 	{
// 		FText FormattedText = FText::Format(
// 			NSLOCTEXT("SkillTree", "TooltipFormat", "{0}\n\n{1}"),
// 			SkillName,
// 			Description
// 		);
// 		TooltipTextDiscription->SetText(FormattedText);
// 		
// 		if (UCanvasPanelSlot* TooltipSlot = Cast<UCanvasPanelSlot>(TooltipBox->Slot))
// 		{
// 			TooltipSlot->SetPosition(Position + FVector2D(10.0f, 10.0f)); // Offset from cursor
// 		}
// 		
// 		TooltipBox->SetVisibility(ESlateVisibility::HitTestInvisible);
// 	}
// }
//
// void USkillTree::HideSkillTooltip()
// {
// 	if (TooltipBox)
// 	{
// 		TooltipBox->SetVisibility(ESlateVisibility::Collapsed);
// 	}
// }