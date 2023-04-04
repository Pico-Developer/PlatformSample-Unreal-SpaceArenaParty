﻿#include "PICOXREditor.h"
#include "IMotionController.h"
#include "Features/IModularFeatures.h"
#include "LevelEditor.h"
#include "Engine/Engine.h"
#include "ISettingsModule.h"
#include "PICOXREditorSettings.h"
#include "DetailCategoryBuilder.h"
#include "Widgets/Input/SButton.h"
#include "DetailWidgetRow.h"
#include "PICOXREditorCommands.h"
#include "Runtime/Core/Public/CoreGlobals.h"
#include "Runtime/Core/Public/Misc/ConfigCacheIni.h"
#include "ToolMenus.h"
#include "PXR_HMDRuntimeSettings.h"
#include "PXR_InputState.h"


static const FName PICOXREditorTabName("PICOXREditor");

#define LOCTEXT_NAMESPACE "FPICOXREditorModule"

void FPICOXREditorModule::StartupModule()
{
	RegisterSettings();
	UPICOXRSettings* Settings = GetMutableDefault<UPICOXRSettings>();
	if (Settings->IsValidLowLevel()
		&&Settings->bEnableAutoKeyMapping)
	{
		AddSampleInputs();
	}

	FPICOXREditorStyle::Initialize();
	FPICOXREditorStyle::ReloadTextures();

	FPICOXREditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	// Dummy action for main toolbar button
	PluginCommands->MapAction(
		FPICOXREditorCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FPICOXREditorModule::PluginButtonClicked),
		FCanExecuteAction());

	// Launch Bindings URL
	PluginCommands->MapAction(
		FPICOXREditorCommands::Get().LaunchAPIReferenceURL,
		FExecuteAction::CreateRaw(this, &FPICOXREditorModule::LaunchAPIReferenceURL),
		FCanExecuteAction());

	// Launch Bindings URL
	PluginCommands->MapAction(
		FPICOXREditorCommands::Get().LaunchSampleDemoURL,
		FExecuteAction::CreateRaw(this, &FPICOXREditorModule::LaunchSampleDemoURL),
		FCanExecuteAction());
	
	// Add Sample Inputs
	PluginCommands->MapAction(
		FPICOXREditorCommands::Get().AddSampleInputs,
		FExecuteAction::CreateRaw(this, &FPICOXREditorModule::AddSampleInputs),
		FCanExecuteAction());

	// Clear Sample Inputs
	PluginCommands->MapAction(
		FPICOXREditorCommands::Get().ClearSampleInputs,
		FExecuteAction::CreateRaw(this, &FPICOXREditorModule::ClearSampleInputs),
		FCanExecuteAction());

	FPICOXREditorStyle MenuStyle = FPICOXREditorStyle();
	MenuStyle.Initialize();

	UToolMenu* ToolBar = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
	FToolMenuSection& Section = ToolBar->FindOrAddSection("Settings");
	Section.AddEntry(FToolMenuEntry::InitComboButton(
		"PICOXR",
		FUIAction(
			FExecuteAction(),
			FCanExecuteAction(),
			FGetActionCheckState(),
			FIsActionButtonVisible::CreateRaw(this, &FPICOXREditorModule::ShowPICOXRInputToolbarDropdown)
		),
		FOnGetContent::CreateRaw(this, &FPICOXREditorModule::FillComboButton, PluginCommands),
		LOCTEXT("PICOXRInputBtn", "PICOXR"),
		LOCTEXT("PICOXRInputBtnTootlip", "PICOXR"),
		FSlateIcon(FPICOXREditorStyle::GetStyleSetName(), "PICOXREditor.PluginAction")
	));
}

bool FPICOXREditorModule::ShowPICOXRInputToolbarDropdown()
{
	// Only add the toolbar if PICOXR is the currently active tracking system
	UPICOXREditorSettings* PICOXREditorSettings = GetMutableDefault<UPICOXREditorSettings>();

	if (PICOXREditorSettings->bShowPICOXRInputToolbarButton)
	{
		return true;
	}
	
	return false;
}

void FPICOXREditorModule::ShutdownModule()
{
	FPICOXREditorStyle::Shutdown();

	FPICOXREditorCommands::Unregister();
}

void FPICOXREditorModule::PluginButtonClicked()
{
	// Empty on purpose
}

