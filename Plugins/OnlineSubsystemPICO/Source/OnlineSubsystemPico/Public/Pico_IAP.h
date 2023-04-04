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
#include "Pico_IAP.generated.h"

/// @file Pico_IAP.h

DECLARE_LOG_CATEGORY_EXTERN(PicoIAP, Log, All);
/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FGetProductsBySKUDelegate, bool, bIsError, const FString&, ErrorMessage, UPico_ProductArray*, ProductArray);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FGetViewerPurchasesDelegate, bool, bIsError, const FString&, ErrorMessage, UPico_PurchaseArray*, PurchaseArray);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FGetNextProductArrayPageDelegate, bool, bIsError, const FString&, ErrorMessage, UPico_ProductArray*, ProductArray);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FConsumePurchaseDelegate, bool, bIsError, const FString&, ErrorMessage);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FGetNextPurchaseArrayPageDelegate, bool, bIsError, const FString&, ErrorMessage, UPico_PurchaseArray*, PurchaseArray);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FLaunchCheckoutFlowDelegate, bool, bIsError, const FString&, ErrorMessage, UPico_Purchase*, Purchase);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FLaunchCheckoutFlow_V2Delegate, bool, bIsError, const FString&, ErrorMessage, UPico_Purchase*, Purchase);

/** @addtogroup Function Function
 *  This is the Function group
 *  @{
 */

/** @defgroup IAP IAP
 *  This is the IAP group
 *  @{
 */

/// @brief PicoIAPInterface class.
class ONLINESUBSYSTEMPICO_API FPicoIAPInterface
{
private:

    FOnlineSubsystemPico& PicoSubsystem;

public:
	FPicoIAPInterface(FOnlineSubsystemPico& InSubsystem);
    ~FPicoIAPInterface();

	FConsumePurchaseDelegate ConsumePurchaseDelegate;
	FGetProductsBySKUDelegate GetProductsBySKUDelegate;
	FGetNextProductArrayPageDelegate GetNextProductArrayPageDelegate;
	FGetViewerPurchasesDelegate GetViewerPurchasesDelegate;
	FGetNextPurchaseArrayPageDelegate GetNextPurchaseArrayPageDelegate;
	FLaunchCheckoutFlowDelegate LaunchCheckoutFlowDelegate;
    FLaunchCheckoutFlow_V2Delegate LaunchCheckoutFlow_V2Delegate;

    /// <summary>Records the order fulfillment result for a consumable.
    /// @note Users are unable to repurchase the same consumable until the previous order is fulfilled.
    /// </summary>
    /// <param name ="SKU">The unique identifier of the consumable.</param> 
    /// <param name ="InConsumePurchaseDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, const FString&, ErrorMessage).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
	bool ConsumePurchase(const FString& SKU, FConsumePurchaseDelegate InConsumePurchaseDelegate);

    /// <summary>Gets a list of purchasable add-ons in the current app.</summary>
    /// <param name ="ProductSKUs">The unique identifiers of add-ons. If this parameter is empty, all purchasable add-ons will be returned.
    /// If SKUs are specified, add-ons with corresponding SKUs will be returned.</param>
    /// <param name ="Count">The number of 'ProductSKUs' arrays to return.</param> 
    /// <param name ="InGetProductsBySKUDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, const FString&, ErrorMessage, UPico_ProductArray *, ProductArray).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
	bool GetProductsBySKU(TArray<FString> ProductSKUs, int32 Count, FGetProductsBySKUDelegate InGetProductsBySKUDelegate);

    /// <summary>Gets the next page of purchasable add-ons.</summary>
    /// <param name ="InProductArray">The current object of product array.</param> 
    /// <param name ="InGetNextProductArrayPageDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, const FString&, ErrorMessage, UPico_ProductArray *, ProductArray).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
	bool GetNextProductsArrayPage(UPico_ProductArray* InProductArray, FGetNextProductArrayPageDelegate InGetNextProductArrayPageDelegate);

    /// <summary>Gets a list of purchased add-ons for a user.
    /// @note This list includes durables and unfulfilled consumables.
    /// </summary>
    /// <param name ="InGetViewerPurchasesDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, const FString&, ErrorMessage, UPico_PurchaseArray *, PurchaseArray).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
	bool GetViewerPurchases(FGetViewerPurchasesDelegate InGetViewerPurchasesDelegate);

