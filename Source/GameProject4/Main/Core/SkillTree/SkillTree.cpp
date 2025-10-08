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

 //void USkillTree::ShowSkillTooltip(const FText& Name, const FText& Description, const FVector2D& ScreenPosition)
 //{
 //	/*if (SkillTooltip)
 //	{
 //		SkillTooltip->SetSkillTooltipInfo(Name, Description);
 //		SkillTooltip->SetVisibility(ESlateVisibility::Visible);
 //		SkillTooltip->SetPositionInViewport(ScreenPosition); 
 //	}*/
 //}

 //void USkillTree::HideSkillTooltip()
 //{
 //	/*if (SkillTooltip)
 //	{
 //		SkillTooltip->SetVisibility(ESlateVisibility::Hidden);
 //	}*/
 //}


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