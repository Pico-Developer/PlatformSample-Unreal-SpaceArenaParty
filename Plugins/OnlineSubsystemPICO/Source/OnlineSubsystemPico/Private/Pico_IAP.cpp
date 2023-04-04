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


#include "Pico_IAP.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemPico.h"
#include <vector>
#include <string>

DEFINE_LOG_CATEGORY(PicoIAP);

bool UOnlinePicoIAPFunction::ConsumePurchase(UObject* WorldContextObject, const FString& SKU, FConsumePurchaseDelegate InConsumePurchaseDelegate)
{
    UE_LOG(PicoIAP, Log, TEXT("UOnlinePicoIAPFunction::ConsumePurchase"));
    FOnlineSubsystemPico* Subsystem = static_cast<FOnlineSubsystemPico*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), PICO_SUBSYSTEM));
    if (Subsystem && Subsystem->GetPicoIAPInterface())
    {
        return Subsystem->GetPicoIAPInterface()->ConsumePurchase(SKU, InConsumePurchaseDelegate);
    }
    return false;
}

bool UOnlinePicoIAPFunction::GetProductsBySKU(UObject* WorldContextObject, TArray<FString> ProductSKUs, int32 Count, FGetProductsBySKUDelegate OnGetProductsBySKUCompleteDelegate)
{
    UE_LOG(PicoIAP, Log, TEXT("UOnlinePicoIAPFunction::GetProductsBySKU"));
    FOnlineSubsystemPico* Subsystem = static_cast<FOnlineSubsystemPico*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), PICO_SUBSYSTEM));
    if (Subsystem && Subsystem->GetPicoIAPInterface())
    {
        return Subsystem->GetPicoIAPInterface()->GetProductsBySKU(ProductSKUs, Count, OnGetProductsBySKUCompleteDelegate);
    }
    return false;
}

bool UOnlinePicoIAPFunction::GetNextProductsArrayPage(UObject* WorldContextObject, UPico_ProductArray* InProductArray, FGetNextProductArrayPageDelegate InGetNextProductArrayPageDelegate)
{
    UE_LOG(PicoIAP, Log, TEXT("UOnlinePicoIAPFunction::GetNextProductsArrayPage"));
    FOnlineSubsystemPico* Subsystem = static_cast<FOnlineSubsystemPico*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), PICO_SUBSYSTEM));
    if (Subsystem && Subsystem->GetPicoIAPInterface())
    {
        return Subsystem->GetPicoIAPInterface()->GetNextProductsArrayPage(InProductArray, InGetNextProductArrayPageDelegate);
    }
    return false;
}

bool UOnlinePicoIAPFunction::GetViewerPurchases(UObject* WorldContextObject, FGetViewerPurchasesDelegate InGetViewerPurchasesDelegate)
{
    UE_LOG(PicoIAP, Log, TEXT("UOnlinePicoIAPFunction::GetViewerPurchases"));
    FOnlineSubsystemPico* Subsystem = static_cast<FOnlineSubsystemPico*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), PICO_SUBSYSTEM));
    if (Subsystem && Subsystem->GetPicoIAPInterface())
    {
        return Subsystem->GetPicoIAPInterface()->GetViewerPurchases(InGetViewerPurchasesDelegate);
    }
    return false;
}

bool UOnlinePicoIAPFunction::GetNextPurchaseArrayPage(UObject* WorldContextObject, UPico_PurchaseArray* InPurchaseArray, FGetNextPurchaseArrayPageDelegate InGetNextPurchaseArrayPageDelegate)
{
    UE_LOG(PicoIAP, Log, TEXT("UOnlinePicoIAPFunction::GetNextPurchaseArrayPage"));
    FOnlineSubsystemPico* Subsystem = static_cast<FOnlineSubsystemPico*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), PICO_SUBSYSTEM));
    if (Subsystem && Subsystem->GetPicoIAPInterface())
    {
        return Subsystem->GetPicoIAPInterface()->GetNextPurchaseArrayPage(InPurchaseArray, InGetNextPurchaseArrayPageDelegate);
    }
    return false;
}