    /// <summary>Gets the next page of purchased add-ons.</summary>
    /// <param name ="InPurchaseArray">The current object of purchase array.</param> 
    /// <param name ="InGetNextPurchaseArrayPageDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, const FString&, ErrorMessage, UPico_PurchaseArray *, PurchaseArray).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
	bool GetNextPurchaseArrayPage(UPico_PurchaseArray* InPurchaseArray, FGetNextPurchaseArrayPageDelegate InGetNextPurchaseArrayPageDelegate);

    /// <summary>
    /// Launches the checkout flow to purchase a durable or consumable.
    /// </summary>
    /// <param name ="SKU">The unique identifier of the add-on the user wants to purchase.</param> 
    /// <param name ="Price">The price for the add-on.</param> 
    /// <param name ="Currency">The currency of the payment.</param> 
    /// <param name ="InLaunchCheckoutFlowDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, const FString&, ErrorMessage, UPico_Purchase *, Purchase).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
	bool LaunchCheckoutFlow(const FString& SKU, const FString& Price, const FString& Currency, FLaunchCheckoutFlowDelegate InLaunchCheckoutFlowDelegate);

    /// <summary>
    /// Launches the checkout flow to purchase a subscription add-on.
    /// </summary>
    /// <param name ="SKU">The unique identifier of the subscription add-on the user wants to purchase.</param> 
    /// <param name ="Price">The price for the subscription add-on.</param> 
    /// <param name ="Currency">The currency of the payment.</param> 
    /// <param name ="OuterId">The unique identifier of the subscription period in the subscription add-on.</param> 
    /// <param name ="InLaunchCheckoutFlow_V2Delegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, const FString&, ErrorMessage, UPico_Purchase *, Purchase).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
    bool LaunchCheckoutFlowV2(const FString& SKU, const FString& Price, const FString& Currency, const FString& OuterId, FLaunchCheckoutFlow_V2Delegate InLaunchCheckoutFlow_V2Delegate);

};

/** @} */
/** @} */

/** @addtogroup BlueprintFunction BlueprintFunction
 *  This is the BlueprintFunction group
 *  @{
 */

/** @defgroup BP_IAP BP_IAP
 *  This is the BP_IAP group
 *  @{
 */

