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

#include "Interfaces/OnlineAchievementsInterface.h"
#include "OnlineIdentityPico.h"
#include "OnlineSubsystemPicoPackage.h"

#define USE_PICO_ACHIEVEMENT_DESC FName(TEXT("USE_PICO_ACHIEVEMENT_DESC"))
#define USE_PICO_ACHIEVEMENT FName(TEXT("USE_PICO_ACHIEVEMNT"))
#define USE_PICO_ACHIEVEMENT_WRITE FName(TEXT("USE_PICO_ACHIEVEMENT_WRITE"))

/**
*	FOnlineAchievementDescPico - Interface class for accessing the oculus achievement description information
*/
struct FOnlineAchievementDescPico : FOnlineAchievementDesc
{
    /** The way this achievement is unlocked */
    EAchievementType Type;

    /** The value that needs to be reached for "Count" Type achievements to unlock */
    uint64 Target;

    /** How many fields needs to be set for "Bitfield" Type achievements to unlock */
    uint32 BitfieldLength;

    EAchievementWritePolicy WritePolicy;
    bool IsArchived;
    FString Name;
    FString LockedImageURL;
    FString UnlockedImageURL;				

    FString ToDebugString() const
    {
        return FString::Printf( TEXT("Name=%s\nTitle=%s\nLockedDesc=%s\nUnlockedDesc=%s\nbIsHidden=%s\nType=%d\nTarget=%llu\nBitfieldLength=%u\nIsArchived: %s\nLockedImageURL: %s\nUnlockedImageURL: %s\nWritePolicy: %d\n"),
            *Name, *Title.ToString(),
            *LockedDesc.ToString(),
            *UnlockedDesc.ToString(),
            bIsHidden ? TEXT("true") : TEXT("false"),
            Type,
            Target,
            BitfieldLength,
            IsArchived ? TEXT("true") : TEXT("false"),
            *LockedImageURL,
            *UnlockedImageURL,
            WritePolicy
            );
    }

};

/**
*	FOnlineAchievementPico - Interface class for accessing the pico achievement information
*/
struct FOnlineAchievementPico : FOnlineAchievement
{
    /** The player's current progress toward a targeted numeric goal */
    long long Count;

    /** The player's current progress toward a set of goals that doesn't have to be completed in order */
    FString Bitfield;

    /** Whether or not this achievement was unlocked */
    bool bIsUnlocked;
    
    FOnlineAchievementPico(const ppfAchievementProgressHandle& AchievementProgress) //:
        // Count(ppf_AchievementProgress_GetCount(AchievementProgress)),
        // Bitfield(ppf_AchievementProgress_GetBitfield(AchievementProgress)),
        // bIsUnlocked(ppf_AchievementProgress_GetIsUnlocked(AchievementProgress))
    {
    	Count = ppf_AchievementProgress_GetCount(AchievementProgress);
    	Bitfield = ppf_AchievementProgress_GetBitfield(AchievementProgress);
    	bIsUnlocked = ppf_AchievementProgress_GetIsUnlocked(AchievementProgress);
    	
        FString AchievementName = UTF8_TO_TCHAR(ppf_AchievementProgress_GetName(AchievementProgress));
        UE_LOG_ONLINE_ACHIEVEMENTS(Display, TEXT("PPF_GAME FOnlineAchievementPico constructor ppf_AchievementProgress_GetName: %s, ppf_AchievementProgress_GetCount: %lld"), *AchievementName, ppf_AchievementProgress_GetCount(AchievementProgress));
        Id = AchievementName;
    }

    FOnlineAchievementPico(const FOnlineAchievementDescPico& AchievementDesc) :
        Count(0),
        bIsUnlocked(false)
    {
        UE_LOG_ONLINE_ACHIEVEMENTS(Display, TEXT("PPF_GAME FOnlineAchievementPico constructor by FOnlineAchievementDescPico AchievementDesc.Name: %s"), *AchievementDesc.Name);
        Id = AchievementDesc.Name;
        Progress = 0;
        if (AchievementDesc.Type == EAchievementType::Bitfield)
        {
            Bitfield = TEXT("");
            for (uint32 i = 0; i < AchievementDesc.BitfieldLength; ++i)
            {
                Bitfield.AppendChar('0');
            }
        }
    }
    FOnlineAchievementPico(const FOnlineAchievement& OnlineAchievementPico) :
        Count(0),
        bIsUnlocked(false)
    {
        UE_LOG_ONLINE_ACHIEVEMENTS(Display, TEXT("PPF_GAME FOnlineAchievementPico constructor by FOnlineAchievement Id: %s"), *OnlineAchievementPico.Id);
        Id = OnlineAchievementPico.Id;
        Progress =  OnlineAchievementPico.Progress;
    }
};


/**
*	IOnlineAchievements - Interface class for achievements
*/
class FOnlineAchievementsPico : public IOnlineAchievements
{
private:
	
	/** Reference to the owning subsystem */
	FOnlineSubsystemPico& PicoSubsystem;

	/** Mapping of players to their achievements */
	TMap<FUniqueNetIdPico, TArray<FOnlineAchievement>> PlayerAchievements;

	/** Cached achievements (not player-specific) */
	TMap<FString, FOnlineAchievementDescPico> AchievementDescriptions;

	void GetWriteAchievementCountValue(FVariantData VariantData, long long& OutData) const;
	void GetWriteAchievementBitfieldValue(FVariantData VariantData, FString& OutData, uint32 BitfieldLength) const;
	double CalculatePlayerAchievementProgress(const FOnlineAchievementPico Achievement);

	
	void SaveLog(const ELogVerbosity::Type Verbosity, const FString & Log) const;

	const int MAX_REQUEST_SIZE = 50; // single max
	int GetAllProgressIndex = 0;
	int GetAllDefinitionIndex = 0;

public:

	/**
	* Constructor
	*
	* @param InSubsystem - A reference to the owning subsystem
	*/
	FOnlineAchievementsPico(FOnlineSubsystemPico& InSubsystem);

	/**
	* Default destructor
	*/
	virtual ~FOnlineAchievementsPico();

	// Begin IOnlineAchievements interface
	virtual void WriteAchievements(const FUniqueNetId& PlayerId, FOnlineAchievementsWriteRef& WriteObject, const FOnAchievementsWrittenDelegate& Delegate = FOnAchievementsWrittenDelegate()) override;
	virtual void QueryAchievements(const FUniqueNetId& PlayerId, const FOnQueryAchievementsCompleteDelegate& Delegate = FOnQueryAchievementsCompleteDelegate()) override;
	virtual void QueryAchievementDescriptions(const FUniqueNetId& PlayerId, const FOnQueryAchievementsCompleteDelegate& Delegate = FOnQueryAchievementsCompleteDelegate()) override;
	virtual EOnlineCachedResult::Type GetCachedAchievement(const FUniqueNetId& PlayerId, const FString& AchievementId, FOnlineAchievement& OutAchievement) override;
	virtual EOnlineCachedResult::Type GetCachedAchievements(const FUniqueNetId& PlayerId, TArray<FOnlineAchievement> & OutAchievements) override;
	virtual EOnlineCachedResult::Type GetCachedAchievementDescription(const FString& AchievementId, FOnlineAchievementDesc& OutAchievementDesc) override;
#if !UE_BUILD_SHIPPING
	virtual bool ResetAchievements(const FUniqueNetId& PlayerId) override;
#endif // !UE_BUILD_SHIPPING
	// End IOnlineAchievements interface
};