bool UOnlinePicoIAPFunction::LaunchCheckoutFlow(UObject* WorldContextObject, const FString& SKU, const FString& Price, const FString& Currency, FLaunchCheckoutFlowDelegate InLaunchCheckoutFlowDelegate)
{
    UE_LOG(PicoIAP, Log, TEXT("UOnlinePicoIAPFunction::LaunchCheckoutFlow"));
    FOnlineSubsystemPico* Subsystem = static_cast<FOnlineSubsystemPico*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), PICO_SUBSYSTEM));
    if (Subsystem && Subsystem->GetPicoIAPInterface())
    {
        return Subsystem->GetPicoIAPInterface()->LaunchCheckoutFlow(SKU, Price, Currency, InLaunchCheckoutFlowDelegate);
    }
    return false;
}

bool UOnlinePicoIAPFunction::LaunchCheckoutFlowV2(UObject* WorldContextObject, const FString& SKU, const FString& Price, const FString& Currency, const FString& OuterId, FLaunchCheckoutFlow_V2Delegate InLaunchCheckoutFlow_V2Delegate)
{
    UE_LOG(PicoIAP, Log, TEXT("UOnlinePicoIAPFunction::LaunchCheckoutFlow_V2"));
    FOnlineSubsystemPico* Subsystem = static_cast<FOnlineSubsystemPico*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), PICO_SUBSYSTEM));
    if (Subsystem && Subsystem->GetPicoIAPInterface())
    {
        return Subsystem->GetPicoIAPInterface()->LaunchCheckoutFlowV2(SKU, Price, Currency, OuterId, InLaunchCheckoutFlow_V2Delegate);
    }
    return false;
}

bool FPicoIAPInterface::ConsumePurchase(const FString& SKU, FConsumePurchaseDelegate InConsumePurchaseDelegate)
{
    UE_LOG(PicoIAP, Log, TEXT("FPicoIAPInterface::ConsumePurchase"));
    ppfRequest RequestId = ppf_IAP_ConsumePurchase(TCHAR_TO_UTF8(*SKU));
    PicoSubsystem.AddAsyncTask(RequestId, FPicoMessageOnCompleteDelegate::CreateLambda(
        [InConsumePurchaseDelegate, this](ppfMessageHandle Message, bool bIsError)
        {
            if (bIsError)
            {
                auto Error = ppf_Message_GetError(Message);
                FString ErrorMessage = UTF8_TO_TCHAR(ppf_Error_GetMessage(Error));
                FString ErrorCode = FString::FromInt(ppf_Error_GetCode(Error));
                ErrorMessage = ErrorMessage + FString(". Error Code: ") + ErrorCode;
                UE_LOG(PicoIAP, Log, TEXT("PicoConsumePurchase return failed:%s"), *ErrorMessage);
                this->ConsumePurchaseDelegate.ExecuteIfBound(true, ErrorMessage);
                InConsumePurchaseDelegate.ExecuteIfBound(true, ErrorMessage);
            }
            else
            {
                UE_LOG(PicoIAP, Log, TEXT("PicoConsumePurchase Successfully"));
                this->ConsumePurchaseDelegate.ExecuteIfBound(false, FString());
                InConsumePurchaseDelegate.ExecuteIfBound(false, FString());
            }
        }));
    return true;
}

