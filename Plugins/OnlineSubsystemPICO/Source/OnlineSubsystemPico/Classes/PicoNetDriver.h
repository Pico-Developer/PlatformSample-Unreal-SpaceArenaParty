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
#include "PicoNetConnection.h"
#include "IpNetDriver.h"
#include "IPAddress.h"
#include "PicoNetDriver.generated.h"

class FOnlineSessionPico;

/**
 *
 */
UCLASS(transient, config = Engine)
class UPicoNetDriver : public UIpNetDriver
{
	GENERATED_BODY()

private:

	bool AddNewClientConnection(const FString& UserID);
	/** Should this net driver behave as a passthrough to normal IP */
	bool bIsPassthrough;

	// todo
	//TMap<uint64, EConnectionState> PendingClientConnections;
	TMap<FString, EConnectionState> PendingClientConnections;

public:
	// todo
	//TMap<uint64, UPicoNetConnection*> Connections;
	TMap<FString, UPicoNetConnection*> Connections;

	// Begin UNetDriver interface.
	virtual bool IsAvailable() const override;
	virtual bool InitBase(bool bInitAsClient, FNetworkNotify* InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error) override;
	virtual bool InitConnect(FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error) override;
	virtual bool InitListen(FNetworkNotify* InNotify, FURL& LocalURL, bool bReuseAddressAndPort, FString& Error) override;
	virtual void TickDispatch(float DeltaTime) override;
	virtual void LowLevelSend(TSharedPtr<const FInternetAddr> Address, void* Data, int32 CountBits, FOutPacketTraits& Traits) override;
	virtual void Shutdown() override;
	virtual bool IsNetResourceValid() override;

	virtual class ISocketSubsystem* GetSocketSubsystem() override;
};
