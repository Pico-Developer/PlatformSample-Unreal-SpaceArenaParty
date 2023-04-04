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
#include "UObject/NoExportTypes.h"
#include "PPF_Platform.h"
#include "OnlineSubsystemPico.h"
#include "Pico_Networking.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(PicoNetworking, Log, All);
/**
 * 
 */

/** @addtogroup Function Function
 *  This is the Function group
 *  @{
 */

 /** @defgroup Networking Networking
  *  This is the Networking group
  *  @{
  */

/// @brief PicoNetworkingInterface class.
class ONLINESUBSYSTEMPICO_API FPicoNetworkingInterface
{
private:

    FOnlineSubsystemPico& PicoSubsystem;

public:
    FPicoNetworkingInterface(FOnlineSubsystemPico& InSubsystem);
    ~FPicoNetworkingInterface();


public:

    /// <summary>
    /// Sends messages to a specified user. The maximum messaging frequency is 1000/s.
    /// </summary>
    /// <param name="UserId">The ID of the user to send messages to.</param>
    /// <param name="BinaryArray">The message length (in bytes). The maximum bytes allowed is 512.</param>
    /// <returns>
    /// * `true`: success
    /// * `false`: failure
    /// </returns>
    bool SendPacket(const FString& UserId, TArray<uint8> BinaryArray);

    /// <summary>
    /// Sends messages to a specified user. The maximum messaging frequency is 1000/s.
    /// </summary>
    /// <param name="UserId">The ID of the user to send messages to.</param>
    /// <param name="BinaryArray">The message length (in bytes). The maximum bytes allowed is 512.</param>
    /// <param name="bReliable">When `reliable` is set to `true`, messages between lost and resume will not be lost.
    /// The retention time is determined by the `reserve_period` parameter configured for the matchmaking pool, with a maximum of 1 minute.
    /// When `reliable` is set to `false`, this function works the same as the other `SendPacket` function.</param>
    /// <returns>
    /// * `true`: success
    /// * `false`: failure
    /// </returns>
    bool SendPacket(const FString& UserId, TArray<uint8> BinaryArray, bool bReliable);

    /// <summary>
    /// Sends messages to other users in the room. The maximum messaging frequency is 1000/s.
    /// </summary>
    /// <param name="BinaryArray">The message length (in bytes). The maximum bytes allowed is 512.</param>
    /// <returns>
    /// * `true`: success
    /// * `false`: failure
    /// </returns>
    bool SendPacketToCurrentRoom(TArray<uint8> BinaryArray);

    /// <summary>
    /// Sends messages to other users in the room. The maximum messaging frequency is 1000/s.
    /// </summary>
    /// <param name="BinaryArray">The message length (in bytes). The maximum bytes allowed is 512.</param>
    /// <param name="bReliable">When `reliable` is set to `true`, messages between lost and resume will not be lost.
    /// The retention time is determined by the `reserve_period` parameter configured for the matchmaking pool, with a maximum of 1 minute.
    /// When `reliable` is set to `false`, this function works the same as the other `SendPacketToCurrentRoom` function.</param>
    /// <returns>
    /// * `true`: success
    /// * `false`: failure
    /// </returns>
    bool SendPacketToCurrentRoom(TArray<uint8> BinaryArray, bool bReliable);

    /// <summary>
    /// Reads the messages from other users in the room.
    /// </summary>
    /// <param name ="OutBinaries">Returns an array of byte.</param>   
    /// <param name ="OutSendId">Returns user id which player send.</param>   
    /// <returns>
    /// * `true`: success
    /// * `false`: failure
    /// </returns>
    bool ReadPacket(TArray<uint8>& OutBinaries, FString& OutSendId);

};

/** @} */
/** @} */


/** @addtogroup BlueprintFunction BlueprintFunction
 *  This is the BlueprintFunction group
 *  @{
 */

 /** @defgroup BP_Networking BP_Networking 
  *  This is the BP_Networking group
  *  @{
  */

/// @brief OnlinePicoNetworking Blueprint Function class.
UCLASS()
class ONLINESUBSYSTEMPICO_API UOnlinePicoNetworkingFunction : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:


    /// <summary>
    /// Sends messages to a specified user. The maximum messaging frequency is 1000/s.
    /// </summary>
    /// <param name ="WorldContextObject">Used to get the information about the current world.</param>
    /// <param name="UserId">The ID of the user to send messages to.</param>
    /// <param name="BinaryArray">The message length (in bytes). The maximum bytes allowed is 512.</param>
    /// <param name="bReliable">When `reliable` is set to `true`, messages between lost and resume will not be lost.
    /// The retention time is determined by the `reserve_period` parameter configured for the matchmaking pool, with a maximum of 1 minute.
    /// When `reliable` is set to `false`, this function works the same as the other `SendPacket` function.</param>
    /// <returns>
    /// * `true`: success
    /// * `false`: failure
    /// </returns>
    UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|Networking")
    static bool SendPacket(UObject* WorldContextObject, const FString& UserId, TArray<uint8> BinaryArray, bool bReliable);

    /// <summary>
    /// Sends messages to other users in the room. The maximum messaging frequency is 1000/s.
    /// </summary>
    /// <param name ="WorldContextObject">Used to get the information about the current world.</param>
    /// <param name="BinaryArray">The message length (in bytes). The maximum bytes allowed is 512.</param>
    /// <param name="bReliable">When `reliable` is set to `true`, messages between lost and resume will not be lost.
    /// The retention time is determined by the `reserve_period` parameter configured for the matchmaking pool, with a maximum of 1 minute.
    /// When `reliable` is set to `false`, this function works the same as the other `SendPacketToCurrentRoom` function.</param>
    /// <returns>
    /// * `true`: success
    /// * `false`: failure
    /// </returns>
    UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|Networking")
    static bool SendPacketToCurrentRoom(UObject* WorldContextObject, TArray<uint8> BinaryArray, bool bReliable);

    /// <summary>
    /// Reads the messages from other users in the room.
    /// </summary>
    /// <param name ="WorldContextObject">Used to get the information about the current world.</param>
    /// <param name ="OutBinaries">Returns an array of byte.</param>   
    /// <param name ="OutSendId">Returns user id which player send.</param>   
    /// <returns>
    /// * `true`: success
    /// * `false`: failure
    /// </returns>
    UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|Networking")
    static bool ReadPacket(UObject* WorldContextObject, TArray<uint8>& OutBinaries, FString& SendUserID);
    
};

/** @} */
/** @} */