bool FPicoIAPInterface::GetProductsBySKU(TArray<FString> ProductSKUs, int32 Count, FGetProductsBySKUDelegate InGetProductsBySKUDelegate)
{
    UE_LOG(PicoIAP, Log, TEXT("FPicoIAPInterface::GetProductsBySKU"));
    //char* SKUsArray = NULL;
    //SKUsArray = new char[ProductSKUs.Num()];
    //for (size_t i = 0; i < ProductSKUs.Num(); i++)
    //{
    //    SKUsArray[i] = FCString::Atoi(*ProductSKUs[i]);
    //}
    //const char**  SKUsArray2 = NULL;
    //SKUsArray2 = new const char*[ProductSKUs.Num()];
    //for (size_t i = 0; i < ProductSKUs.Num(); i++)
    //{
    //    SKUsArray2[i] = &SKUsArray[i];
    //}
    std::vector<std::string> StringArray;
    for (size_t i = 0; i < ProductSKUs.Num(); i++)
    {
        StringArray.push_back(std::string(TCHAR_TO_UTF8(*(ProductSKUs[i]))));
    }
    std::vector<const char*> CharPtrArray;
    for (size_t i = 0; i < StringArray.size(); i++)
    {
        CharPtrArray.push_back(StringArray[i].c_str());
    }
    ppfRequest RequestId = ppf_IAP_GetProductsBySKU(CharPtrArray.data(), Count);
    PicoSubsystem.AddAsyncTask(RequestId, FPicoMessageOnCompleteDelegate::CreateLambda(
        [InGetProductsBySKUDelegate, this](ppfMessageHandle Message, bool bIsError)
        {
            if (bIsError)
            {
                auto Error = ppf_Message_GetError(Message);
                FString ErrorMessage = UTF8_TO_TCHAR(ppf_Error_GetMessage(Error));
                FString ErrorCode = FString::FromInt(ppf_Error_GetCode(Error));
                ErrorMessage = ErrorMessage + FString(". Error Code: ") + ErrorCode;
                UE_LOG(PicoIAP, Log, TEXT("GetProductsBySKU return failed:%s"), *ErrorMessage);
                this->GetProductsBySKUDelegate.ExecuteIfBound(true, ErrorMessage, nullptr);
                InGetProductsBySKUDelegate.ExecuteIfBound(true, ErrorMessage, nullptr);
            }
            else
            {
                UE_LOG(PicoIAP, Log, TEXT("GetProductsBySKU Successfully"));
                UPico_ProductArray* Pico_ProductArray = NewObject<UPico_ProductArray>();
                Pico_ProductArray->InitParams(ppf_Message_GetProductArray(Message));
                this->GetProductsBySKUDelegate.ExecuteIfBound(false, FString(), Pico_ProductArray);
                InGetProductsBySKUDelegate.ExecuteIfBound(false, FString(), Pico_ProductArray);
            }
        }));
    return true;
}

bool FPicoIAPInterface::GetNextProductsArrayPage(UPico_ProductArray* InProductArray, FGetNextProductArrayPageDelegate InGetNextProductArrayPageDelegate)
{
    UE_LOG(PicoIAP, Log, TEXT("FPicoIAPInterface::GetNextProductsArrayPage"));
    ppfRequest RequestId = ppf_IAP_GetNextProductArrayPage(TCHAR_TO_UTF8(*InProductArray->GetNextPageParam()));
    PicoSubsystem.AddAsyncTask(RequestId, FPicoMessageOnCompleteDelegate::CreateLambda(
        [InGetNextProductArrayPageDelegate, this](ppfMessageHandle Message, bool bIsError)
        {
            if (bIsError)
            {
                auto Error = ppf_Message_GetError(Message);
                FString ErrorMessage = UTF8_TO_TCHAR(ppf_Error_GetMessage(Error));
                FString ErrorCode = FString::FromInt(ppf_Error_GetCode(Error));
                ErrorMessage = ErrorMessage + FString(". Error Code: ") + ErrorCode;
                UE_LOG(PicoIAP, Log, TEXT("GetNextProductsArrayPage return failed:%s"), *ErrorMessage);
                this->GetNextProductArrayPageDelegate.ExecuteIfBound(true, ErrorMessage, nullptr);
                InGetNextProductArrayPageDelegate.ExecuteIfBound(true, ErrorMessage, nullptr);
            }
            else
            {
                UE_LOG(PicoIAP, Log, TEXT("GetNextProductsArrayPage Successfully"));
                UPico_ProductArray* Pico_ProductArray = NewObject<UPico_ProductArray>();
                Pico_ProductArray->InitParams(ppf_Message_GetProductArray(Message));
                this->GetNextProductArrayPageDelegate.ExecuteIfBound(false, FString(), Pico_ProductArray);
                InGetNextProductArrayPageDelegate.ExecuteIfBound(false, FString(), Pico_ProductArray);
            }
        }));
    return true;
}

