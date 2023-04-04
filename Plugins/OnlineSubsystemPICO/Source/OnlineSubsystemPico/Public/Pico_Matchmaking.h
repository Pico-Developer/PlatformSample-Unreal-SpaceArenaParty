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
#include "UObject/NoExportTypes.h"
#include "OnlineSubsystemPicoNames.h"
#include "PPF_Platform.h"
#include "Pico_Matchmaking.generated.h"
//


DECLARE_LOG_CATEGORY_EXTERN(PicoMatchmaking, Log, All);

class UPico_User;
class UPico_DataStore;
class UPico_UserArray;
class UPico_Room;

class UPico_MatchmakingStats;
class UPico_MatchmakingRoomList;
class UPico_MatchmakingRoom;
class UPico_MatchmakingBrowseResult;
class UPico_MatchmakingEnqueueResultAndRoom;
class UPico_MatchmakingEnqueueResult;
class UPico_MatchmakingAdminSnapshot;
class UPico_MatchmakingAdminSnapshotCandidateList;
class UPico_MatchmakingAdminSnapshotCandidate;


/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_FourParams(FMatchmakingMatchFoundNotify, bool, /*bIsError*/ int, /*ErrorCode*/ const FString&, /*ErrorMessage*/ UPico_Room* /*Room*/);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FMatchmakingCancelNotify, bool, /*bIsError*/ int, /*ErrorCode*/ const FString& /*ErrorMessage*/);

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FMatchmakingReportResultsInsecure, bool, bIsError, int, ErrorCode, const FString&, ErrorMessage);
DECLARE_DYNAMIC_DELEGATE_FourParams(FMatchmakingGetStats, bool, bIsError, int, ErrorCode, const FString&, ErrorMessage, UPico_MatchmakingStats*, MatchmakingStats);
DECLARE_DYNAMIC_DELEGATE_FourParams(FMatchmakingBrowse2, bool, bIsError, int, ErrorCode, const FString&, ErrorMessage, UPico_MatchmakingBrowseResult*, MatchmakingBrowseResult);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FMatchmakingCancel, bool, bIsError, int, ErrorCode, const FString&, ErrorMessage);
DECLARE_DYNAMIC_DELEGATE_FourParams(FMatchmakingCreateAndEnqueueRoom2, bool, bIsError, int, ErrorCode, const FString&, ErrorMessage, UPico_MatchmakingEnqueueResultAndRoom*, MatchmakingEnqueueResultAndRoom);
DECLARE_DYNAMIC_DELEGATE_FourParams(FMatchmakingEnqueue2, bool, bIsError, int, ErrorCode, const FString&, ErrorMessage, UPico_MatchmakingEnqueueResult*, MatchmakingEnqueueResult);
DECLARE_DYNAMIC_DELEGATE_FourParams(FMatchmakingGetAdminSnapshot, bool, bIsError, int, ErrorCode, const FString&, ErrorMessage, UPico_MatchmakingAdminSnapshot*, MatchmakingAdminSnapshot);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FMatchmakingStartMatch, bool, bIsError, int, ErrorCode, const FString&, ErrorMessage);
// FRoom_LaunchInvitableUserFlow
// FRoom_GetNextRoomArrayPage

/** @addtogroup Function Function
 *  This is the Function group
 *  @{
 */

/** @defgroup Matchmaking Matchmaking
 *  This is the Matchmaking group
 *  @{
 */

/// @brief PicoMatchmakingInterface class.
class ONLINESUBSYSTEMPICO_API FPicoMatchmakingInterface
{
private:
	FOnlineSubsystemPico& PicoSubsystem;

public:
	FPicoMatchmakingInterface(FOnlineSubsystemPico& InSubsystem);
	~FPicoMatchmakingInterface();

	FDelegateHandle MatchmakingMatchFoundHandle;
	void OnMatchmakingMatchFoundNotification(ppfMessageHandle Message, bool bIsError);

