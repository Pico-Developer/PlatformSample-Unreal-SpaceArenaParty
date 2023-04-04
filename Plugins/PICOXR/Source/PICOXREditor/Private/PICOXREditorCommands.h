#pragma once

#include "PICOXREditorStyle.h"


class FPICOXREditorCommands : public TCommands<FPICOXREditorCommands>
{
public:

	FPICOXREditorCommands()
		: TCommands<FPICOXREditorCommands>(TEXT("PICOXREditor"), NSLOCTEXT("Contexts", "PICOXREditor", "PICOXREditor Plugin"), NAME_None, TEXT("PICOXREditor.Common.Icon"))
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> PluginAction;
	TSharedPtr<FUICommandInfo> LaunchAPIReferenceURL;
	TSharedPtr<FUICommandInfo> LaunchSampleDemoURL;
	TSharedPtr<FUICommandInfo> AddSampleInputs;
	TSharedPtr<FUICommandInfo> ClearSampleInputs;

};
