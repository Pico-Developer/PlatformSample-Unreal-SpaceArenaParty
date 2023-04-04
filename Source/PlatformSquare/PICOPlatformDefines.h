// Copyright
#pragma once

#if ENGINE_MINOR_VERSION >= 15
#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "EngineDefines.h"
#include "UObject/Object.h"
#include "UObject/ScriptMacros.h"
#else
#include "CoreUObject.h"
#include "Engine.h"
#endif

#include "Delegates/DelegateCombinations.h"
#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"
#include "Logging/LogVerbosity.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPICO, Log, All);

#define PICO_FUNC (FString(__FUNCTION__))             
#define PICO_LINE (FString::FromInt(__LINE__))         
#define PICO_FUNC_LINE (PICO_FUNC + "(" + PICO_LINE + ")")