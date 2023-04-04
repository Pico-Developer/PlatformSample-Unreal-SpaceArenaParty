// Fill out your copyright notice in the Description page of Project Settings.


#include "PicoAmbisonicsSettings.h"

#pragma region FAmbisonicsEncodingSettings_Impl
namespace Pxr_Audio
{
	namespace Spatializer
	{
		uint32 FAmbisonicsEncodingSettings::GetUniqueId() const
		{
			return Order;
		}

		TUniquePtr<ISoundfieldEncodingSettingsProxy> FAmbisonicsEncodingSettings::Duplicate() const
		{
			FAmbisonicsEncodingSettings* Proxy = new FAmbisonicsEncodingSettings();
			Proxy->Order = Order;
			return TUniquePtr<ISoundfieldEncodingSettingsProxy>(Proxy);
		}
	}
}
#pragma endregion FAmbisonicsEncodingSettings_Impl

#pragma region UPicoAmbisonicsSettings_Impl
TUniquePtr<ISoundfieldEncodingSettingsProxy> UPicoAmbisonicsSettings::GetProxy() const
{
	auto* Proxy = new Pxr_Audio::Spatializer::FAmbisonicsEncodingSettings();
	Proxy->Order = Order;
	return TUniquePtr<ISoundfieldEncodingSettingsProxy>(Proxy);
}
#pragma endregion UPicoAmbisonicsSettings_Impl