bool FPicoIAPInterface::GetViewerPurchases(FGetViewerPurchasesDelegate InGetViewerPurchasesDelegate)
{
    UE_LOG(PicoIAP, Log, TEXT("FPicoIAPInterface::GetViewerPurchases"));
    ppfRequest RequestId = ppf_IAP_GetViewerPurchases();
    PicoSubsystem.AddAsyncTask(RequestId, FPicoMessageOnCompleteDelegate::CreateLambda(
        [InGetViewerPurchasesDelegate, this](ppfMessageHandle Message, bool bIsError)
        {
            if (bIsError)
            {
                auto Error = ppf_Message_GetError(Message);
                FString ErrorMessage = UTF8_TO_TCHAR(ppf_Error_GetMessage(Error));
                FString ErrorCode = FString::FromInt(ppf_Error_GetCode(Error));
                ErrorMessage = ErrorMessage + FString(". Error Code: ") + ErrorCode;
                UE_LOG(PicoIAP, Log, TEXT("GetViewerPurchases return failed:%s"), *ErrorMessage);
                this->GetViewerPurchasesDelegate.ExecuteIfBound(true, ErrorMessage, nullptr);
                InGetViewerPurchasesDelegate.ExecuteIfBound(true, ErrorMessage, nullptr);
            }
            else
            {
                UE_LOG(PicoIAP, Log, TEXT("GetViewerPurchases Successfully"));
                UPico_PurchaseArray* Pico_PurchaseArray = NewObject<UPico_PurchaseArray>();
                Pico_PurchaseArray->InitParams(ppf_Message_GetPurchaseArray(Message));
                this->GetViewerPurchasesDelegate.ExecuteIfBound(false, FString(), Pico_PurchaseArray);
                InGetViewerPurchasesDelegate.ExecuteIfBound(false, FString(), Pico_PurchaseArray);
            }
        }));
    return true;
}

bool FPicoIAPInterface::GetNextPurchaseArrayPage(UPico_PurchaseArray* InPurchaseArray, FGetNextPurchaseArrayPageDelegate InGetNextPurchaseArrayPageDelegate)
{
    UE_LOG(PicoIAP, Log, TEXT("FPicoIAPInterface::GetNextPurchaseArrayPage"));
    ppfRequest RequestId = ppf_IAP_GetNextPurchaseArrayPage(TCHAR_TO_UTF8(*InPurchaseArray->GetNextPageParam()));
    PicoSubsystem.AddAsyncTask(RequestId, FPicoMessageOnCompleteDelegate::CreateLambda(
        [InGetNextPurchaseArrayPageDelegate, this](ppfMessageHandle Message, bool bIsError)
        {
            if (bIsError)
            {
                auto Error = ppf_Message_GetError(Message);
                FString ErrorMessage = UTF8_TO_TCHAR(ppf_Error_GetMessage(Error));
                FString ErrorCode = FString::FromInt(ppf_Error_GetCode(Error));
                ErrorMessage = ErrorMessage + FString(". Error Code: ") + ErrorCode;
                UE_LOG(PicoIAP, Log, TEXT("GetNextPurchaseArrayPage return failed:%s"), *ErrorMessage);
                this->GetNextPurchaseArrayPageDelegate.ExecuteIfBound(true, ErrorMessage, nullptr);
                InGetNextPurchaseArrayPageDelegate.ExecuteIfBound(true, ErrorMessage, nullptr);
            }
            else
            {
                UE_LOG(PicoIAP, Log, TEXT("GetNextPurchaseArrayPage Successfully"));
                UPico_PurchaseArray* Pico_PurchaseArray = NewObject<UPico_PurchaseArray>();
                Pico_PurchaseArray->InitParams(ppf_Message_GetPurchaseArray(Message));
                this->GetNextPurchaseArrayPageDelegate.ExecuteIfBound(false, FString(), Pico_PurchaseArray);
                InGetNextPurchaseArrayPageDelegate.ExecuteIfBound(false, FString(), Pico_PurchaseArray);
            }
        }));
    return true;
}

bool FPicoIAPInterface::LaunchCheckoutFlow(const FString& SKU, const FString& Price, const FString& Currency, FLaunchCheckoutFlowDelegate InLaunchCheckoutFlowDelegate)
{
    UE_LOG(PicoIAP, Log, TEXT("FPicoIAPInterface::LaunchCheckoutFlow"));
    ppfRequest RequestId = ppf_IAP_LaunchCheckoutFlow(TCHAR_TO_UTF8(*SKU), TCHAR_TO_UTF8(*Price), TCHAR_TO_UTF8(*Currency));
    PicoSubsystem.AddAsyncTask(RequestId, FPicoMessageOnCompleteDelegate::CreateLambda(
        [InLaunchCheckoutFlowDelegate, this](ppfMessageHandle Message, bool bIsError)
        {
            if (bIsError)
            {
                auto Error = ppf_Message_GetError(Message);
                FString ErrorMessage = UTF8_TO_TCHAR(ppf_Error_GetMessage(Error));
                FString ErrorCode = FString::FromInt(ppf_Error_GetCode(Error));
                ErrorMessage = ErrorMessage + FString(". Error Code: ") + ErrorCode;
                UE_LOG(PicoIAP, Log, TEXT("LaunchCheckoutFlow return failed:%s"), *ErrorMessage);
                this->LaunchCheckoutFlowDelegate.ExecuteIfBound(true, ErrorMessage, nullptr);
                InLaunchCheckoutFlowDelegate.ExecuteIfBound(true, ErrorMessage, nullptr);
            }
            else
            {
                UE_LOG(PicoIAP, Log, TEXT("LaunchCheckoutFlow Successfully"));
                UPico_Purchase* Pico_Purchase = NewObject<UPico_Purchase>();
                Pico_Purchase->InitParams(ppf_Message_GetPurchase(Message));
                this->LaunchCheckoutFlowDelegate.ExecuteIfBound(false, FString(), Pico_Purchase);
                InLaunchCheckoutFlowDelegate.ExecuteIfBound(false, FString(), Pico_Purchase);
            }
        }));
    return true;
}