void FPICOXREditorModule::LaunchAPIReferenceURL()
{
	FPlatformProcess::LaunchURL(TEXT("https://pdocor.pico-interactive.com/reference/unreal/xr/12832/240774/"), NULL, NULL);
}

void FPICOXREditorModule::LaunchSampleDemoURL()
{
	FPlatformProcess::LaunchURL(TEXT("https://github.com/Pico-Developer"), NULL, NULL);
}

void FPICOXREditorModule::AddSampleInputs()
{
#if WITH_EDITOR
    // Get Existing Input Settings
	auto DefaultInputSettings = GetDefault<UInputSettings>();
	TArray<FInputAxisKeyMapping> ExistingAxisKeys = DefaultInputSettings->GetAxisMappings();
	TArray<FInputActionKeyMapping> ExistingActionKeys = DefaultInputSettings->GetActionMappings();

	// Create new Input Settings
	UInputSettings* InputSettings = GetMutableDefault<UInputSettings>();
	if (InputSettings->IsValidLowLevel())
	{
		// left
		AddNewSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonX")), FPICOTouchKey::PICOTouch_Left_X_Click);
		AddNewSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonY")), FPICOTouchKey::PICOTouch_Left_Y_Click);
		AddNewSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonLeftMenu")), FPICOTouchKey::PICOTouch_Left_Menu_Click);
		AddNewSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonLeftHome")), FPICOTouchKey::PICOTouch_Left_Home_Click);
		AddNewSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonLeftTrigger")), FPICOTouchKey::PICOTouch_Left_Trigger_Click);
		AddNewSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonLeftGrip")), FPICOTouchKey::PICOTouch_Left_Grip_Click);
		
		AddNewSampleAxisMapping(ExistingAxisKeys, InputSettings, FName(TEXT("LeftTriggerAxis")), FPICOTouchKey::PICOTouch_Left_Trigger_Axis);
		AddNewSampleAxisMapping(ExistingAxisKeys, InputSettings, FName(TEXT("LeftGripAxis")), FPICOTouchKey::PICOTouch_Left_Grip_Axis);
		AddNewSampleAxisMapping(ExistingAxisKeys, InputSettings, FName(TEXT("LeftJoyStickXAxis")), FPICOTouchKey::PICOTouch_Left_Thumbstick_X);
		AddNewSampleAxisMapping(ExistingAxisKeys, InputSettings, FName(TEXT("LeftJoyStickYAxis")), FPICOTouchKey::PICOTouch_Left_Thumbstick_Y);
		// Right
		AddNewSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonA")), FPICOTouchKey::PICOTouch_Right_A_Click);
		AddNewSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonB")), FPICOTouchKey::PICOTouch_Right_B_Click);
		AddNewSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonRightSystem")), FPICOTouchKey::PICOTouch_Right_System_Click);
		AddNewSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonRightHome")), FPICOTouchKey::PICOTouch_Right_Home_Click);
		AddNewSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonRightTrigger")), FPICOTouchKey::PICOTouch_Right_Trigger_Click);
		AddNewSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonRightGrip")), FPICOTouchKey::PICOTouch_Right_Grip_Click);
		
		AddNewSampleAxisMapping(ExistingAxisKeys, InputSettings, FName(TEXT("RightTriggerAxis")), FPICOTouchKey::PICOTouch_Right_Trigger_Axis);
		AddNewSampleAxisMapping(ExistingAxisKeys, InputSettings, FName(TEXT("RightGripAxis")), FPICOTouchKey::PICOTouch_Right_Grip_Axis);
		AddNewSampleAxisMapping(ExistingAxisKeys, InputSettings, FName(TEXT("RightJoyStickXAxis")), FPICOTouchKey::PICOTouch_Right_Thumbstick_X);
		AddNewSampleAxisMapping(ExistingAxisKeys, InputSettings, FName(TEXT("RightJoyStickYAxis")), FPICOTouchKey::PICOTouch_Right_Thumbstick_Y);
		
		// Update the config file
		InputSettings->SaveKeyMappings();
		InputSettings->UpdateDefaultConfigFile();
	}
#endif
}

