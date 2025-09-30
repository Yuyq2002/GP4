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


// void USkillTree::AddSkillNode(FVector2D Position, int32 Cost, ESkillType Type)
// {
// 	if (!CanvasPanel) return;
// 	USkill* NewSkill = CreateWidget<USkill>(GetWorld(), SkillWidgetClass);
// 	SkillNodes.Add(NewSkill);
//
// 	UCanvasPanelSlot* CanvasPanelSlot = CanvasPanel->AddChildToCanvas(NewSkill);
// 	if (CanvasPanelSlot)
// 	{
// 		CanvasPanelSlot->SetAutoSize(true);
// 		CanvasPanelSlot->SetPosition(Position);
// 	}
// }
//
// void USkillTree::OnSkillClicked(USkill* ClickedSkill)
// {
// 	if (!PendingConnectionStart)
// 	{
// 		PendingConnectionStart = ClickedSkill;
// 	}
// 	else
// 	{
// 		DrawConnection(PendingConnectionStart, ClickedSkill);
// 		PendingConnectionStart = nullptr;
// 	}
// }
//
// void USkillTree::DrawConnection(USkill* From, USkill* To)
// {
// 	if (!CanvasPanel || !From || !To) return;
//
// 	FVector2D Start = GetWidgetCenter(From);
// 	FVector2D End = GetWidgetCenter(To);
//
// 	UImage* NewLine = NewObject<UImage>(CanvasPanel);
// 	if (NewLine)
// 	{
// 		ConnectionLines.Add(NewLine);
// 		CanvasPanel->AddChild(NewLine);
// 		
// 		FVector2D Dir = End - Start;
// 		float Length = Dir.Size();
// 		float Angle = FMath::RadiansToDegrees(FMath::Atan2(Dir.Y, Dir.X));
//
// 		UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(NewLine->Slot);
// 		if (CanvasPanelSlot)
// 		{
// 			CanvasPanelSlot->SetPosition(Start);
// 			CanvasPanelSlot->SetSize(FVector2D(Length, 2.f));
// 			CanvasPanelSlot->SetAlignment(FVector2D(0.f, 0.5f));
// 			NewLine->SetRenderTransformAngle(Angle);
// 		}
// 	}
// }
//
// FVector2D USkillTree::GetWidgetCenter(UUserWidget* Widget)
// {
// 	if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Widget->Slot))
// 	{
// 		return CanvasPanelSlot->GetPosition() + CanvasPanelSlot->GetSize() * 0.5f;
// 	}
// 	return FVector2D::ZeroVector;
// }