	FDelegateHandle MatchmakingCancelHandle;
	void OnMatchmakingCancelNotification(ppfMessageHandle Message, bool bIsError);
	
	FMatchmakingMatchFoundNotify MatchmakingMatchFoundNotify;
	FMatchmakingCancelNotify MatchmakingCancelNotify;

	FMatchmakingReportResultsInsecure ReportResultsInsecureDelegate;
	FMatchmakingGetStats GetStatsDelegate;
	FMatchmakingBrowse2 Browse2Delegate;
	FMatchmakingCancel CancelDelegate;
	FMatchmakingCreateAndEnqueueRoom2 CreateAndEnqueueRoom2Delegate;
	FMatchmakingEnqueue2 Enqueue2Delegate;
	FMatchmakingGetAdminSnapshot GetAdminSnapshotDelegate;
	FMatchmakingStartMatch StartMatchDelegate;

	/// <summary>Reports the result of a skill-rating match.
	/// @note  Applicable to the following matchmaking modes: Quickmatch, Browse (+ Skill Pool)</summary>
	/// <param name="RoomID">The ID of the room.</param>
	/// <param name="Data">The key-value pairs.</param>
    /// <param name="InDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, int, ErrorCode, const FString&, ErrorMessage).</param>
	/// <returns>Bool:
	/// <ul>
	/// <li>`true`: success</li>
	/// <li>`false`: failure</li>
	/// </ul>
	/// </returns>  
	bool ReportResultsInsecure(const FString& RoomID, const TMap<FString, int>& Data, FMatchmakingReportResultsInsecure InDelegate);

	/// <summary>Gets the matchmaking statistics for the current user.
	/// @note  Applicable to the following matchmaking modes: Quickmatch, Browse</summary>
	/// 
	/// <param name="Pool">The pool to look in.</param>
	/// <param name="MaxLevel">(beta feature, don't use it)</param>
	/// <param name="approach">(beta feature, don't use it)</param>
    /// <param name="InDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, int, ErrorCode, const FString&, ErrorMessage, UPico_MatchmakingStats *, MatchmakingStats).</param>
	/// <returns>Bool:
	/// <ul>
	/// <li>`true`: success</li>
	/// <li>`false`: failure</li>
	/// </ul>
	/// </returns>  
	bool GetStats(const FString& Pool, uint32 MaxLevel, EMatchmakingStatApproach Approach = EMatchmakingStatApproach::Trailing, FMatchmakingGetStats InDelegate = FMatchmakingGetStats());

	/// <summary>Gets rooms by matchmakinging pool name.
	/// The user can join the room with `RoomService.Join2 to`or cancel the retrieval with `MatchmakingService.Cancel`.
	/// @note  Applicable to the following matchmaking mode: Browse</summary>
	///
	/// <param name="pool">The matchmaking pool name you want to browse.</param>
	/// <param name="MatchmakingOptions">(Optional) The matchmaking configuration of the browse request.</param>
    /// <param name="InDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, int, ErrorCode, const FString&, ErrorMessage, UPico_MatchmakingBrowseResult *, MatchmakingBrowseResult).</param>
	/// <returns>Bool:
	/// <ul>
	/// <li>`true`: success</li>
	/// <li>`false`: failure</li>
	/// </ul>
	/// </returns>  
	bool Browse2(const FString& Pool, FPicoMatchmakingOptions MatchmakingOptions, FMatchmakingBrowse2 InDelegate);

	/// <summary>Cancels a matchmaking request. Call this function
	/// to cancel an enqueue request before a match
	/// is made. This is typically triggered when a user gives up waiting.
	/// If you do not cancel the request but the user goes offline, the user/room
	/// will be timed out according to the setting of reserved period on the PICO Developer Platform.
	/// @note  Applicable to the following matchmaking modes: Quickmatch, Browse</summary>
    /// <param name="InDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, int, ErrorCode, const FString&, ErrorMessage).</param>
	/// <returns>Bool:
	/// <ul>
	/// <li>`true`: success</li>
	/// <li>`false`: failure</li>
	/// </ul>
	/// </returns>  
	bool Cancel(FMatchmakingCancel InDelegate);

