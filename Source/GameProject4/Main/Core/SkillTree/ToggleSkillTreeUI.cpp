// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleSkillTreeUI.h"

#if WITH_EDITOR
#include "EditorCategoryUtils.h"
#endif

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/PawnMovementComponent.h"


// Sets default values for this component's properties
UToggleSkillTreeUI::UToggleSkillTreeUI(): ToggleSkillTreeAction(nullptr),
                                          LocalPlayerController(nullptr),
                                          SkillTreeWidget(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UToggleSkillTreeUI::BindInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (!EnhancedInputComponent || !ToggleSkillTreeAction)
	{
		return;
	}

	EnhancedInputComponent->BindAction(ToggleSkillTreeAction, ETriggerEvent::Started, 
		this, &UToggleSkillTreeUI::ToggleSkillTree);

}

// void UToggleSkillTreeUI::SetupInputComponent(class UEnhancedInputComponent* EnhancedInputComponent)
// {
// 	if (EnhancedInputComponent && ToggleSkillTreeAction)
// 	{
// 		EnhancedInputComponent->BindAction(ToggleSkillTreeAction, ETriggerEvent::Started, 
// 			this, &UToggleSkillTreeUI::ToggleSkillTree);
// 	}
// }


// Called when the game starts
void UToggleSkillTreeUI::BeginPlay()
{
	Super::BeginPlay();
	//failsafe for multiplayer so that UI only opens on client side
	APawn* PlayerPawn = Cast<APawn>(GetOwner());
	if (!PlayerPawn || !PlayerPawn->IsLocallyControlled())
	{
		return;
	}
	
	// if (ToggleSkillTreeAction)
	// {
	// 	SkillTreeWidget = CreateWidget(GetWorld(), SkillTreeWidgetClass);
	// 	SkillTreeWidget->AddToViewport();
	// 	SkillTreeWidget->SetVisibility(ESlateVisibility::Collapsed);
	// }
	//
	// LocalPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	LocalPlayerController = Cast<APlayerController>(PlayerPawn->GetInstigatorController());
	
	// if (!LocalPlayerController)
	// {
	// 	return;
	// }

	if (LocalPlayerController && SkillTreeWidgetClass)
	{
		SkillTreeWidget = CreateWidget<UUserWidget>(LocalPlayerController, SkillTreeWidgetClass);
		if (SkillTreeWidget)
		{
			SkillTreeWidget->AddToViewport();
			SkillTreeWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	// if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerPawn->InputComponent))
	// {
	// 	if (ToggleSkillTreeAction)
	// 	{
	// 		EnhancedInputComponent->BindAction(ToggleSkillTreeAction, ETriggerEvent::Started, 
	// 			this, &UToggleSkillTreeUI::ToggleSkillTree);
	// 	}
	// }

}
//logic for opening skill tree UI, rework this into its own component after testing is finished
// ...

void UToggleSkillTreeUI::ToggleSkillTree()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Skill Tree Toggled"));
	if (!SkillTreeWidget || !LocalPlayerController)
	{
		return;
	}
	APawn* Pawn = LocalPlayerController->GetPawn();
	if (!Pawn)
	{
		return;
	}
	
	if (isSkillTreeToggled)
	{
		SkillTreeWidget->SetVisibility(ESlateVisibility::Visible);
		LocalPlayerController->SetInputMode(FInputModeGameAndUI());
		LocalPlayerController->SetShowMouseCursor(true);

		if (Pawn->GetMovementComponent())
		{
			Pawn->GetMovementComponent()->Deactivate();
		}
		
		isSkillTreeToggled = false;
	}
	else
	{
		SkillTreeWidget->SetVisibility(ESlateVisibility::Collapsed);
		LocalPlayerController->SetInputMode(FInputModeGameOnly());
		LocalPlayerController->SetShowMouseCursor(false);
		
		if (Pawn->GetMovementComponent())
		{
			Pawn->GetMovementComponent()->Activate();
		};
		
		isSkillTreeToggled = true;
	}

}
// Called every frame
void UToggleSkillTreeUI::TickComponent(float DeltaTime, ELevelTick TickType,
									   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

