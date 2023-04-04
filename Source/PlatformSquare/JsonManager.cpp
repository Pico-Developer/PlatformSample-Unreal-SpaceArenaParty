// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonManager.h"

#include "Json.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializerMacros.h"
#include "Serialization/JsonSerializer.h"

FString UJsonManager::MakeJsonLaunchExtra(FLaunchExtraInfo InLaunchExtraInfo)
{
    TSharedPtr<FJsonObject> Root = MakeShareable(new FJsonObject);
    Root->SetStringField(TEXT("SessionRoomID"), InLaunchExtraInfo.SessionRoomID);
    Root->SetStringField(TEXT("MapName"), InLaunchExtraInfo.MapName);
    Root->SetStringField(TEXT("RTCRoomID"), InLaunchExtraInfo.RTCRoomID);
    Root->SetStringField(TEXT("LocationInfo"), InLaunchExtraInfo.LocationInfo);
    FString JsonStr;
    if (Root.IsValid() && Root->Values.Num() > 0)
    {
        TSharedRef<TJsonWriter<TCHAR>> JsonWrite = TJsonWriterFactory<TCHAR>::Create(&JsonStr);
        FJsonSerializer::Serialize(Root.ToSharedRef(), JsonWrite);
    }
    return JsonStr;
}

bool UJsonManager::ParseJsonLaunchExtra(const FString& InJsonStr, FLaunchExtraInfo &OutLaunchExtraInfo)
{
    TSharedPtr<FJsonObject> Root = MakeShareable(new FJsonObject);

    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InJsonStr);
    if (FJsonSerializer::Deserialize(Reader, Root))
    {
        UE_LOG(LogTemp, Warning, TEXT("Deserialize true "));
        FString Str;
        if (Root->TryGetStringField(TEXT("LocationInfo"), Str))
        {
            UE_LOG(LogTemp, Warning, TEXT("LocationInfo: %s"), *Str);
            OutLaunchExtraInfo.LocationInfo = Str;
        }
        if (Root->TryGetStringField(TEXT("MapName"), Str))
        {
            UE_LOG(LogTemp, Warning, TEXT("MapName: %s"), *Str);
            OutLaunchExtraInfo.MapName = Str;
        }
        if (Root->TryGetStringField(TEXT("SessionRoomID"), Str))
        {
            UE_LOG(LogTemp, Warning, TEXT("SessionRoomID: %s"), *Str);
            OutLaunchExtraInfo.SessionRoomID = Str;
        }
        if (Root->TryGetStringField(TEXT("RTCRoomID"), Str))
        {
            UE_LOG(LogTemp, Warning, TEXT("RTCRoomID: %s"), *Str);
            OutLaunchExtraInfo.RTCRoomID = Str;
        }
        return true;
    }
    UE_LOG(LogTemp, Warning, TEXT("Deserialize false "));
    return false;
}

