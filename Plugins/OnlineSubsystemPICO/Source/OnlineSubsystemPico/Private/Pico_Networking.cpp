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


#include "Pico_Networking.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemPico.h"

DEFINE_LOG_CATEGORY(PicoNetworking);
FPicoNetworkingInterface::FPicoNetworkingInterface(FOnlineSubsystemPico& InSubsystem) :
    PicoSubsystem(InSubsystem)
{

}

FPicoNetworkingInterface::~FPicoNetworkingInterface()
{

}


bool FPicoNetworkingInterface::SendPacket(const FString& UserId, TArray<uint8> BinaryArray)
{
    UE_LOG(PicoNetworking, Log, TEXT("FPicoNetworkingInterface::SendPacket"));
    return ppf_Net_SendPacket(TCHAR_TO_UTF8(*UserId), BinaryArray.Num(), BinaryArray.GetData());
}

bool FPicoNetworkingInterface::SendPacket(const FString& UserId, TArray<uint8> BinaryArray, bool bReliable)
{
    UE_LOG(PicoNetworking, Log, TEXT("FPicoNetworkingInterface::SendPacketReliable"));
    return ppf_Net_SendPacket2(TCHAR_TO_UTF8(*UserId), BinaryArray.Num(), BinaryArray.GetData(), bReliable);
}

bool FPicoNetworkingInterface::SendPacketToCurrentRoom(TArray<uint8> BinaryArray)
{
    UE_LOG(PicoNetworking, Log, TEXT("FPicoNetworkingInterface::SendPacketToCurrentRoom"));
    return ppf_Net_SendPacketToCurrentRoom(BinaryArray.Num(), BinaryArray.GetData());
}

bool FPicoNetworkingInterface::SendPacketToCurrentRoom(TArray<uint8> BinaryArray, bool bReliable)
{
    UE_LOG(PicoNetworking, Log, TEXT("FPicoNetworkingInterface::SendPacketToCurrentRoomReliable"));
    return ppf_Net_SendPacketToCurrentRoom2(BinaryArray.Num(), BinaryArray.GetData(), bReliable);
}

bool FPicoNetworkingInterface::ReadPacket(TArray<uint8>& OutBinaries, FString& OutSendId)
{
    UE_LOG(PicoNetworking, Log, TEXT("FPicoNetworkingInterface::ReadPacket"));
    auto Packet = ppf_Net_ReadPacket();
    if (Packet)
    {
        OutBinaries.Empty();

        int32 Num = ppf_Packet_GetSize(Packet);
        const uint8* Datas = (const uint8*)ppf_Packet_GetBytes(Packet);
        OutBinaries.Append(Datas, Num);
        OutSendId = UTF8_TO_TCHAR(ppf_Packet_GetSenderID(Packet));
        ppf_Packet_Free(Packet);
        return true;
    }
    return false;
}

bool UOnlinePicoNetworkingFunction::SendPacket(UObject* WorldContextObject, const FString& UserId, TArray<uint8> BinaryArray, bool bReliable)
{
    UE_LOG(PicoNetworking, Log, TEXT("UOnlinePicoNetworkingFunction::SendPacket"));
    FOnlineSubsystemPico* Subsystem = static_cast<FOnlineSubsystemPico*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), PICO_SUBSYSTEM));
    if (Subsystem && Subsystem->GetPicoNetworkingInterface())
    {
        return Subsystem->GetPicoNetworkingInterface()->SendPacket(UserId, BinaryArray, bReliable);
    }
    return false;
}

bool UOnlinePicoNetworkingFunction::SendPacketToCurrentRoom(UObject* WorldContextObject, TArray<uint8> BinaryArray, bool bReliable)
{
    UE_LOG(PicoNetworking, Log, TEXT("UOnlinePicoNetworkingFunction::SendPacketToCurrentRoom"));
    FOnlineSubsystemPico* Subsystem = static_cast<FOnlineSubsystemPico*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), PICO_SUBSYSTEM));
    if (Subsystem && Subsystem->GetPicoNetworkingInterface())
    {
        return Subsystem->GetPicoNetworkingInterface()->SendPacketToCurrentRoom(BinaryArray, bReliable);
    }
    return false;
}

bool UOnlinePicoNetworkingFunction::ReadPacket(UObject* WorldContextObject, TArray<uint8>& OutBinaries, FString& SendUserID)
{
    UE_LOG(PicoNetworking, Log, TEXT("UOnlinePicoNetworkingFunction::ReadPacket"));
    FOnlineSubsystemPico* Subsystem = static_cast<FOnlineSubsystemPico*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), PICO_SUBSYSTEM));
    if (Subsystem && Subsystem->GetPicoNetworkingInterface())
    {
        return Subsystem->GetPicoNetworkingInterface()->ReadPacket(OutBinaries, SendUserID);
    }
    return false;
}