/// @brief OnlinePicoIAP Blueprint Function class.
UCLASS()
class ONLINESUBSYSTEMPICO_API UOnlinePicoIAPFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:


	/// <summary>Records the order fulfillment result for a consumable.
	/// @note Users are unable to repurchase the same consumable until the previous order is fulfilled.
    /// </summary>
    /// <param name ="WorldContextObject">Used to get the information about the current world.</param> 
    /// <param name ="SKU">The unique identifier of the add-on.</param> 
    /// <param name ="InConsumePurchaseDelegate">Will be executed when the request has been completed.
    /// Delegate will contain the requested object class (bool, bIsError, const FString&, ErrorMessage).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|IAP")
	static bool ConsumePurchase(UObject* WorldContextObject, const FString& SKU, FConsumePurchaseDelegate InConsumePurchaseDelegate);

    /// <summary>Gets a list of purchasable add-ons in the current app.</summary>
    /// <param name ="WorldContextObject">Used to get the information about the current world.</param> 
    /// <param name ="ProductSKUs">The unique identifiers of add-ons.</param> 
    /// <param name ="Count">The number of ProductSKUs arrays to return.</param> 
    /// <param name ="InGetProductsBySKUDelegate">Will be executed when the request has been completed.
    /// Delegate will contain the requested object class (bool, bIsError, const FString&, ErrorMessage, UPico_ProductArray *, ProductArray).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|IAP")
	static bool GetProductsBySKU(UObject* WorldContextObject, TArray<FString> ProductSKUs, int32 Count, FGetProductsBySKUDelegate InGetProductsBySKUDelegate);

    /// <summary>Gets the next page of purchasable add-ons.</summary>
    /// <param name ="WorldContextObject">Used to get the information about the current world.</param> 
    /// <param name ="InProductArray">The current object of product array.</param> 
    /// <param name ="InGetNextProductArrayPageDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, const FString&, ErrorMessage, UPico_ProductArray *, ProductArray).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|IAP")
	static bool GetNextProductsArrayPage(UObject* WorldContextObject, UPico_ProductArray* InProductArray, FGetNextProductArrayPageDelegate InGetNextProductArrayPageDelegate);

    /// <summary>Gets a list of purchased add-ons for a user in the current app.
	/// @note This list includes unfulfilled consumables and durables.
	/// </summary>
	/// <param name ="WorldContextObject">Used to get the information about the current world.</param> 
    /// <param name ="InGetViewerPurchasesDelegate">Will be executed when the request has been completed.
    /// Delegate will contain the requested object class (bool, bIsError, const FString&, ErrorMessage, UPico_PurchaseArray *, PurchaseArray).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|IAP")
	static bool GetViewerPurchases(UObject* WorldContextObject, FGetViewerPurchasesDelegate InGetViewerPurchasesDelegate);

    /// <summary>Gets the next page of purchased add-ons.</summary>
    /// <param name ="WorldContextObject">Used to get the information about the current world.</param> 
    /// <param name ="InPurchaseArray">The current object of purchase array.</param> 
    /// <param name ="InGetNextPurchaseArrayPageDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, const FString&, ErrorMessage, UPico_PurchaseArray *, PurchaseArray).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|IAP")
	static bool GetNextPurchaseArrayPage(UObject* WorldContextObject, UPico_PurchaseArray* InPurchaseArray, FGetNextPurchaseArrayPageDelegate InGetNextPurchaseArrayPageDelegate);

    /// <summary>
    /// Launches the checkout flow to purchase a consumable or durable.
    /// </summary>
    /// <param name ="WorldContextObject">Used to get the information about the current world.</param> 
    /// <param name ="SKU">The unique identifier of the add-on to purchase.</param> 
    /// <param name ="Price">The price for the add-on.</param> 
    /// <param name ="Currency">The currency of the payment.</param> 
    /// <param name ="InLaunchCheckoutFlowDelegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, const FString&, ErrorMessage, UPico_Purchase *, Purchase).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|IAP")
	static bool LaunchCheckoutFlow(UObject* WorldContextObject, const FString& SKU, const FString& Price, const FString& Currency, FLaunchCheckoutFlowDelegate InLaunchCheckoutFlowDelegate);

    /// <summary>
    /// Launches the checkout flow to purchase a subscription add-on.
    /// </summary>
    /// <param name ="WorldContextObject">Used to get the information about the current world.</param> 
    /// <param name ="SKU">The unique identifier of the subscription add-on the user wants to purchase.</param> 
    /// <param name ="Price">The price for the subscription add-on.</param> 
    /// <param name ="Currency">The currency of the payment.</param> 
    /// <param name ="OuterId">The unique identifier of the subscription period in the subscription add-on.</param> 
    /// <param name ="InLaunchCheckoutFlow_V2Delegate">Will be executed when the request has been completed. 
    /// Delegate will contain the requested object class (bool, bIsError, const FString&, ErrorMessage, UPico_Purchase *, Purchase).</param>
    /// <returns>Bool: 
    /// <ul>
    /// <li>`true`: success</li>
    /// <li>`false`: failure</li>
    /// </ul>
    /// </returns>
    UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "OnlinePico|IAP")
    static bool LaunchCheckoutFlowV2(UObject* WorldContextObject, const FString& SKU, const FString& Price, const FString& Currency, const FString& OuterId, FLaunchCheckoutFlow_V2Delegate InLaunchCheckoutFlow_V2Delegate);
};

/** @} */
/** @} */

/**
 * PPF_Product.h
 */

 /// @brief UPico_Product class.
UCLASS(BlueprintType)
class ONLINESUBSYSTEMPICO_API UPico_Product : public UObject
{
	GENERATED_BODY()

private:
	FString Description = FString();
    FString DetailDescription = FString();
	FString Price = FString();
	FString Currency = FString();
	FString Name = FString();
	FString SKU = FString();
    FString Icon = FString();
    EAddonsType AddonsType = EAddonsType::Unknown;
    EPeriodType PeriodType = EPeriodType::Unknown;
    EPeriodType TrialPeriodUnit = EPeriodType::Unknown;
    int32 TrialPeriodValue = -1;
    FString OuterId = FString();
    FString OriginalPrice = FString();
    bool bIsContinuous = false;

public:
	void InitParams(ppfProduct* InppfProductHandle);

    /**@brief The description of the add-on. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product")
	FString GetDescription();

    /**@brief The detailed description of the add-on. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product")
	FString GetDetailDescription();

    /**@brief The price of the add-on, which is a number string. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product")
	FString GetPrice();

    /**@brief Not use! */
    UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product")
    FString GetFormattedPrice();

    /**@brief The currency required for purchasing the add-on. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product")
	FString GetCurrency();

    /**@brief The name of the add-on. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product")
	FString GetName();

    /**@brief The unique identifier of the add-on. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product")
	FString GetSKU();

    /**@brief The icon of the add-on, which is an image URL.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product")
	FString GetIcon();

    /**@brief The type of the add-on */
    UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product")
    EAddonsType GetAddonsType();

