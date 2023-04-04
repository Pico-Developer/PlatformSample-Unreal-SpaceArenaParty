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
#include "OnlineSubsystemPico.h"
#include "OnlineSubsystemPicoPackage.h"
#include "OnlineSubsystemPicoNames.h"

/// @file PicoApplicationInterface.h

/** @addtogroup Function Function
 *  This is the Function group
 *  @{
 */

 /** @defgroup Application Application
  *  This is the Application group
  *  @{
  */

DECLARE_LOG_CATEGORY_EXTERN(ApplicationInterface, Log, All);

// Request
DECLARE_DELEGATE_ThreeParams(FOnLaunchOtherAppComplete, const FString& /*String Message*/, bool /*IsSuccessed*/, const FString& /*Error Message*/);
DECLARE_DELEGATE_ThreeParams(FOnLaunchOtherAppByPresenceComplete, const FString& /*String Message*/, bool /*IsSuccessed*/, const FString& /*Error Message*/);
DECLARE_DELEGATE_SixParams(FOnGetVersionComplete, int64 /*CurrentCode*/, const FString& /*CurrentName*/, int64 /*LatestCode*/, const FString& /*LatestName*/, bool /*IsSuccessed*/, const FString& /*Error Message*/);
DECLARE_DELEGATE_ThreeParams(FOnLaunchOtherAppByAppIdComplete, const FString& /*String Message*/, bool /*IsSuccessed*/, const FString& /*Error Message*/);
DECLARE_DELEGATE_ThreeParams(FOnLaunchStoreComplete, const FString& /*String Message*/, bool /*IsSuccessed*/, const FString& /*Error Message*/);

/// <summary>Pico Application interface class.</summary>
class ONLINESUBSYSTEMPICO_API FPicoApplicationInterface
{
private:

    FOnlineSubsystemPico& PicoSubsystem;

public:
    FPicoApplicationInterface(FOnlineSubsystemPico& InSubsystem);
    ~FPicoApplicationInterface();

    /// <summary>
    /// Launches a different app in a user's library.
    /// @note If the user does not have that app installed, the user will be directed to that app's page on the PICO Store.
    /// </summary>
    /// <param name ="PackageName">The package name of the app to launch.</param> 
    /// <param name ="Message">A message to be passed to the launched app.</param> 
    /// <param name ="Delegate">Will be executed when the request has been complete.  
    /// Delegate will contain the requested object class (const FString& /*String Message*/, bool /*IsSuccessed*/, const FString& /*Error Message*/).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
    bool LaunchOtherApp(const FString& PackageName, const FString& Message, const FOnLaunchOtherAppComplete& Delegate = FOnLaunchOtherAppComplete());
    void OnQueryLaunchOtherAppComplete(ppfMessageHandle Message, bool bIsError, const FOnLaunchOtherAppComplete& Delegate);

    /// <summary>
    /// Gets the current app's latest version in the PICO Store.
    /// </summary>
    /// <param name ="Delegate">Will be executed when the request has been complete.  
    /// Delegate will contain the requested object class (int64 /*CurrentCode*/, const FString& /*CurrentName*/, int64 /*LatestCode*/, const FString& /*LatestName*/, bool /*IsSuccessed*/, const FString& /*Error Message*/).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
    bool GetVersion(const FOnGetVersionComplete& Delegate = FOnGetVersionComplete());
    void OnQueryGetVersionComplete(ppfMessageHandle Message, bool bIsError, const FOnGetVersionComplete& Delegate);

    /// <summary>
    /// Launches a different app in a user's library.
    /// @note If the user does not have that app installed, the user will be directed to that app's page on the PICO Store.
    /// </summary>
    /// <param name ="AppID">The ID of the app to launch.</param> 
    /// <param name ="PackageName">The package name of the app to launch.</param> 
    /// <param name ="Message">A message to be passed to the launched app.</param> 
    /// <param name ="ApiName">The API name of the destination in the app.</param> 
    /// <param name ="LobbySessionId">The lobby session ID of the user's presence, which identifies a user group or team. Users with the same lobby session ID can play together or form a team in a game.</param> 
    /// <param name ="MatchSessionId">The match session ID of the user's presence, which identifies all users within the same destination, such as maps and levels. Users with different lobby session IDs will have the same match session ID when playing the same match.</param> 
    /// <param name ="TrackId">The tracking ID of the app launch event.</param> 
    /// <param name ="Extra">Extra data defined by the developer.</param> 
    /// <param name ="Delegate">Will be executed when the request has been completed.  
    /// Delegate will contain the requested object class (const FString& /*String Message*/, bool /*IsSuccessed*/, const FString& /*Error Message*/).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
    bool LaunchOtherAppByPresence(const FString& AppID, const FString& PackageName, const FString& Message, const FString& ApiName, const FString& LobbySessionId, const FString& MatchSessionId, const FString& TrackId, const FString& Extra, const FOnLaunchOtherAppByPresenceComplete& Delegate = FOnLaunchOtherAppByPresenceComplete());
    void OnQueryLaunchOtherAppByPresenceComplete(ppfMessageHandle Message, bool bIsError, const FOnLaunchOtherAppByPresenceComplete& Delegate);

    /// <summary>
    /// Launches a different app in a user's library by app ID.
    /// @note If the user does not have that app installed, the user will be directed to that app's page on the PICO Store.
    /// </summary>
    /// <param name ="AppId">The ID of the app to launch.</param> 
    /// <param name ="Message">A message to be passed to the launched app.</param> 
    /// <param name ="Delegate">Will be executed when the request has been complete.  
    /// Delegate will contain the requested object class (const FString& /*String Message*/, bool /*IsSuccessed*/, const FString& /*Error Message*/).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
    bool LaunchOtherAppByAppId(const FString& AppId, const FString& Message, const FOnLaunchOtherAppByAppIdComplete& Delegate = FOnLaunchOtherAppByAppIdComplete());
    void OnQueryLaunchOtherAppByAppIdComplete(ppfMessageHandle Message, bool bIsError, const FOnLaunchOtherAppByAppIdComplete& Delegate);

    /// <summary>
    /// Launches the PICO Store and jump to the details page of the current app.
    /// </summary>
    /// <param name ="Delegate">Will be executed when the request has been complete.  
    /// Delegate will contain the requested object class (const FString& /*String Message*/, bool /*IsSuccessed*/, const FString& /*Error Message*/).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
    bool LaunchStore(const FOnLaunchStoreComplete& Delegate = FOnLaunchStoreComplete());
    void OnQueryLaunchStoreComplete(ppfMessageHandle Message, bool bIsError, const FOnLaunchStoreComplete& Delegate);
};
/** @} */ // end of Application
/** @} */ // end of Function