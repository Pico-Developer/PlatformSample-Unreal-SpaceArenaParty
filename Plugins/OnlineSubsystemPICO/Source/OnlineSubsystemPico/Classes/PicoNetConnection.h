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

#include "IpConnection.h"
#include "PPF_Platform.h"
#include "PicoNetConnection.generated.h"

/**
 *
 */
UCLASS(transient, config = Engine)
class UPicoNetConnection : public UIpConnection
{
	GENERATED_BODY()

private:
	ppfID PeerID;
	FString UserID;
	/** Should this net connection behave as a passthrough to normal IP */
	bool bIsPassThrough;

public:

	// Begin NetConnection Interface
	virtual void InitBase(UNetDriver* InDriver, class FSocket* InSocket, const FURL& InURL, EConnectionState InState, int32 InMaxPacket = 0, int32 InPacketOverhead = 0) override;
	virtual void InitRemoteConnection(UNetDriver* InDriver, class FSocket* InSocket, const FURL& InURL, const class FInternetAddr& InRemoteAddr, EConnectionState InState, int32 InMaxPacket = 0, int32 InPacketOverhead = 0) override;
	virtual void InitLocalConnection(UNetDriver* InDriver, class FSocket* InSocket, const FURL& InURL, EConnectionState InState, int32 InMaxPacket = 0, int32 InPacketOverhead = 0) override;
	virtual void LowLevelSend(void* Data, int32 CountBits, FOutPacketTraits& Traits) override;
	FString LowLevelGetRemoteAddress(bool bAppendPort = false) override;
	FString LowLevelDescribe() override;
	virtual void FinishDestroy() override;

	virtual FString RemoteAddressToString() override;
	// End NetConnection Interface

};
