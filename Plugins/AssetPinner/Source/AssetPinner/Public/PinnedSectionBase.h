// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "PinnedSectionBase.generated.h"

class UWrapBox;
class UEditorUtilityButton;

/**
 * 
 */
UCLASS()
class ASSETPINNER_API UPinnedSectionBase : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct();
	bool GetEditMode();

private:
	UFUNCTION()
	void OnListChangedCallback(const TArray<FString>& List, const TArray<bool>& StatusList);
	void Refresh(const TArray<FString>& List, const TArray<bool>& StatusList);

	UFUNCTION()
	void OnClearButtonClicked();

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);


private:
	bool EditMode;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<UEditorUtilityWidget> AssetSlotWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget))
	UWrapBox* PinnedWrapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget))
	UWrapBox* RecentWrapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget))
	UEditorUtilityButton* ClearButton;
};
