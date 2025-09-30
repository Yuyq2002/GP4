// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "Logging/LogMacros.h"
#include "Main/Core/Data/Interface/ControllableInterface.h"
#include "ToggleSkillTreeUI.generated.h"

class UInputAction;
class UUserWidget;
struct FInputActionValue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))

/**
 * 
 */
class GAMEPROJECT4_API UToggleSkillTreeUI : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UToggleSkillTreeUI();

	void BindInput(class UEnhancedInputComponent* EnhancedInputComponent);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ToggleSkillTreeAction;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> SkillTreeWidgetClass;
	
	void ToggleSkillTree();

	bool isSkillTreeToggled = true;
	
	UPROPERTY()
	APlayerController* LocalPlayerController;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	UUserWidget* SkillTreeWidget;
};