void FPICOXREditorModule::ClearSampleInputs()
{
#if WITH_EDITOR
	auto DefaultInputSettings = GetDefault<UInputSettings>();
	TArray<FInputAxisKeyMapping> ExistingAxisKeys = DefaultInputSettings->GetAxisMappings();
	TArray<FInputActionKeyMapping> ExistingActionKeys = DefaultInputSettings->GetActionMappings();

	// Create new Input Settings
	UInputSettings* InputSettings = GetMutableDefault<UInputSettings>();
	if (InputSettings->IsValidLowLevel())
	{
		// // left
		RemoveSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonX")), FPICOTouchKey::PICOTouch_Left_X_Click);
		RemoveSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonY")), FPICOTouchKey::PICOTouch_Left_Y_Click);
		RemoveSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonLeftMenu")), FPICOTouchKey::PICOTouch_Left_Menu_Click);
		RemoveSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonLeftHome")), FPICOTouchKey::PICOTouch_Left_Home_Click);
		RemoveSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonLeftTrigger")), FPICOTouchKey::PICOTouch_Left_Trigger_Click);
		RemoveSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonLeftGrip")), FPICOTouchKey::PICOTouch_Left_Grip_Click);
		
		RemoveSampleAxisMapping(ExistingAxisKeys, InputSettings, FName(TEXT("LeftTriggerAxis")), FPICOTouchKey::PICOTouch_Left_Trigger_Axis);
		RemoveSampleAxisMapping(ExistingAxisKeys, InputSettings, FName(TEXT("LeftGripAxis")), FPICOTouchKey::PICOTouch_Left_Grip_Axis);
		RemoveSampleAxisMapping(ExistingAxisKeys, InputSettings, FName(TEXT("LeftJoyStickXAxis")), FPICOTouchKey::PICOTouch_Left_Thumbstick_X);
		RemoveSampleAxisMapping(ExistingAxisKeys, InputSettings, FName(TEXT("LeftJoyStickYAxis")), FPICOTouchKey::PICOTouch_Left_Thumbstick_Y);
		// Right
		RemoveSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonA")), FPICOTouchKey::PICOTouch_Right_A_Click);
		RemoveSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonB")), FPICOTouchKey::PICOTouch_Right_B_Click);
		RemoveSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonRightSystem")), FPICOTouchKey::PICOTouch_Right_System_Click);
		RemoveSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonRightHome")), FPICOTouchKey::PICOTouch_Right_Home_Click);
		RemoveSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonRightTrigger")), FPICOTouchKey::PICOTouch_Right_Trigger_Click);
		RemoveSampleActionMapping(ExistingActionKeys, InputSettings, FName(TEXT("ButtonRightGrip")), FPICOTouchKey::PICOTouch_Right_Grip_Click);
		
		RemoveSampleAxisMapping(ExistingAxisKeys, InputSettings, FName(TEXT("RightTriggerAxis")), FPICOTouchKey::PICOTouch_Right_Trigger_Axis);
		RemoveSampleAxisMapping(ExistingAxisKeys, InputSettings, FName(TEXT("RightGripAxis")), FPICOTouchKey::PICOTouch_Right_Grip_Axis);
		RemoveSampleAxisMapping(ExistingAxisKeys, InputSettings, FName(TEXT("RightJoyStickXAxis")), FPICOTouchKey::PICOTouch_Right_Thumbstick_X);
		RemoveSampleAxisMapping(ExistingAxisKeys, InputSettings, FName(TEXT("RightJoyStickYAxis")), FPICOTouchKey::PICOTouch_Right_Thumbstick_Y);
		// Update the config file
		InputSettings->SaveKeyMappings();
		InputSettings->UpdateDefaultConfigFile();
	}
#endif
}

bool FPICOXREditorModule::AddNewSampleAxisMapping(TArray<FInputAxisKeyMapping> ExistingAxisKeys, UInputSettings* InputSettings, FName ActionName, FKey ActionKey)
{
	// Create new axis mapping
	FInputAxisKeyMapping NewAxisMapping = FInputAxisKeyMapping(ActionName, ActionKey);

	// Check if this mapping already exists in the project
	if (!ExistingAxisKeys.Contains(NewAxisMapping))
	{
		// If none, create a new one
		InputSettings->AddAxisMapping(NewAxisMapping);
		return true;
	}
	
	return false;
}

bool FPICOXREditorModule::AddNewSampleActionMapping(TArray<FInputActionKeyMapping> ExistingActionKeys, UInputSettings* InputSettings, FName ActionName, FKey ActionKey)
{
	// Create new action mapping
	FInputActionKeyMapping NewActionMapping = FInputActionKeyMapping(ActionName, ActionKey);

	// Check if this mapping already exists in the project
	if (!ExistingActionKeys.Contains(NewActionMapping))
	{
		// If none, create a new one
		InputSettings->AddActionMapping(NewActionMapping);
		return true;
	}

	return false;
}

