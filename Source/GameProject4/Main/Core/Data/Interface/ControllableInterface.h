// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ControllableInterface.generated.h"

/*
This class does not need to be modified.
Empty class for reflection system visibility.
Uses the UINTERFACE macro.
Inherits from UInterface.
*/
UINTERFACE(MinimalAPI, Blueprintable)
class UControllableInterface : public UInterface
{
	GENERATED_BODY()
};

/* Actual Interface declaration. */
class IControllableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void PrimaryAction();
	virtual void PrimaryAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void SecondaryAction();
	virtual void Secondary_Implementation() {};

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void DashAction();
	virtual void DashAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void RotationAction();
	virtual void RotationAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void RotationStartAction();
	virtual void RotationStartAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void RotationEndAction();
	virtual void RotationEndAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void TabAction();
	virtual void TabAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void MoveAction(const FVector& Value);
	virtual void MoveAction_Implementation(const FVector& Value) {};

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void InteractAction();
	virtual void InteractAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void UsePotionAction();
	virtual void UsePotionAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void UseAuraAction();
	virtual void UseAuraAction_Implementation() {};
};