bool FPicoIAPInterface::LaunchCheckoutFlowV2(const FString& SKU, const FString& Price, const FString& Currency, const FString& OuterId, FLaunchCheckoutFlow_V2Delegate InLaunchCheckoutFlow_V2Delegate)
{
    UE_LOG(PicoIAP, Log, TEXT("FPicoIAPInterface::LaunchCheckoutFlow_V2"));
#if PLATFORM_ANDROID
    ppfRequest RequestId = ppf_IAP_LaunchCheckoutFlowV2(TCHAR_TO_UTF8(*SKU), TCHAR_TO_UTF8(*Price), TCHAR_TO_UTF8(*Currency), TCHAR_TO_UTF8(*OuterId));
    PicoSubsystem.AddAsyncTask(RequestId, FPicoMessageOnCompleteDelegate::CreateLambda(
        [InLaunchCheckoutFlow_V2Delegate, this](ppfMessageHandle Message, bool bIsError)
        {
            if (bIsError)
            {
                auto Error = ppf_Message_GetError(Message);
                FString ErrorMessage = UTF8_TO_TCHAR(ppf_Error_GetMessage(Error));
                FString ErrorCode = FString::FromInt(ppf_Error_GetCode(Error));
                ErrorMessage = ErrorMessage + FString(". Error Code: ") + ErrorCode;
                UE_LOG(PicoIAP, Log, TEXT("LaunchCheckoutFlow_V2 return failed:%s"), *ErrorMessage);
                this->LaunchCheckoutFlow_V2Delegate.ExecuteIfBound(true, ErrorMessage, nullptr);
                InLaunchCheckoutFlow_V2Delegate.ExecuteIfBound(true, ErrorMessage, nullptr);
            }
            else
            {
                UE_LOG(PicoIAP, Log, TEXT("LaunchCheckoutFlow_V2 Successfully"));
                UPico_Purchase* Pico_Purchase = NewObject<UPico_Purchase>();
                Pico_Purchase->InitParams(ppf_Message_GetPurchase(Message));
                this->LaunchCheckoutFlow_V2Delegate.ExecuteIfBound(false, FString(), Pico_Purchase);
                InLaunchCheckoutFlow_V2Delegate.ExecuteIfBound(false, FString(), Pico_Purchase);
            }
        }));
    return true;
#endif
    return false;
}