	/// <summary>Creates a matchmaking room, then enqueues and joins it.
	/// @note  Applicable to the following matchmaking modes: Quickmatch, Browse, Advanced (Can Users Create Rooms=`true`)</summary>
	/// 
	/// <param name="pool">The matchmaking pool to use, which is created on the PICO Developer Platform.</param>
	/// <param name="MatchmakingOptions">(Optional) Additional matchmaking configuration for this request.</param>
    /// <param name="InDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, int, ErrorCode, const FString&, ErrorMessage, UPico_MatchmakingEnqueueResultAndRoom *, MatchmakingEnqueueResultAndRoom).</param>
	/// <returns>Bool:
	/// <ul>
	/// <li>`true`: success</li>
	/// <li>`false`: failure</li>
	/// </ul>
	/// </returns>  
	bool CreateAndEnqueueRoom2(const FString& Pool, FPicoMatchmakingOptions MatchmakingOptions, FMatchmakingCreateAndEnqueueRoom2 InDelegate);

	/// <summary>Enqueues for an available matchmaking room to join.
	/// When the server finds a match, it will return a message of
	/// type `MessageType.Notification_Matchmaking_MatchFound`. You
	/// can join found matching rooms by calling `RoomService.Join2`. 
	/// If you want to cancel the match early, you can use `MatchmakingService.Cancel`.
	/// @note  Applicable to the following matchmaking mode: Quickmatch</summary>
	/// 
	/// <param name="pool">The matchmaking pool to use, which is defined on the PICO Developer Platform.</param>
	/// <param name="MatchmakingOptions">(Optional) Match configuration for Enqueue.</param>
    /// <param name="InDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, int, ErrorCode, const FString&, ErrorMessage, UPico_MatchmakingEnqueueResult *, MatchmakingEnqueueResult).</param>
	/// <returns>Bool:
	/// <ul>
	/// <li>`true`: success</li>
	/// <li>`false`: failure</li>
	/// </ul>
	/// </returns>  
	bool Enqueue2(const FString& pool, FPicoMatchmakingOptions MatchmakingOptions, FMatchmakingEnqueue2 InDelegate);

	/// <summary>Debugs the state of the current matchmaking pool queue.
	/// @note 
	/// * This function should not be used in production.
	/// * Applicable to the following matchmaking modes: Quickmatch, Browse
	///
	/// </summary>
    /// <param name="InDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, int, ErrorCode, const FString&, ErrorMessage, UPico_MatchmakingAdminSnapshot *, MatchmakingAdminSnapshot).</param>
	/// <returns>Bool:
	/// <ul>
	/// <li>`true`: success</li>
	/// <li>`false`: failure</li>
	/// </ul>
	/// </returns>  
	bool GetAdminSnapshot(FMatchmakingGetAdminSnapshot InDelegate);

	/// <summary>Reports that a skill-rating match has started.
	/// You can use this method after joining the room.
	/// @note 
	/// * This function is only for pools with skill-based matching.
	/// * Applicable to the following matchmaking modes: Quickmatch, Browse (+ Skill Pool)
	/// </summary>
	///
	/// <param name="RoomId">The ID of the room you want to match.</param>
    /// <param name="InDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, int, ErrorCode, const FString&, ErrorMessage).</param>
	/// <returns>Bool:
	/// <ul>
	/// <li>`true`: success</li>
	/// <li>`false`: failure</li>
	/// </ul>
	/// </returns>  
	bool StartMatch(const FString& RoomId, FMatchmakingStartMatch InDelegate);
private:
	ppfMatchmakingOptions* GetppfMatchmakingOptions(FPicoMatchmakingOptions PicoMatchmakingOptions);
	ppfMatchmakingStatApproach GetppfMatchmakingStatApproach(EMatchmakingStatApproach MatchmakingStatApproach);
	ppfID GetppfID(const FString& StringID);
};

