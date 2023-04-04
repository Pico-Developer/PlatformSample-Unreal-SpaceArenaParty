#include "PICOXREditorCommands.h"

#define LOCTEXT_NAMESPACE "FPICOXREditorModule"
/* UI_COMMAND takes long for the compile to optimize */
PRAGMA_DISABLE_OPTIMIZATION
void FPICOXREditorCommands::RegisterCommands()
{
 	UI_COMMAND(PluginAction, "PICOXREditor", "Execute PICOXREditor action", EUserInterfaceActionType::Button, FInputChord());
 	UI_COMMAND(LaunchAPIReferenceURL, "Launch PICOXR API Reference", "Launch PICOXR API Reference", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(LaunchSampleDemoURL, "Launch PICOXR Sample", "Launch Sample", EUserInterfaceActionType::Button, FInputChord());
 	UI_COMMAND(AddSampleInputs, "Add Sample Inputs", "Add Sample Inputs", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ClearSampleInputs, "Clear Sample Inputs", "Clear Sample Inputs", EUserInterfaceActionType::Button, FInputChord());

}
PRAGMA_ENABLE_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