void UPico_Product::InitParams(ppfProduct* InppfProductHandle)
{
    UE_LOG(PicoIAP, Log, TEXT("UPico_Product::InitParams"));
    Description = UTF8_TO_TCHAR(ppf_Product_GetDescription(InppfProductHandle));

    Price = UTF8_TO_TCHAR(ppf_Product_GetPrice(InppfProductHandle));
    Currency = UTF8_TO_TCHAR(ppf_Product_GetCurrency(InppfProductHandle));
    Name = UTF8_TO_TCHAR(ppf_Product_GetName(InppfProductHandle));
    SKU = UTF8_TO_TCHAR(ppf_Product_GetSKU(InppfProductHandle));

#if PLATFORM_ANDROID
    DetailDescription = UTF8_TO_TCHAR(ppf_Product_GetDetailDescription(InppfProductHandle));
    Icon = UTF8_TO_TCHAR(ppf_Product_GetIcon(InppfProductHandle));
    ppfAddonsType pAddonsType = ppf_Product_GetAddonsType(InppfProductHandle);
    switch (pAddonsType)
    {
    case ppfAddonsType::ppfAddonsType_Invalid :
        AddonsType = EAddonsType::Invalid;
    	break;
    case ppfAddonsType::ppfAddonsType_Durable:
        AddonsType = EAddonsType::Durable;
        break;
    case ppfAddonsType::ppfAddonsType_Consumable:
        AddonsType = EAddonsType::Consumable;
        break;
    case ppfAddonsType::ppfAddonsType_Subscription:
        AddonsType = EAddonsType::Subscription;
        break;
    }
    ppfPeriodType pPeriodType = ppf_Product_GetPeriodType(InppfProductHandle);
    switch (pPeriodType)
    {
    case ppfPeriodType::ppfPeriodType_Invalid:
        PeriodType = EPeriodType::Invalid;
    	break;
    case ppfPeriodType::ppfPeriodType_None:
        PeriodType = EPeriodType::None;
        break;
    case ppfPeriodType::ppfPeriodType_Hour:
        PeriodType = EPeriodType::Hour;
        break;
    case ppfPeriodType::ppfPeriodType_Day:
        PeriodType = EPeriodType::Day;
        break;
    case ppfPeriodType::ppfPeriodType_Week:
        PeriodType = EPeriodType::Week;
        break;
    case ppfPeriodType::ppfPeriodType_Month:
        PeriodType = EPeriodType::Month;
        break;
    case ppfPeriodType::ppfPeriodType_Quarter:
        PeriodType = EPeriodType::Quarter;
        break;
    case ppfPeriodType::ppfPeriodType_Year:
        PeriodType = EPeriodType::Year;
        break;
    }

    ppfPeriodType pTrialPeriodUnit = ppf_Product_GetTrialPeriodUnit(InppfProductHandle);
    switch (pTrialPeriodUnit)
    {
    case ppfPeriodType::ppfPeriodType_Invalid:
        TrialPeriodUnit = EPeriodType::Invalid;
        break;
    case ppfPeriodType::ppfPeriodType_None:
        TrialPeriodUnit = EPeriodType::None;
        break;
    case ppfPeriodType::ppfPeriodType_Hour:
        TrialPeriodUnit = EPeriodType::Hour;
        break;
    case ppfPeriodType::ppfPeriodType_Day:
        TrialPeriodUnit = EPeriodType::Day;
        break;
    case ppfPeriodType::ppfPeriodType_Week:
        TrialPeriodUnit = EPeriodType::Week;
        break;
    case ppfPeriodType::ppfPeriodType_Month:
        TrialPeriodUnit = EPeriodType::Month;
        break;
    case ppfPeriodType::ppfPeriodType_Quarter:
        TrialPeriodUnit = EPeriodType::Quarter;
        break;
    case ppfPeriodType::ppfPeriodType_Year:
        TrialPeriodUnit = EPeriodType::Year;
        break;
    }
    TrialPeriodValue = ppf_Product_GetTrialPeriodValue(InppfProductHandle);
    OuterId = UTF8_TO_TCHAR(ppf_Product_GetOuterId(InppfProductHandle));
    UE_LOG(PicoIAP, Log, TEXT("ppf_Product_GetOuterId return OuterID:%s"), *OuterId);
    OriginalPrice = UTF8_TO_TCHAR(ppf_Product_GetOriginalPrice(InppfProductHandle));
    bIsContinuous = ppf_Product_IsContinuous(InppfProductHandle);
#endif

}

FString UPico_Product::GetDescription()
{
    return Description;
}

FString UPico_Product::GetDetailDescription()
{
    return DetailDescription;
}

FString UPico_Product::GetPrice()
{
    return Price;
}

FString UPico_Product::GetFormattedPrice()
{
    return FString();
}

FString UPico_Product::GetCurrency()
{
    return Currency;
}

FString UPico_Product::GetName()
{
    return Name;
}

FString UPico_Product::GetSKU()
{
    return SKU;
}

FString UPico_Product::GetIcon()
{
    return Icon;
}

EAddonsType UPico_Product::GetAddonsType()
{
    return AddonsType;
}

EPeriodType UPico_Product::GetPeriodType()
{
    return PeriodType;
}

EPeriodType UPico_Product::GetTrialPeriodUnit()
{
    return TrialPeriodUnit;
}

int32 UPico_Product::GetTrialPeriodValue()
{
    return TrialPeriodValue;
}

FString UPico_Product::GetOuterId()
{
    return OuterId;
}

