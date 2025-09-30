// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetPinner.h"
#include "ToolMenus.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "EditorUtilitySubsystem.h"

#define LOCTEXT_NAMESPACE "FAssetPinnerModule"

void FAssetPinnerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FAssetPinnerModule::RegisterMenuExtension));
}

void FAssetPinnerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

        // Unregister the startup function
    UToolMenus::UnRegisterStartupCallback(this);

    // Unregister all our menu extensions
    UToolMenus::UnregisterOwner(this);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAssetPinnerModule, AssetPinner)

void FAssetPinnerModule::RegisterMenuExtension()
{
    // Use the current object as the owner of the menus
    // This allows us to remove all our custom menus when the
    // module is unloaded (see ShutdownModule below)
    FToolMenuOwnerScoped OwnerScoped(this);

    // Extend the "File" section of the main toolbar
    UToolMenu* AssetsToolBar = UToolMenus::Get()->ExtendMenu(
        "LevelEditor.LevelEditorToolBar.ModesToolBar");
    FToolMenuSection& ToolbarSection = AssetsToolBar->FindOrAddSection("File");

    ToolbarSection.AddEntry(FToolMenuEntry::InitToolBarButton(
        TEXT("AssetPinner"),
        FExecuteAction::CreateLambda([]()
            {
                UObject* WidgetObj = LoadObject<UObject>(nullptr, TEXT("/AssetPinner/BPW_PinnedAssetSection"));
                UEditorUtilityWidgetBlueprint* WidgetBP = Cast<UEditorUtilityWidgetBlueprint>(WidgetObj);

                if (WidgetBP)
                {
                    UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
                    EditorUtilitySubsystem->SpawnAndRegisterTab(WidgetBP);
                }
            }),
        INVTEXT("Asset Pinner"),
        INVTEXT("Open the asset pinner"),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "ViewportActorPreview.Pinned")
    ));
}
