#pragma once

#include "Modules/ModuleManager.h"
#include "IDetailCustomization.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerInput.h"
#include "Input/Reply.h"

class FToolBarBuilder;
class FMenuBuilder;
class SWidget;

class FPICOXREditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** These functions will be bound to Commands */
	void PluginButtonClicked();
	;
	void LaunchAPIReferenceURL();
	void LaunchSampleDemoURL();

	void AddSampleInputs();
	void ClearSampleInputs();
	void RegisterSettings();
	void UnregisterSettings();
private:
	bool AddNewSampleAxisMapping(TArray<FInputAxisKeyMapping> ExistingAxisKeys, UInputSettings* InputSettings, FName ActionName, FKey ActionKey);
	bool AddNewSampleActionMapping(TArray<FInputActionKeyMapping> ExistingActionKeys, UInputSettings* InputSettings, FName ActionName, FKey ActionKey);
	bool RemoveSampleAxisMapping(TArray<FInputAxisKeyMapping> ExistingAxisKeys, UInputSettings* InputSettings, FName ActionName, FKey ActionKey);
	bool RemoveSampleActionMapping(TArray<FInputActionKeyMapping> ExistingActionKeys, UInputSettings* InputSettings, FName ActionName, FKey ActionKey);

	bool AddUniqueAxisMapping(TArray<FInputAxisKeyMapping> ExistingAxisKeys, UInputSettings* InputSettings, FName ActionName, FKey ActionKey);
	bool AddUniqueActionMapping(TArray<FInputActionKeyMapping> ExistingActionKeys, UInputSettings* InputSettings, FName ActionName, FKey ActionKey);
	bool ShowPICOXRInputToolbarDropdown();

	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedRef<SWidget> FillComboButton(TSharedPtr<class FUICommandList> Commands);

};