FString UPico_Product::GetOriginalPrice()
{
    return OriginalPrice;
}

bool UPico_Product::GetIsContinuous()
{   
    return bIsContinuous;
}

void UPico_ProductArray::InitParams(ppfProductArray* InppfProductArrayHandle)
{
    UE_LOG(PicoIAP, Log, TEXT("UPico_ProductArray::InitParams"));
    Size = ppf_ProductArray_GetSize(InppfProductArrayHandle);
    for (int32 i = 0; i < Size; i++)
    {
        UPico_Product* ThisElement = NewObject<UPico_Product>();
        ThisElement->InitParams(ppf_ProductArray_GetElement(InppfProductArrayHandle, i));
        ProductArray.Add(ThisElement);
    }
    bHasNextPage = ppf_ProductArray_HasNextPage(InppfProductArrayHandle);
    if (bHasNextPage)
    {
        NextPageParam = ppf_ProductArray_GetNextPageParam(InppfProductArrayHandle);
    }
}

UPico_Product* UPico_ProductArray::GetElement(int32 Index)
{
    if (ProductArray.IsValidIndex(Index))
    {
        return ProductArray[Index];
    }
    return nullptr;
}

FString UPico_ProductArray::GetNextPageParam()
{
    return NextPageParam;
}

int32 UPico_ProductArray::GetSize()
{
    return Size;
}

bool UPico_ProductArray::HasNextPage()
{
    return bHasNextPage;
}

void UPico_Purchase::InitParams(ppfPurchase* InppfPurchaseHandle)
{
    UE_LOG(PicoIAP, Log, TEXT("UPico_Purchase::InitParams"));
    ExpirationTime = ppf_Purchase_GetExpirationTime(InppfPurchaseHandle);
    GrantTime = ppf_Purchase_GetGrantTime(InppfPurchaseHandle);
    ID = UTF8_TO_TCHAR(ppf_Purchase_GetID(InppfPurchaseHandle));
    SKU = UTF8_TO_TCHAR(ppf_Purchase_GetSKU(InppfPurchaseHandle));

#if PLATFORM_ANDROID
    Icon = UTF8_TO_TCHAR(ppf_Purchase_GetIcon(InppfPurchaseHandle));
    OuterId = UTF8_TO_TCHAR(ppf_Purchase_GetOuterId(InppfPurchaseHandle));
    NextPayTime = ppf_Purchase_GetNextPayTime(InppfPurchaseHandle);
    ppfAddonsType pAddonsType = ppf_Purchase_GetAddonsType(InppfPurchaseHandle);
    switch (pAddonsType)
    {
    case ppfAddonsType::ppfAddonsType_Invalid:
        AddonsType = EAddonsType::Invalid;
        break;
    case ppfAddonsType::ppfAddonsType_Durable:
        AddonsType = EAddonsType::Durable;
        break;
    case ppfAddonsType::ppfAddonsType_Consumable:
        AddonsType = EAddonsType::Consumable;
        break;
    case ppfAddonsType::ppfAddonsType_Subscription:
        AddonsType = EAddonsType::Subscription;
        break;
    }

    ppfPeriodType pCurrentPeriodType = ppf_Purchase_GetCurrentPeriodType(InppfPurchaseHandle);
    switch (pCurrentPeriodType)
    {
    case ppfPeriodType::ppfPeriodType_Invalid:
        CurrentPeriodType = EPeriodType::Invalid;
        break;
    case ppfPeriodType::ppfPeriodType_None:
        CurrentPeriodType = EPeriodType::None;
        break;
    case ppfPeriodType::ppfPeriodType_Hour:
        CurrentPeriodType = EPeriodType::Hour;
        break;
    case ppfPeriodType::ppfPeriodType_Day:
        CurrentPeriodType = EPeriodType::Day;
        break;
    case ppfPeriodType::ppfPeriodType_Week:
        CurrentPeriodType = EPeriodType::Week;
        break;
    case ppfPeriodType::ppfPeriodType_Month:
        CurrentPeriodType = EPeriodType::Month;
        break;
    case ppfPeriodType::ppfPeriodType_Quarter:
        CurrentPeriodType = EPeriodType::Quarter;
        break;
    case ppfPeriodType::ppfPeriodType_Year:
        CurrentPeriodType = EPeriodType::Year;
        break;
    }

    ppfPeriodType pNextPeriodType = ppf_Purchase_GetNextPeriodType(InppfPurchaseHandle);
    switch (pNextPeriodType)
    {
    case ppfPeriodType::ppfPeriodType_Invalid:
        NextPeriodType = EPeriodType::Invalid;
        break;
    case ppfPeriodType::ppfPeriodType_None:
        NextPeriodType = EPeriodType::None;
        break;
    case ppfPeriodType::ppfPeriodType_Hour:
        NextPeriodType = EPeriodType::Hour;
        break;
    case ppfPeriodType::ppfPeriodType_Day:
        NextPeriodType = EPeriodType::Day;
        break;
    case ppfPeriodType::ppfPeriodType_Week:
        NextPeriodType = EPeriodType::Week;
        break;
    case ppfPeriodType::ppfPeriodType_Month:
        NextPeriodType = EPeriodType::Month;
        break;
    case ppfPeriodType::ppfPeriodType_Quarter:
        NextPeriodType = EPeriodType::Quarter;
        break;
    case ppfPeriodType::ppfPeriodType_Year:
        NextPeriodType = EPeriodType::Year;
        break;
}

    ppfDiscountType pDiscountType = ppf_Purchase_GetDiscountType(InppfPurchaseHandle);
    switch (pDiscountType)
    {
    case ppfDiscountType::ppfDiscountType_Invalid:
        DiscountType = EDiscountType::Invalid;
        break;
    case ppfDiscountType::ppfDiscountType_Null:
        DiscountType = EDiscountType::Null;
        break;
    case ppfDiscountType::ppfDiscountType_FreeTrial:
        DiscountType = EDiscountType::FreeTrial;
        break;
    case ppfDiscountType::ppfDiscountType_Discount:
        DiscountType = EDiscountType::Discount;
        break;
    }
#endif
}