bool FPICOXREditorModule::RemoveSampleAxisMapping(TArray<FInputAxisKeyMapping> ExistingAxisKeys, UInputSettings* InputSettings, FName ActionName, FKey ActionKey)
{
	// Create new axis mapping
	FInputAxisKeyMapping NewAxisMapping = FInputAxisKeyMapping(ActionName, ActionKey);

	// Check if this mapping already exists in the project
	if (ExistingAxisKeys.Contains(NewAxisMapping))
	{
		// If none, create a new one
		InputSettings->RemoveAxisMapping(NewAxisMapping);
		return true;
	}
	
	return false;
}

bool FPICOXREditorModule::RemoveSampleActionMapping(TArray<FInputActionKeyMapping> ExistingActionKeys, UInputSettings* InputSettings, FName ActionName, FKey ActionKey)
{
	// Create new action mapping
	FInputActionKeyMapping NewActionMapping = FInputActionKeyMapping(ActionName, ActionKey);

	// Check if this mapping already exists in the project
	if (ExistingActionKeys.Contains(NewActionMapping))
	{
		// If none, create a new one
		InputSettings->RemoveActionMapping(NewActionMapping);
		return true;
	}

	return false;
}

bool FPICOXREditorModule::AddUniqueAxisMapping(TArray<FInputAxisKeyMapping> ExistingAxisKeys, UInputSettings* InputSettings, FName ActionName, FKey ActionKey)
{
	// Create new axis mapping
	FInputAxisKeyMapping NewAxisMapping = FInputAxisKeyMapping(ActionName, ActionKey);

	// Check if this mapping already exists in the project
	if (ExistingAxisKeys.Find(NewAxisMapping) < 1)
	{
		// If none, create a new one
		InputSettings->AddAxisMapping(NewAxisMapping);
		return true;
	}
	
return false;
}

bool FPICOXREditorModule::AddUniqueActionMapping(TArray<FInputActionKeyMapping> ExistingActionKeys, UInputSettings* InputSettings, FName ActionName, FKey ActionKey)
{
	// Create new action mapping
	FInputActionKeyMapping NewActionMapping = FInputActionKeyMapping(ActionName, ActionKey);

	// Check if this mapping already exists in the project
	if (ExistingActionKeys.Find(NewActionMapping) < 1)
	{
		// If none, create a new one
		InputSettings->AddActionMapping(NewActionMapping);
		return true;
	}

	return false;
}

TSharedRef<SWidget> FPICOXREditorModule::FillComboButton(TSharedPtr<class FUICommandList> Commands)
{
	FMenuBuilder MenuBuilder(true, Commands);
	MenuBuilder.AddMenuEntry(FPICOXREditorCommands::Get().LaunchSampleDemoURL, NAME_None, TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FPICOXREditorStyle::GetStyleSetName(), "PICOXREditor.LaunchBindingsURL"));
	MenuBuilder.AddMenuEntry(FPICOXREditorCommands::Get().LaunchAPIReferenceURL, NAME_None, TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FPICOXREditorStyle::GetStyleSetName(), "PICOXREditor.LaunchBindingsURL"));
	MenuBuilder.AddMenuEntry(FPICOXREditorCommands::Get().AddSampleInputs, NAME_None, TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FPICOXREditorStyle::GetStyleSetName(), "PICOXREditor.AddSampleInputs"));
	MenuBuilder.AddMenuEntry(FPICOXREditorCommands::Get().ClearSampleInputs, NAME_None, TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FPICOXREditorStyle::GetStyleSetName(), "PICOXREditor.ClearSampleInputs"));

	return MenuBuilder.MakeWidget();
}

void FPICOXREditorModule::RegisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Editor", "PICOXR Settings",
			LOCTEXT("PICOXREditorSettingsName", "PICOXR Settings"),
			LOCTEXT("PICOXREditorSettingsDescription", "Configure the PICOXR plugin"),
			GetMutableDefault<UPICOXREditorSettings>()
		);

		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		//PropertyModule.RegisterCustomClassLayout(UPICOXREditorSettings::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FPICOXRSettingsDetailsCustomization::MakeInstance));
	}
}

void FPICOXREditorModule::UnregisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "PICOXR");
	}
}

	
IMPLEMENT_MODULE(FPICOXREditorModule, PICOXREditor)

#undef LOCTEXT_NAMESPACE