    /**@brief The period type for the subscription add-on.*/
    UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product")
    EPeriodType GetPeriodType();

    /**@brief The trial period unit for the subscription add-on.*/
    UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product")
    EPeriodType GetTrialPeriodUnit();

    /**@brief The trial period value for the subscription add-on.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product")
	int32 GetTrialPeriodValue();

    /**@brief The unique identifier of a subscription period.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product")
	FString GetOuterId();

    /**@brief The original price of the add-on. This field means the price
     * without discount.
     */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product")
	FString GetOriginalPrice();

    /**@brief Whether the subscription is auto renewed.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product")
	bool GetIsContinuous();

};

/**
 * PPF_ProductArray.h
 */

 /// @brief UPico_ProductArray class.
UCLASS(BlueprintType)
class ONLINESUBSYSTEMPICO_API UPico_ProductArray : public UObject
{
	GENERATED_BODY()

private:

    UPROPERTY()
	TArray<UPico_Product*> ProductArray;
	int32 Size = 0;
	bool bHasNextPage;
	FString NextPageParam = FString();

public:
	void InitParams(ppfProductArray* InppfProductArrayHandle);

    /** @brief Get ProductArray element form Index.*/
    UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product Array")
	UPico_Product* GetElement(int32 Index);

	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product Array")
    FString GetNextPageParam();

    /** @brief Get the size of ProductArray .*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product Array")
    int32 GetSize();

    /** @brief Get whether the list has the next page.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Product Array")
    bool HasNextPage();
};

/**
 * PPF_Purchase.h
 */

 /// @brief UPico_Purchase class.
UCLASS(BlueprintType)
class ONLINESUBSYSTEMPICO_API UPico_Purchase : public UObject
{
	GENERATED_BODY()

private:
	int64 ExpirationTime = 0;
	int64 GrantTime = 0;
	FString ID = FString();
	FString SKU = FString();
    FString Icon = FString();
    EAddonsType AddonsType = EAddonsType::Unknown;
    FString OuterId = FString();
    EPeriodType CurrentPeriodType = EPeriodType::Unknown;
    EPeriodType NextPeriodType = EPeriodType::Unknown;
    int64 NextPayTime = 0;
    EDiscountType DiscountType = EDiscountType::Unknown;
public:
	void InitParams(ppfPurchase* InppfPurchaseHandle);


    /**@brief The expiration time. Only valid when it's subscription type.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Purchase")
	int64 GetExpirationTime();

    /**@brief The grant time. Only valid when it's subscription type.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Purchase")
	int64 GetGrantTime();

    /** @brief The ID of the purchase order. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Purchase")
	FString GetID();

    /** @brief The unique identifier of the add-on in the purchase order. */
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Purchase")
	FString GetSKU();

    /** @brief The icon of the add-on.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Purchase")
	FString GetIcon();

    /** @brief The type of the purchased add-on.*/
    UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Purchase")
    EAddonsType GetAddonsType();

    /** @brief The current period type of subscription. Only valid when it's subscription.*/
    UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Purchase")
    EPeriodType GetCurrentPeriodType();

    /** @brief The next period type of subscription. Only valid when it's subscription.*/
    UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Purchase")
    EPeriodType GetNextPeriodType();

    /** @brief The outer id of the purchased add-on.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Purchase")
	FString GetOuterId();

    /** @brief The next pay time of subscription. Only valid when it's subscription.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Purchase")
	int64 GetNextPayTime();

    /**@brief The discount info of the purchase.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Purchase")
	EDiscountType GetDiscountType();
};

/**
 * PPF_PurchaseArray.h
 */

 /// @brief UPico_PurchaseArray class.
UCLASS(BlueprintType)
class ONLINESUBSYSTEMPICO_API UPico_PurchaseArray : public UObject
{
	GENERATED_BODY()

private:

    UPROPERTY()
	TArray<UPico_Purchase*> PurchaseArray;
	int32 Size = 0;
	bool bHasNextPage;
	FString NextPageParam = FString();

public:
	void InitParams(ppfPurchaseArray* InppfPurchaseArrayHandle);

    /** @brief Get PurchaseArray element form Index.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Purchase Array")
	UPico_Purchase* GetElement(int32 Index);

	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Purchase Array")
	FString GetNextPageParam();

    /** @brief Get the size of PurchaseArray .*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Purchase Array")
	int32 GetSize();

    /** @brief Get whether the list has the next page.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|IAP|Purchase Array")
	bool HasNextPage();
};