int64 UPico_Purchase::GetExpirationTime()
{
    return ExpirationTime;
}

int64 UPico_Purchase::GetGrantTime()
{
    return GrantTime;
}

FString UPico_Purchase::GetID()
{
    return ID;
}

FString UPico_Purchase::GetSKU()
{
    return SKU;
}

FString UPico_Purchase::GetIcon()
{
    return Icon;    
}

EAddonsType UPico_Purchase::GetAddonsType()
{
    return AddonsType;
}

EPeriodType UPico_Purchase::GetCurrentPeriodType()
{
    return CurrentPeriodType;
}

EPeriodType UPico_Purchase::GetNextPeriodType()
{
    return NextPeriodType;
}

FString UPico_Purchase::GetOuterId()
{
    return OuterId;
}

int64 UPico_Purchase::GetNextPayTime()
{
    return NextPayTime;
}

EDiscountType UPico_Purchase::GetDiscountType()
{
    return DiscountType;
}

void UPico_PurchaseArray::InitParams(ppfPurchaseArray* InppfPurchaseArrayHandle)
{
    UE_LOG(PicoIAP, Log, TEXT("UPico_PurchaseArray::InitParams"));
    Size = ppf_PurchaseArray_GetSize(InppfPurchaseArrayHandle);
    for (int32 i = 0; i < Size; i++)
    {
        UPico_Purchase* ThisElement = NewObject<UPico_Purchase>();
        ThisElement->InitParams(ppf_PurchaseArray_GetElement(InppfPurchaseArrayHandle, i));
        PurchaseArray.Add(ThisElement);
    }
    bHasNextPage = ppf_PurchaseArray_HasNextPage(InppfPurchaseArrayHandle);
    if (bHasNextPage)
    {
        NextPageParam = ppf_PurchaseArray_GetNextPageParam(InppfPurchaseArrayHandle);
    }
}

UPico_Purchase* UPico_PurchaseArray::GetElement(int32 Index)
{
    if (PurchaseArray.IsValidIndex(Index))
    {
        return PurchaseArray[Index];
    }
    return nullptr;
}

FString UPico_PurchaseArray::GetNextPageParam()
{
    return NextPageParam;
}

int32 UPico_PurchaseArray::GetSize()
{
    return Size;
}

bool UPico_PurchaseArray::HasNextPage()
{
    return bHasNextPage;
}

FPicoIAPInterface::FPicoIAPInterface(FOnlineSubsystemPico& InSubsystem) :
    PicoSubsystem(InSubsystem)
{

}

FPicoIAPInterface::~FPicoIAPInterface()
{

}