/** @} */
/** @} */


/** @addtogroup BlueprintFunction BlueprintFunction
 *  This is the BlueprintFunction group
 *  @{
 */

/** @defgroup BP_Matchmaking BP_Matchmaking
 *  This is the BP_Matchmaking group
 *  @{
 */

/// @brief OnlinePicoMatchmaking Blueprint Function class.
UCLASS()
class ONLINESUBSYSTEMPICO_API UOnlinePicoMatchmakingFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	/// <summary>Reports the result of a skill-rating match.
	/// @note  Applicable to the following matchmaking modes: Quickmatch, Browse (+ Skill Pool)</summary>
	/// <param name="RoomID">The ID of the room.</param>
	/// <param name="Data">The key-value pairs.</param>
    /// <param name="InDelegate">Will be executed when the request has been completed.  
    /// Delegate will contain the requested object class (bool, bIsError, int, ErrorCode, const FString&, ErrorMessage).</param>
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|Matchmaking")
	static void ReportResultsInsecure(UObject* WorldContextObject, const FString& RoomID, const TMap<FString, int>& Data, FMatchmakingReportResultsInsecure InDelegate);

	/// <summary>Gets the matchmaking statistics for the current user.
	/// @note  Applicable to the following matchmaking modes: Quickmatch, Browse</summary>
	/// 
	/// <param name="Pool">The pool to look in.</param>
	/// <param name="MaxLevel">(beta feature, don't use it)</param>
	/// <param name="approach">(beta feature, don't use it)</param>
    /// <param name="InDelegate">Will be executed when the request has been completed.
    /// Delegate will contain the requested object class (bool, bIsError, int, ErrorCode, const FString&, ErrorMessage, UPico_MatchmakingStats *, MatchmakingStats).</param>
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|Matchmaking")
	// static void GetStats(UObject* WorldContextObject, const FString& Pool, int32 MaxLevel, EMatchmakingStatApproach approach = EMatchmakingStatApproach::Trailing, const FMatchmakingGetStats& InDelegate = FMatchmakingGetStats());
	static void GetStats(UObject* WorldContextObject, const FString& Pool, int32 MaxLevel, EMatchmakingStatApproach approach, FMatchmakingGetStats InDelegate);

	/// <summary>Gets rooms by matchmakinging pool name.
	/// The user can join the room with `RoomService.Join2 to`or cancel the retrieval with `MatchmakingService.Cancel`.
	/// @note  Applicable to the following matchmaking mode: Browse</summary>
	///
	/// <param name="pool">The matchmaking pool name you want to browse.</param>
	/// <param name="MatchmakingOptions">(Optional) The matchmaking configuration of the browse request.</param>
    /// <param name="InDelegate">Will be executed when the request has been completed.
    /// Delegate will contain the requested object class (bool, bIsError, int, ErrorCode, const FString&, ErrorMessage, UPico_MatchmakingBrowseResult *, MatchmakingBrowseResult).</param>
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|Matchmaking")
	static void Browse2(UObject* WorldContextObject, const FString& Pool, FPicoMatchmakingOptions MatchmakingOptions, FMatchmakingBrowse2 InDelegate);

	/// <summary>Cancels a matchmaking request. Call this function
	/// to cancel an enqueue request before a match
	/// is made. This is typically triggered when a user gives up waiting.
	/// If you do not cancel the request but the user goes offline, the user/room
	/// will be timed out according to the setting of reserved period on the PICO Developer Platform.
	/// @note  Applicable to the following matchmaking modes: Quickmatch, Browse</summary>
	/// 
    /// <param name="InDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, int, ErrorCode, const FString&, ErrorMessage).</param>
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|Matchmaking")
	static void Cancel(UObject* WorldContextObject, FMatchmakingCancel InDelegate);

	/// <summary>Creates a matchmaking room, then enqueues and joins it.
	/// @note  Applicable to the following matchmaking modes: Quickmatch, Browse, Advanced (Can Users Create Rooms=`true`)</summary>
	/// 
	/// <param name="pool">The matchmaking pool to use, which is created on the PICO Developer Platform.</param>
	/// <param name="MatchmakingOptions">(Optional) Additional matchmaking configuration for this request.</param>
    /// <param name="InDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, int, ErrorCode, const FString&, ErrorMessage, UPico_MatchmakingEnqueueResultAndRoom *, MatchmakingEnqueueResultAndRoom).</param>
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|Matchmaking")
	static void CreateAndEnqueueRoom2(UObject* WorldContextObject, const FString& Pool, FPicoMatchmakingOptions MatchmakingOptions, FMatchmakingCreateAndEnqueueRoom2 InDelegate);

	/// <summary>Enqueues for an available matchmaking room to join.
	/// When the server finds a match, it will return a message of
	/// type `MessageType.Notification_Matchmaking_MatchFound`. You
	/// can join found matching rooms by calling `RoomService.Join2`. 
	/// If you want to cancel the match early, you can use `MatchmakingService.Cancel`.
	/// @note  Applicable to the following matchmaking mode: Quickmatch</summary>
	/// 
	/// <param name="pool">The matchmaking pool to use, which is defined on the PICO Developer Platform.</param>
	/// <param name="MatchmakingOptions">(Optional) Match configuration for Enqueue.</param>
    /// <param name="InDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, int, ErrorCode, const FString&, ErrorMessage, UPico_MatchmakingEnqueueResult *, MatchmakingEnqueueResult).</param>
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|Matchmaking")
	static void Enqueue2(UObject* WorldContextObject, const FString& pool, FPicoMatchmakingOptions MatchmakingOptions, FMatchmakingEnqueue2 InDelegate);

	/// <summary>Debugs the state of the current matchmaking pool queue.
	/// @note 
	/// * This function should not be used in production.
	/// * Applicable to the following matchmaking modes: Quickmatch, Browse
	///
	/// </summary>
	/// 
    /// <param name="InDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, int, ErrorCode, const FString&, ErrorMessage, UPico_MatchmakingAdminSnapshot *, MatchmakingAdminSnapshot).</param>
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|Matchmaking")
	static void GetAdminSnapshot(UObject* WorldContextObject, FMatchmakingGetAdminSnapshot InDelegate);

	/// <summary>Reports that a skill-rating match has started.
	/// You can use this method after joining the room.
	/// @note 
	/// * This function is only for pools with skill-based matching.
	/// * Applicable to the following matchmaking modes: Quickmatch, Browse (+ Skill Pool)
	/// </summary>
	///
	/// <param name="RoomId">The ID of the room you want to match.</param>
    /// <param name="InDelegate">Will be executed when the request has been completed.
    /// Delegate will contain the requested object class (bool, bIsError, int, ErrorCode, const FString&, ErrorMessage).</param>
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|Matchmaking")
	static void StartMatch(UObject* WorldContextObject, const FString& RoomId, FMatchmakingStartMatch InDelegate);
};

