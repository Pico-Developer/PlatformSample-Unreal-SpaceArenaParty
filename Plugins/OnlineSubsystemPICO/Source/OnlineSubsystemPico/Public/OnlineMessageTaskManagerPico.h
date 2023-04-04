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
#include "PPF_Platform.h"
#include "OnlineAsyncTaskManager.h"
#include "OnlineSubsystemPicoNames.h"

DECLARE_DELEGATE_TwoParams(FPicoMessageOnCompleteDelegate, ppfMessageHandle, bool /*bIsTimeOut or bIsError*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FPicoMulticastMessageOnCompleteDelegate, ppfMessageHandle, bool);


class FOnlineAsyncTaskPico : public FOnlineAsyncTaskBasic<class FOnlineSubsystemPico>
{

PACKAGE_SCOPE:

    ppfRequest RequestId;
    FPicoMessageOnCompleteDelegate Delegate;


private:
    ppfMessageHandle MessageHandle = nullptr;
    bool bIsError;

public:
    FOnlineAsyncTaskPico(class FOnlineSubsystemPico* InPicoSubsystem, ppfRequest InRequestId, FPicoMessageOnCompleteDelegate InDelegate) :
        FOnlineAsyncTaskBasic(InPicoSubsystem),
        RequestId(InRequestId),
        Delegate(InDelegate)
    {
    }

    virtual ~FOnlineAsyncTaskPico()
    {
    }
    /**
     *	Get a human readable description of task
     */
    virtual FString ToString() const override;

    /**
     * Give the async task time to do its work
     * Can only be called on the async task manager thread
     */
    virtual void Tick() override;

    /**
     * Give the async task a chance to marshal its data back to the game thread
     * Can only be called on the game thread by the async task manager
     */
    virtual void Finalize() override;

    /**
     *	Async task is given a chance to trigger it's delegates
     */
    virtual void TriggerDelegates() override;

    /**
     * Check the state of the async task
     * @return true if complete, false otherwise
     */
    virtual bool IsDone() const override
    {
        return bIsComplete;
    }

    /**
     * Check the success of the async task
     * @return true if successful, false otherwise
     */
    virtual bool WasSuccessful() const override
    {
        return bWasSuccessful;
    }


    void TaskReceiveMessage(ppfMessageHandle InMessageHandle, bool InbIsError);
};

class FOnlineAsyncEventPico : public FOnlineAsyncEvent<FOnlineSubsystemPico>
{
private:

    ppfMessageHandle MessageHandle;
    bool bIsError;
    FPicoMulticastMessageOnCompleteDelegate Delegate;

public:

    FOnlineAsyncEventPico(class FOnlineSubsystemPico* InSubsystem, ppfMessageHandle InMessageHandle, bool InbIsError, FPicoMulticastMessageOnCompleteDelegate InDelegate) :
        FOnlineAsyncEvent(InSubsystem),
        MessageHandle(InMessageHandle),
        bIsError(InbIsError),
        Delegate(InDelegate)
    {
    }

    virtual ~FOnlineAsyncEventPico()
    {
    }

    /**
     *	Get a human readable description of task
     */
    virtual FString ToString() const override;


    /**
     * Give the async task a chance to marshal its data back to the game thread
     * Can only be called on the game thread by the async task manager
     */
    virtual void Finalize() override
    {
    }

    virtual void TriggerDelegates() override
    {
        if (MessageHandle)
        {
            Delegate.Broadcast(MessageHandle, bIsError);
            ppf_FreeMessage(MessageHandle);
        }
    }
};

class FOnlineAsyncTaskManagerPico : public FOnlineAsyncTaskManager
{

private:
    TMap<ppfMessageType, FPicoMulticastMessageOnCompleteDelegate> NotificationMap;

    TMap<uint64, FOnlineAsyncTaskPico*> RequestTaskMap;

protected:

    /** Cached reference to the main online subsystem */
    class FOnlineSubsystemPico* PicoSubsystem;
public:
    FOnlineAsyncTaskManagerPico(class FOnlineSubsystemPico* InOnlineSubsystem) :
        PicoSubsystem(InOnlineSubsystem)
    {
    }

    ~FOnlineAsyncTaskManagerPico()
    {
    }

    // FOnlineAsyncTaskManager
    virtual void OnlineTick() override;

    void TickTask();

    void CollectedRequestTask(ppfRequest Request, FOnlineAsyncTaskPico* InTask);

    FPicoMulticastMessageOnCompleteDelegate& GetOrAddNotifyDelegate(ppfMessageType MessageType);

    void RemoveNotifyDelegate(ppfMessageType MessageType, const FDelegateHandle& Delegate);
};

