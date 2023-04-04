/*******************************************************************************
Copyright © 2015-2022 PICO Technology Co., Ltd.All rights reserved.

NOTICE：All information contained herein is, and remains the property of
PICO Technology Co., Ltd. The intellectual and technical concepts
contained herein are proprietary to PICO Technology Co., Ltd. and may be
covered by patents, patents in process, and are protected by trade secret or
copyright law. Dissemination of this information or reproduction of this
material is strictly forbidden unless prior written permission is obtained from
PICO Technology Co., Ltd.
*******************************************************************************/
// This plugin incorporates portions of the Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc.In the United States of America and elsewhere.
// Unreal® Engine, Copyright 1998 – 2022, Epic Games, Inc.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
/**
 * Online subsystem module class  (Pico Implementation)
 * Code related to the loading of the Oculus module
 */


class FOnlineSubsystemPicoModule : public IModuleInterface
{
private:

    /** Class responsible for creating instance(s) of the subsystem */
    class FOnlineFactoryPico* PicoFactory;

public:

    FOnlineSubsystemPicoModule() :
        PicoFactory(nullptr)
    {}

    virtual ~FOnlineSubsystemPicoModule() {}

    // IModuleInterface

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    void RegisterSettings();
    void UnregisterSettings();

    virtual bool SupportsDynamicReloading() override
    {
        return false;
    }

    virtual bool SupportsAutomaticShutdown() override
    {
        return false;
    }

private:
    /** Handle to the test dll we will load */
    void* WindowsLibraryHandle;
    void* LibgccHandel;
    void* libstdcHandel;
    void* LibwinpthreadHandel;
};