/** @} */
/** @} */

/// @brief UPico_MatchmakingStats class.
UCLASS(BlueprintType)
class ONLINESUBSYSTEMPICO_API UPico_MatchmakingStats : public UObject
{
	GENERATED_BODY()
private:
	unsigned int DrawCount;
	unsigned int LossCount;
	unsigned int SkillLevel;
	double SkillMean;
	double SkillStandardDeviation;
	unsigned int WinCount;

public:
	void InitParams(ppfMatchmakingStatsHandle InHandle);

	/** @brief The current user's number of draws. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Stats")
	int32 GetDrawCount();

	/** @brief The current user's number of losses. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Stats")
	int32 GetLossCount();

	/** @brief The current user's skill level for the current matchmaking pool. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Stats")
	int32 GetSkillLevel();

	/** @brief The average of all skill levels for the current matchmaking pool. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Stats")
	float GetSkillMean();

	/** @brief The standard deviation of all skill levels for the current matchmaking pool */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Stats")
	float GetSkillStandardDeviation();

	/** @brief The current user's number of wins. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Stats")
	int32 GetWinCount();

	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Stats")
	FString GetDebugString();
};

/// @brief UPico_MatchmakingBrowseResult class.
UCLASS(BlueprintType)
class ONLINESUBSYSTEMPICO_API UPico_MatchmakingBrowseResult : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY()
	UPico_MatchmakingEnqueueResult* EnqueueResult;
	UPROPERTY()

	UPico_MatchmakingRoomList* MatchmakingRooms;
public:
	void InitParams(ppfMatchmakingBrowseResultHandle InHandle);

	/** @brief Matchmaking enqueue result. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Browse Result")
	UPico_MatchmakingEnqueueResult* GetMatchmakingEnqueueResult();

	/** @brief The list of matchmaking rooms. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Browse Result")
	UPico_MatchmakingRoomList* GetMatchmakingRooms();

	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Browse Result")
	FString GetDebugString();
};

/// @brief UPico_MatchmakingRoomList class.
UCLASS(BlueprintType)
class ONLINESUBSYSTEMPICO_API UPico_MatchmakingRoomList : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TArray<UPico_MatchmakingRoom*> MatchmakingRoomArray;
	FString NextPageParam = FString();
	int32 Size = 0;
	int32 TotalSize = 0;
	bool bHasNextPage;
public:
	void InitParams(ppfMatchmakingRoomArrayHandle InHandle);

	/** @brief Get MatchmakingRoomArray element form Index.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Room Array")
	UPico_MatchmakingRoom* GetElement(int32 Index);


	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Room Array")
	FString GetNextPageParam();

	/** @brief Get the size of MatchmakingRoomArray .*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Room Array")
	int32 GetSize();
    
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Room Array")
	int32 GetTotalSize();

	/** @brief Get whether the list has the next page.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Room Array")
	bool HasNextPage();

	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Room Array")
	FString GetDebugString();
};

/// @brief UPico_MatchmakingRoom class.
UCLASS(BlueprintType)
class ONLINESUBSYSTEMPICO_API UPico_MatchmakingRoom : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY()
	UPico_Room* Room;
	unsigned int PingTime;
	bool HasPingTime;

public:
	void InitParams(ppfMatchmakingRoomHandle InHandle);

	/** @brief Room info. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Room")
	UPico_Room* GetRoom();

	/** @brief Currently, always `0`. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Room")
	int32 GetPingTime();

	/** @brief Currently, always `false`. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Room")
	bool GetHasPingTime();

	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Room")
	FString GetDebugString();
};

/// @brief UPico_MatchmakingEnqueueResultAndRoom class.
UCLASS(BlueprintType)
class ONLINESUBSYSTEMPICO_API UPico_MatchmakingEnqueueResultAndRoom : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UPico_MatchmakingEnqueueResult* MatchmakingEnqueueResult;
	UPROPERTY()
	UPico_Room* Room;

public:
	void InitParams(ppfMatchmakingEnqueueResultAndRoomHandle InHandle);

	/** @brief Matchmaking enqueue result. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Enqueue Result And Room")
	UPico_MatchmakingEnqueueResult* GetMatchmakingEnqueueResult();

	/** @brief Matchmaking room info. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Enqueue Result And Room")
	UPico_Room* GetRoom();

	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Enqueue Result And Room")
	FString GetDebugString();
};

/// @brief UPico_MatchmakingEnqueueResult class.
UCLASS(BlueprintType)
class ONLINESUBSYSTEMPICO_API UPico_MatchmakingEnqueueResult : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UPico_MatchmakingAdminSnapshot* AdminSnapshotOptional;
	unsigned int AverageWait; 
	unsigned int MatchesInLastHourCount;
	unsigned int MaxExpectedWait;
	FString Pool; 
	unsigned int RecentMatchPercentage;

public:
	void InitParams(ppfMatchmakingEnqueueResultHandle InHandle);

	/** @brief Matchmaking snapshot options. Used for debugging only. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Enqueue Result")
	UPico_MatchmakingAdminSnapshot* GetAdminSnapshotOptional();

	/** @brief The average waiting time. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Enqueue Result")
	int32 GetAverageWait();

	/** @brief The number of matches made in the last hour. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Enqueue Result")
	int32 GetMatchesInLastHourCount();

	/** @brief The expected longest waiting time. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Enqueue Result")
	int32 GetMaxExpectedWait();
	
	/** @brief Matchmaking pool name. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Enqueue Result")
	FString GetPool();

	/** @brief Match rate. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Enqueue Result")
	int32 GetRecentMatchPercentage();

	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Enqueue Result")
	FString GetDebugString();
};

/// @brief UPico_MatchmakingAdminSnapshot class.
UCLASS(BlueprintType)
class ONLINESUBSYSTEMPICO_API UPico_MatchmakingAdminSnapshot : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UPico_MatchmakingAdminSnapshotCandidateList* CandidateList;
	double MyCurrentThreshold;

public:
	void InitParams(ppfMatchmakingAdminSnapshotHandle InHandle);

	/** @brief List of matchmaking candidates */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Admin Snapshot")
	UPico_MatchmakingAdminSnapshotCandidateList* GetCandidateList();

	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Admin Snapshot")
	float GetMyCurrentThreshold();
	
	/** @brief The current matching threshold. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Admin Snapshot")
	FString GetDebugString();
};

/// @brief UPico_MatchmakingAdminSnapshotCandidateList class.
UCLASS(BlueprintType)
class ONLINESUBSYSTEMPICO_API UPico_MatchmakingAdminSnapshotCandidateList : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TArray<UPico_MatchmakingAdminSnapshotCandidate*> MatchmakingAdminSnapshotCandidateArray;
	FString NextPageParam = FString();
	size_t Size = 0;
	int32 TotalSize = 0;
	bool bHasNextPage;
public:
	void InitParams(ppfMatchmakingAdminSnapshotCandidateArrayHandle InHandle);

	/** @brief Get MatchmakingAdminSnapshotCandidateArray element form Index.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking AdminSnapshot Candidate Array")
	UPico_MatchmakingAdminSnapshotCandidate* GetElement(int32 Index);

	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking AdminSnapshot Candidate Array")
	FString GetNextPageParam();

	/** @brief Get the size of MatchmakingAdminSnapshotCandidateArray .*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking AdminSnapshot Candidate Array")
	int32 GetSize();
    
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking AdminSnapshot Candidate Array")
	int32 GetTotalSize();

	/** @brief Get whether the list has the next page.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking AdminSnapshot Candidate Array")
	bool HasNextPage();
	
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking AdminSnapshot Candidate Array")
	FString GetDebugString();
};

/// @brief UPico_MatchmakingAdminSnapshotCandidate class.
UCLASS(BlueprintType)
class ONLINESUBSYSTEMPICO_API UPico_MatchmakingAdminSnapshotCandidate : public UObject
{
	GENERATED_BODY()

private:
	bool CanMatch;
	double MyTotalScore;
	double TheirCurrentThreshold;

public:
	void InitParams(ppfMatchmakingAdminSnapshotCandidateHandle InHandle);

	/** @brief Whether me and the other user can be matched. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Admin Snapshot Candidate")
	bool GetCanMatch();

	/** @brief My matching threshold. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Admin Snapshot Candidate")
	float GetMyTotalScore();

	/** @brief The other user's matching threshold. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Admin Snapshot Candidate")
	float GetTheirCurrentThreshold();
	
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Matchmaking|Matchmaking Admin Snapshot Candidate")
	FString GetDebugString();
};
